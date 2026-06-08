#include "config.h"
#include "map.h"
#include "ray.h"

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

static void test_dda_matches_simple_for_oblique_rays(void) {
    Map map = make_test_map();
    const double angles[] = {0.35, 2.2, 4.8};

    for (int i = 0; i < (int)(sizeof(angles) / sizeof(angles[0])); i++) {
        RayHit simple = ray_check(&map, 5.5, 4.5, angles[i], 20.0);
        RayHit dda = ray_check_dda(&map, 5.5, 4.5, angles[i], 20.0);

        expect_int("oblique simple hit", simple.hit, 1);
        expect_int("oblique dda hit", dda.hit, 1);
        expect_near("oblique distance", dda.distance, simple.distance, 0.01);
        expect_int("oblique hit tile is wall", map_is_wall(&map, dda.map_x, dda.map_y), 1);
    }
}

int main(void) {
    test_axis_aligned_rays();
    test_max_distance_blocks_far_wall();
    test_dda_matches_simple_for_oblique_rays();

    if (failures != 0) {
        printf("%d ray test(s) failed\n", failures);
        return 1;
    }

    printf("ray tests passed\n");
    return 0;
}
