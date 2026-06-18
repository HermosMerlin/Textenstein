cmake_minimum_required(VERSION 3.20)

project(Textenstein C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(textenstein
    src/main.c
    src/map.c
    src/input.c
    src/player.c
    src/motion.c
    src/ray.c
    src/renderer.c
    src/console.c
    src/game_time.c
    src/wall_texture.c
    src/collision.c
    src/shoot.c
    
)

target_include_directories(textenstein PRIVATE include)

if (WIN32)
    target_link_libraries(textenstein PRIVATE user32)
endif()

if (MSVC)
    target_compile_options(textenstein PRIVATE /W4)
else()
    target_compile_options(textenstein PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(textenstein PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()

add_executable(test_ray
    test/test_ray.c
    src/map.c
    src/ray.c
    src/collision.c
)

target_include_directories(test_ray PRIVATE include)

if (MSVC)
    target_compile_options(test_ray PRIVATE /W4)
else()
    target_compile_options(test_ray PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_definitions(test_ray PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

add_test(NAME ray COMMAND test_ray)
