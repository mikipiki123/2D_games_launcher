
#include "../include/WindowManager.h"
#include "../include/buttonHandler.h"
#include "../include/CsvHandler.h"
#include <iostream>
#include "TetrisMain.h"

enum {
    QUIT,
    SNAKE,
    TETRIS
};

int main() {

    WindowManager* windowManager = new WindowManager(1080, 720);

    csvInit("records.csv");


    if (!windowManager->init()) {
        std::cout << "Error initializing window manager" << std::endl;
        return -1;
    }

    if (!windowManager->loadMedia("images/pixel_image.jpg")) {
        std::cout << "Error loading pixel images" << std::endl;
        return -1;
    }

    bool quit = false;

    SDL_Event e;

    ButtonHandler* snakeButton = new ButtonHandler("Snake", SNAKE, windowManager, {(int)windowManager->screen_width / 2, (int)windowManager->screen_height/2, 200, 60}, {100, 100, 100} ,{170, 170, 255}, {100, 100, 255});

    ButtonHandler* quitButton = new ButtonHandler("Quit",QUIT, windowManager, {38, 28, 60, 40}, {100, 100, 100} , {60, 60, 60},{30, 30, 30});

    ButtonHandler* tetrisButton = new ButtonHandler("Tetris",TETRIS,windowManager, {(int)windowManager->screen_width / 2, (int)windowManager->screen_height/2 + 80, 200, 60}, {100, 100, 100} ,{170, 170, 255}, {100, 100, 255});

    windowManager->music = Mix_LoadMUS("sounds/Wii Music - Gaming Background Music (HD).mp3");

    Mix_PlayMusic(windowManager->music, -1);


    while( !quit ) {


        while (SDL_PollEvent( &e ) != 0) {


            snakeButton->buttonStatus(&e, snakePlay); // same as below
            tetrisButton->buttonStatus(&e, tetrisPlay);


            quitButton->isHover = quitButton->isInside();

            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && quitButton->isHover) {
                quitButton->isPressed = true;
            }

            if (e.type == SDL_MOUSEBUTTONUP && quitButton->isPressed) {
                quitButton->isPressed = false;
                if (quitButton->isHover) {
                    quit = true;
                }
            }

        }


        SDL_RenderClear(windowManager->renderer);
        SDL_RenderCopy(windowManager->renderer, windowManager->backGround, nullptr, nullptr);
        snakeButton->buttonRender();
        quitButton->buttonRender();
        tetrisButton->buttonRender();
        SDL_RenderPresent( windowManager->renderer );
    }

    std::cout << "Hello, World!" << std::endl;

    windowManager->close();
}
