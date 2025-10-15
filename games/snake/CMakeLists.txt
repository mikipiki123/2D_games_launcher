cmake_minimum_required(VERSION 4.0)
project(snake)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_library(snake SHARED
        src/SnakeMain.cpp
        ../../src/WindowManager.cpp
        src/Snake.cpp
        src/GameLogic.cpp)

target_link_libraries(snake PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
