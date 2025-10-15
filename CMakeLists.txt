cmake_minimum_required(VERSION 4.0)
project(gameMenu)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(
        games/snake/include
        games/tetris/include
)

add_executable(gameMenu
        src/main.cpp
        src/buttonHandler.cpp
        src/buttonHandler.h
        src/CsvHandler.cpp
        src/CsvHandler.h
)

link_directories(
        games/snake
        games/tetris
)

target_link_libraries(gameMenu PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
        ${CMAKE_CURRENT_SOURCE_DIR}/games/snake/libsnake.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/games/tetris/libtetris.dylib
)

