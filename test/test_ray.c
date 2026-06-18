#include "config.h"
#include "map.h"
#include "ray.h"
#include "collision.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

static Map make_test_map(void) {
    static const char* rows[] = {
      "########",
      "#......#",
      "#......#",
      "#......#",
      "#......#",
      "#......#",
      "#......#",
      "########",
    };

    Map map = {0};
    map.height = (int)(sizeof(rows) / sizeof(rows[0]));
    map.width = (int)strlen(rows[0]);

    for (int y = 0; y < map.height; y++) {
        memcpy(map.tiles[y], rows[y], (size_t)map.width + 1);
    }

    return map;
}

static void expect_int(const char* name, int actual, int expected) {
    if (actual != expected) {
        printf("FAIL %s: expected %d, got %d\n", name, expected, actual);
        failures++;
    }
}

static void expect_near(const char* name, double actual, double expected, double eps) {
    if (fabs(actual - expected) > eps) {
        printf("FAIL %s: expected %.6f, got %.6f\n", name, expected, actual);
        failures++;
    }
}

static void expect_axis_ray(
  const char* name,
  const Map* map,
  double angle,
  int expected_map_x,
  int expected_map_y,
  double expected_distance,
  double expected_hit_x,
  double expected_hit_y) {
    RayHit hit = ray_check_dda(map, 5.5, 4.5, angle, 20.0);

    char label[128];
    snprintf(label, sizeof(label), "%s hit", name);
    expect_int(label, hit.hit, 1);

    snprintf(label, sizeof(label), "%s map_x", name);
    expect_int(label, hit.map_x, expected_map_x);

    snprintf(label, sizeof(label), "%s map_y", name);
    expect_int(label, hit.map_y, expected_map_y);

    snprintf(label, sizeof(label), "%s distance", name);
    expect_near(label, hit.distance, expected_distance, 1e-6);

    snprintf(label, sizeof(label), "%s hit_x", name);
    expect_near(label, hit.hit_x, expected_hit_x, 1e-6);

    snprintf(label, sizeof(label), "%s hit_y", name);
    expect_near(label, hit.hit_y, expected_hit_y, 1e-6);
}

static void test_axis_aligned_rays(void) {
    Map map = make_test_map();

    expect_axis_ray("right", &map, 0.0, 7, 4, 1.5, 7.0, 4.5);
    expect_axis_ray("down", &map, PI / 2.0, 5, 7, 2.5, 5.5, 7.0);
    expect_axis_ray("left", &map, PI, 0, 4, 4.5, 1.0, 4.5);
    expect_axis_ray("up", &map, 3.0 * PI / 2.0, 5, 0, 3.5, 5.5, 1.0);
}

static void test_max_distance_blocks_far_wall(void) {
    Map map = make_test_map();
    RayHit hit = ray_check_dda(&map, 5.5, 4.5, 0.0, 1.0);

    expect_int("max distance hit", hit.hit, 0);
    expect_near("max distance value", hit.distance, 1.0, 1e-6);
}

// 8x8 单墙房间,玩家中心 (4.5, 4.5),r=0.2
//   列 0 和 7、行 0 和 7 是墙;其余是空地
//   方框 [4.3,4.7]x[4.3,4.7] 落在空地内,安全
static void test_collision(void) {
    Map map = make_test_map();
    const double r = 0.2;

    // --- collision_is_safe ---
    // case 1: 空地中心应安全
    expect_int("is_safe 空地中心", collision_is_safe(&map, 4.5, 4.5, r), 1);
    // case 2: 压右墙(7.0+0.2=7.2 落到墙列 7)应不安全
    expect_int("is_safe 压右墙", collision_is_safe(&map, 7.0, 4.5, r), 0);
    // case 3: 压上墙(0.5-0.2=0.3 落到墙行 0)应不安全
    expect_int("is_safe 压上墙", collision_is_safe(&map, 4.5, 0.5, r), 0);
    // case 4: 角落压墙(7.0, 7.0)应不安全
    expect_int("is_safe 角落压墙", collision_is_safe(&map, 7.0, 7.0, r), 0);

    // --- collision_can_move_x ---
    // case 5: x 从 4.5 移到 4.7,方框右边 4.9 落列 4(空地),应允许
    expect_int("can_move_x 小步", collision_can_move_x(&map, 4.7, 4.5, r), 1);
    // case 6: x 从 4.5 移到 4.9,方框右边 5.1 落列 5(空地),应允许
    expect_int("can_move_x 接近边界", collision_can_move_x(&map, 4.9, 4.5, r), 1);
    // case 7: x 从 4.5 移到 6.9,方框右边 7.1 落列 7(墙),应禁止
    expect_int("can_move_x 撞右墙", collision_can_move_x(&map, 6.9, 4.5, r), 0);
    // case 8: x 从 4.5 移到 1.1,方框左边 0.9 落列 0(墙),应禁止
    expect_int("can_move_x 撞左墙", collision_can_move_x(&map, 1.1, 4.5, r), 0);

    // --- collision_can_move_y ---
    // case 9: y 从 4.5 移到 4.7,方框下边 4.9 落行 4(空地),应允许
    expect_int("can_move_y 小步", collision_can_move_y(&map, 4.5, 4.7, r), 1);
    // case 10: y 从 4.5 移到 4.9,方框下边 5.1 落行 5(空地),应允许
    expect_int("can_move_y 接近边界", collision_can_move_y(&map, 4.5, 4.9, r), 1);
    // case 11: y 从 4.5 移到 6.9,方框下边 7.1 落行 7(墙),应禁止
    //         此 case 抓 collision_can_move_y 内部坐标颠倒 bug:
    //         若 map_is_wall(map, y_min, cx) 颠倒,会查错格子,返回 1(穿墙)
    expect_int("can_move_y 撞下墙", collision_can_move_y(&map, 4.5, 6.9, r), 0);
    // case 12: y 从 4.5 移到 1.1,方框上边 0.9 落行 0(墙),应禁止
    //         此 case 抓调用方参数顺序 bug:若传成 (map, next_y, x, r) 会查错格子
    expect_int("can_move_y 撞上墙", collision_can_move_y(&map, 4.5, 1.1, r), 0);
}

int main(void) {
    test_axis_aligned_rays();
    test_max_distance_blocks_far_wall();
    test_collision();

    if (failures != 0) {
        printf("%d test(s) failed\n", failures);
        return 1;
    }

    printf("all tests passed\n");
    return 0;
}
