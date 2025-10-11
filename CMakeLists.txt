cmake_minimum_required(VERSION 4.0)
project(gameMenu)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(games/snake/include)

add_executable(gameMenu library.cpp
        buttonHandler.cpp
        buttonHandler.h
        CsvHandler.cpp
        CsvHandler.h
)

link_directories(games/snake)


target_link_libraries(gameMenu PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
        /Users/miki/Documents/programiki/cpp/gameMenu/games/snake/libsnake.dylib
)

