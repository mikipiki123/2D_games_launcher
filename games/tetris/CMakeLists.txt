cmake_minimum_required(VERSION 4.0)
project(tetris)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(../snake/include)

#add_executable(tetris src/main.cpp
#        src/gameLogic.cpp
#        src/shapes.cpp
#)

add_library(tetris SHARED
        src/TetrisMain.cpp
        src/gameLogic.cpp
        src/shapes.cpp
)

link_directories(../snake)


target_link_libraries(tetris PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
        ${CMAKE_CURRENT_SOURCE_DIR}/../snake/libsnake.dylib
)
