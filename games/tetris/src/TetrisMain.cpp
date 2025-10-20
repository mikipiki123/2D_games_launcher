#include <iostream>
#include "../../../include/WindowManager.h"
#include "gameLogic.h"
#include "../include/TetrisMain.h"

using namespace std;

#define PRINT_MATRIX()\
for (int i = 0; i < GAME_DIM_HEIGHT; ++i) { \
    for (int j = 0; j < GAME_DIM_WIDTH; ++j)\
        std::cout << gameLogic.savedMatrix[i][j] << ' ';\
    std::cout << '\n';\
}\
std::cout << "\n\n\n" << std::endl;\

UserData tetrisPlay(WindowManager* window, UserData* bestUser) {


    WindowManager* windowManager = NULL;
    if (window == NULL) {
        std::cout << "starting with default WindowManager" << std::endl;
        windowManager = new WindowManager(640, 480);
        if (!windowManager->init()) {
            std::cerr << "Failed to initialize SDL window." << std::endl;
            exit(1);
        }
    } else {
        std::cout << "Tetris Called!! with existed window: " << std::endl;
        windowManager = window;
    }

    if (!windowManager->loadMedia("games/tetris/images/tetrisBackground.jpg")) {
        std::cerr << "Error loading grass image" << std::endl;
        exit(1);
    }

    if (!windowManager->loadAudio("games/tetris/sounds/landed.wav")) {
        std::cout << "Failed to load audio." << std::endl;
        exit(1);
    }


    GameLogic gameLogic(windowManager);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    int highestScore = stoi(bestUser->score);
    bool gameOver = false;

    std::array<std::string,4> text = {"Score:", std::to_string(gameLogic.score), "Best score: " , bestUser->name + ": " + std::to_string(highestScore)};

    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    int tmpScore = 0;

    //text generate
    for (int i = 0; i < text.size(); i++) {
        if (!windowManager->loadFont(text[i], i)) {
            std::cout << "Failed to load Font." << std::endl;
            exit(1);
        }

        SDL_Rect textScore = {10, 5 + i * 20, 0, 0};

        SDL_QueryTexture(windowManager->text[i], nullptr, nullptr, &textScore.w, &textScore.h);

        SDL_RenderCopy(windowManager->renderer, windowManager->text[i], nullptr, &textScore);

    }

     //While application is running
    while( !quit ) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                bestUser->score = "-1";
                bestUser->name = ".";
                return *bestUser;
            }

            //User presses a key
            if( e.type == SDL_KEYDOWN ) {
                //Select surfaces based on key press
                switch( e.key.keysym.sym ) {
                    case SDLK_SPACE:
                        // start = std::chrono::high_resolution_clock::now();
                        gameLogic.checkAndRotate();
                        std::cout << "space!---------------" << std::endl;
                        break;

                    case SDLK_UP:
                        // start = std::chrono::high_resolution_clock::now();
                        gameLogic.checkAndRotate();
                        std::cout << "up!---------------" << std::endl;
                        break;

                    case SDLK_DOWN:
                        // start = std::chrono::high_resolution_clock::now();
                        std::cout << "down!---------------" << std::endl;
                        gameLogic.checkAndMove(KEY_PRESS_SURFACE_DOWN);
                        break;

                    case SDLK_LEFT:
                        // start = std::chrono::high_resolution_clock::now();
                        std::cout << "left!---------------" << std::endl;
                        gameLogic.checkAndMove(KEY_PRESS_SURFACE_LEFT);
                        break;

                    case SDLK_RIGHT:
                        // start = std::chrono::high_resolution_clock::now();
                        std::cout << "right!---------------" << std::endl;
                        gameLogic.checkAndMove(KEY_PRESS_SURFACE_RIGHT);
                        break;
                    default:
                    std::cout << "default!---------------" << std::endl;
                }
                //render game --
                gameLogic.updateMatrix();
                // PRINT_MATRIX()
                if (!gameLogic.checkAndGenShape()) {
                    quit = true;
                }
                //-------
            }
        }

        // Record end time
        auto end = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > gameLogic.speed) {
            std::cout << "TIME! 1" << std::endl;
            gameLogic.checkAndMove(KEY_PRESS_SURFACE_DOWN);
            gameLogic.updateMatrix();
            // PRINT_MATRIX()
            if (!gameLogic.checkAndGenShape()) {
                quit = true;
            }
            start = std::chrono::high_resolution_clock::now();


        }



        // SDL_SetRenderDrawColor(windowManager->renderer, 0, 0, 0, 255);

        //Clear screen
        SDL_RenderClear(windowManager->renderer);

        //draw Background
        if (windowManager->backGround == NULL) {
            std::cout << "Failed to load media. - background" << std::endl;
            SDL_SetRenderDrawColor(windowManager->renderer, 0, 0, 0, 255);
        } else {
            SDL_RenderCopy(windowManager->renderer, windowManager->backGround, nullptr, nullptr);
        }


        //draw game
        gameLogic.drawGame();

        //text
        if (gameLogic.score > tmpScore) {
            tmpScore = gameLogic.score;
            text[1] = std::to_string(gameLogic.score);

            SDL_DestroyTexture(windowManager->text[1]);
            if (!windowManager->loadFont(text[1], 1)) {
                std::cout << "Failed to load Font." << std::endl;
                exit(1);
            }
        }
        for (int i = 0; i < text.size(); i++) {
            SDL_Rect textScore = {10, 5 + i * 20, 0, 0};

            SDL_QueryTexture(windowManager->text[i], nullptr, nullptr, &textScore.w, &textScore.h);

            SDL_RenderCopy(windowManager->renderer, windowManager->text[i], nullptr, &textScore);
        }


        //Update screen
        SDL_RenderPresent( windowManager->renderer );
    }

    quit = false;

    Mix_PauseMusic();

    if (gameLogic.score > highestScore) {
        bestUser->score = std::to_string(gameLogic.score);

        std::string userInput = " ";

        SDL_StartTextInput(); // enable text input events

        if (!windowManager->loadMedia("games/tetris/images/arcadeWin.jpg")) {
            std::cout << "Failed to load game_over." << std::endl;
            exit(1);
        }

        SDL_Delay(200);

        if (!windowManager->loadAudio("games/tetris/sounds/COCWin.wav")) {
            std::cout << "Failed to load audio." << std::endl;
            exit(1);
        }

        Mix_PlayChannel(-1, windowManager->soundEffect, 0);

        text[2] = "Type your name:";
        text[3] = "Press ENTER";

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_TEXTINPUT) {
                    userInput += e.text.text; // append typed character
                }

                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && userInput != " ") {
                    std::cout << "-" <<userInput.c_str() << "-" << std::endl;
                    userInput.pop_back(); // delete last char
                }else if (e.key.keysym.sym == SDLK_RETURN && userInput.back() != ' ') {
                    // User pressed Enter -> print & clear
                    std::cout << "Submitted: " << userInput << std::endl;
                    gameOver = true;
                    quit = true;
                }
            }

            // clear screen
            // SDL_SetRenderDrawColor(windowManager->renderer, 0, 0, 0, 255);
            SDL_RenderClear(windowManager->renderer);
            SDL_RenderCopy(windowManager->renderer, windowManager->backGround, nullptr, nullptr);

            // render text
            SDL_Surface* surface = TTF_RenderText_Solid(windowManager->font, userInput.c_str(), {255, 255, 255, 255});
            SDL_Texture* texture = SDL_CreateTextureFromSurface(windowManager->renderer, surface);
            SDL_Rect dst = {275, 125, surface->w, surface->h};
            SDL_RenderCopy(windowManager->renderer, texture, NULL, &dst);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            for (int i = 0; i < text.size(); i++) {

                SDL_DestroyTexture(windowManager->text[i]);
                if (!windowManager->loadFont(text[i], i)) {
                    std::cout << "Failed to load Font." << std::endl;
                    exit(1);
                }

                double scale = 1.3;
                int tw,th;
                SDL_QueryTexture(windowManager->text[i], nullptr, nullptr, &tw, &th);
                SDL_Rect textRect = {20,
                                     (int)(windowManager->screen_height - (th*scale)) / 20 + (int)(1.2 * th * scale * i), (int)(tw*scale), (int)(th*scale)};
                SDL_RenderCopy(windowManager->renderer, windowManager->text[i], nullptr, &textRect);
            }

            SDL_RenderPresent(windowManager->renderer);
        }

        bestUser->name = userInput.substr(1, userInput.size() - 1);

    } else {

        if (!windowManager->loadMedia("games/tetris/images/arcadeLose.jpg")) {
            std::cerr << "Failed to load game_over." << std::endl;
            exit(1);
        }


        if (!windowManager->loadAudio("games/tetris/sounds/LOSE.wav")) {
            std::cerr << "Failed to load audio." << std::endl;
            exit(1);
        }

        Mix_PlayChannel(-1, windowManager->soundEffect, 0);

        while( !quit ) {
            while (SDL_PollEvent( &e ) != 0) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    std::cout << "Mouse Button pushed!" << std::endl;
                    gameOver = true;
                    quit = true;
                    break;
                }
            }

            SDL_RenderClear(windowManager->renderer);
            SDL_RenderCopy(windowManager->renderer, windowManager->backGround, nullptr, nullptr);

            for (int i = 0; i < text.size(); i++) {

                SDL_DestroyTexture(windowManager->text[i]);
                if (!windowManager->loadFont(text[i], i)) {
                    std::cout << "Failed to load Font." << std::endl;
                    exit(1);
                }

                double scale = 1.3;
                int tw,th;
                SDL_QueryTexture(windowManager->text[i], nullptr, nullptr, &tw, &th);
                SDL_Rect textRect = {(int)(windowManager->screen_width - (tw*scale)) / 2,
                                     (int)(windowManager->screen_height - (th*scale)) / 20 + (int)(1.2 * th * scale * i), (int)(tw*scale), (int)(th*scale)};
                SDL_RenderCopy(windowManager->renderer, windowManager->text[i], nullptr, &textRect);
            }

            SDL_RenderPresent( windowManager->renderer);
        }
        bestUser->score = "-1";
        bestUser->name = ".";
    }

    std::cout << "game ended successfully? " << gameOver << std::endl;
    Mix_ResumeMusic();



    if (gameOver) {
        return *bestUser;
    }

    bestUser->score = "-1";
    bestUser->name = ".";


    return *bestUser;
}