

#include "GameLogic.h"
#include "../include//WindowManager.h"
#include "../include/SnakeMain.h"


#define RESPONSE_TO_CORNER_OR_APPLE(x) \
    if (gameLogic.isCorner(x)) { \
        quit = true;}\
    if (gameLogic.isEaten(x)){ \
        Mix_PlayChannel(-1, windowManager->soundEffect, 0); \
        gameLogic.grow(); \
        score++; text[1] = std::to_string(score); \
        if (speed > 400){ \
            speed -= 20; \
        } \
        gameLogic.genApple();}\



UserData snakePlay(WindowManager* window, UserData* bestUser) {

    // WindowManager* windowManager = new WindowManager();

    WindowManager* windowManager = NULL;
    if (window == NULL) {
        std::cout << "starting with default WindowManager" << std::endl;
        windowManager = new WindowManager(640, 480);
        if (!windowManager->init()) {
            std::cout << "Failed to initialize SDL window." << std::endl;
            exit(1);
        }
    } else {
        std::cout << "Snake Called!! with existed window: " << std::endl;
        windowManager = window;
    }

    windowManager->loadMedia("images/grass.jpg");


    GameLogic gameLogic(windowManager);



    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    bool gameOver = false;

    //best score text:
    std::cout << bestUser->score.c_str() << std::endl;
    int highestScore = std::stoi(bestUser->score.c_str());


    int score = 0;

    // bestUser->game = "Snake";
    std::string sentence = bestUser->name + ": " + bestUser->score;
    std::cout << sentence << std::endl;
    std::array<std::string,4> text = {"Score:", std::to_string(score), "Best score: " , sentence};

    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    int speed = 800;

    if (!windowManager->loadAudio("sounds/ApplePay.wav")) {
        std::cout << "Failed to load audio." << std::endl;
        exit(1);
    }

    //While application is running
    while( !quit ) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            //User presses a key
            if( e.type == SDL_KEYDOWN ) {
                //Select surfaces based on key press
                switch( e.key.keysym.sym ) {
                    case SDLK_UP:
                        RESPONSE_TO_CORNER_OR_APPLE(KEY_PRESS_SURFACE_UP)
                        gameLogic.entities[0]->move(KEY_PRESS_SURFACE_UP);
                        gameLogic.entities[0]->lastMove = KEY_PRESS_SURFACE_UP;
                        start = std::chrono::high_resolution_clock::now();
                        std::cout << "up!---------------" << std::endl;
                        break;

                    case SDLK_DOWN:
                        RESPONSE_TO_CORNER_OR_APPLE(KEY_PRESS_SURFACE_DOWN)
                        gameLogic.entities[0]->move(KEY_PRESS_SURFACE_DOWN);
                        gameLogic.entities[0]->lastMove = KEY_PRESS_SURFACE_DOWN;
                        start = std::chrono::high_resolution_clock::now();
                        std::cout << "down!---------------" << std::endl;
                        break;

                    case SDLK_LEFT:
                        RESPONSE_TO_CORNER_OR_APPLE(KEY_PRESS_SURFACE_LEFT)
                        gameLogic.entities[0]->move(KEY_PRESS_SURFACE_LEFT);
                        gameLogic.entities[0]->lastMove = KEY_PRESS_SURFACE_LEFT;
                        start = std::chrono::high_resolution_clock::now();
                        std::cout << "left!---------------" << std::endl;
                        break;

                    case SDLK_RIGHT:
                        RESPONSE_TO_CORNER_OR_APPLE(KEY_PRESS_SURFACE_RIGHT)
                        gameLogic.entities[0]->move(KEY_PRESS_SURFACE_RIGHT);
                        gameLogic.entities[0]->lastMove = KEY_PRESS_SURFACE_RIGHT;
                        start = std::chrono::high_resolution_clock::now();
                        std::cout << "right!---------------" << std::endl;
                        break;
                    default:
                    std::cout << "default!---------------" << std::endl;
                }
                gameLogic.updateMatrixAndSkeleton();
            }
        }

        // Record end time
        auto end = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > speed) {
            std::cout << "TIME! 3" << std::endl;
            // keyboardToggle = true;
            RESPONSE_TO_CORNER_OR_APPLE(gameLogic.entities[0]->lastMove)
            gameLogic.entities[0]->move(gameLogic.entities[0]->lastMove);
            gameLogic.updateMatrixAndSkeleton();
            start = std::chrono::high_resolution_clock::now();
        }


        // SDL_SetRenderDrawColor(windowManager->renderer, 0, 0, 0, 255);

        //Clear screen
        SDL_RenderClear(windowManager->renderer);

        //draw Background
        if (windowManager->backGround == NULL) {
            std::cout << "Failed to load media. - grass.jpg" << std::endl;
            SDL_SetRenderDrawColor(windowManager->renderer, 0, 0, 0, 255);
        } else {
            SDL_RenderCopy(windowManager->renderer, windowManager->backGround, nullptr, nullptr);
        }



        //draw game
        gameLogic.drawGame();

        //text
        for (int i = 0; i < text.size(); i++) {
            if (!windowManager->loadFont(text[i], i)) {
                std::cout << "Failed to load Font." << std::endl;
                exit(1);
            }

            int tw,th;
            SDL_Rect textScore = {10, 5 + i * 20, 0, 0};

            SDL_QueryTexture(windowManager->text[i], nullptr, nullptr, &textScore.w, &textScore.h);

            SDL_RenderCopy(windowManager->renderer, windowManager->text[i], nullptr, &textScore);
        }

        //Update screen
        SDL_RenderPresent( windowManager->renderer );

    }

    quit = false;

    Mix_PauseMusic();

    if (score > highestScore) {
        bestUser->score = std::to_string(score);

        std::string userInput = " ";

        SDL_StartTextInput(); // enable text input events

        if (!windowManager->loadMedia("images/winImage.jpg")) {
            std::cout << "Failed to load game_over." << std::endl;
            exit(1);
        }

        SDL_Delay(200);

        if (!windowManager->loadAudio("sounds/WIN.wav")) {
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

        if (!windowManager->loadMedia("images/game_over.jpg")) {
            std::cerr << "Failed to load game_over." << std::endl;
            exit(1);
        }

        SDL_Delay(200);

        if (!windowManager->loadAudio("sounds/LOSE.wav")) {
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

            SDL_RenderPresent( windowManager->renderer );
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