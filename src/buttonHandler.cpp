//
// Created by Michael Berner on 08/10/2025.
//

#include "buttonHandler.h"

#include "CsvHandler.h"

ButtonHandler::ButtonHandler(WindowManager *window, SDL_Rect rect, SDL_Color normalColor, SDL_Color hoverColor ,SDL_Color clickedColor) {
    this->windowManager = window;

    this->rect = rect; //centralize
    this->rect.x = this->rect.x - rect.w / 2;
    this->rect.y = this->rect.y - rect.h / 2;

    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->clickedColor = clickedColor;
}


bool ButtonHandler::isInside() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
}

void ButtonHandler::buttonRender() {
    double scale = 1.0;

    // Set color based on state shnoozel
    SDL_Color color;
    if (this->isPressed) {
        color = this->clickedColor;
        scale = 1.1;
    } else if (this->isHover){
        // {170, 170, 255}
        color = this->hoverColor; //todo non constant color
        scale = 1.1;
        // std::cout << "On the button! -----" << std::endl;
    } else
        color = this->normalColor;

    // Draw button rectangle
    SDL_SetRenderDrawColor(this->windowManager->renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(this->windowManager->renderer, &rect);

    // Draw button border
    SDL_SetRenderDrawColor(this->windowManager->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(this->windowManager->renderer, &rect);

    // Render button text
    if (!windowManager->loadFont(this->buttonText, 0)) { // 0 for first texture
        std::cout << "Failed to load Font." << std::endl;
        exit(1);
    }

    int tw, th;
    SDL_QueryTexture(windowManager->text[0], nullptr, nullptr, &tw, &th);
    SDL_Rect textRect = {this->rect.x + (this->rect.w - (int)(tw*scale)) / 2,
                         this->rect.y + (this->rect.h - (int)(th*scale)) / 2, (int)(tw*scale), (int)(th*scale)};
    SDL_RenderCopy(windowManager->renderer, windowManager->text[0], nullptr, &textRect);
}

void ButtonHandler::buttonStatus(SDL_Event* e, UserData (*GameFunction)(WindowManager*, UserData*)) {
    std::vector<UserData> bestUsers = csvRead("records.csv"); //todo no need for game name
    UserData bestUser;

    std::string buttonGame = this->buttonText;
    transform(buttonGame.begin(), buttonGame.end(), buttonGame.begin(), ::tolower);

    for (int i = 0; i < bestUsers.size(); i++) {
        if (bestUsers[i].game == buttonGame) {
            bestUser = bestUsers[i];
        }
    }


    this->isHover = this->isInside();

    if (e->type == SDL_MOUSEBUTTONDOWN && this->isHover) {
        this->isPressed = true;
    }

    if (e->type == SDL_MOUSEBUTTONUP && this->isPressed) {
        this->isPressed = false;
        if (this->isHover) {
            std::cout << "button clicked - start game.." << std::endl;
            Mix_VolumeMusic(30); // values range [0,128]
            bestUser = GameFunction(this->windowManager, &bestUser); // each game must implement return same struct
            std::cout << "User: " << bestUser.name.c_str() << std::endl;
            std::cout << "Score: " << bestUser.score.c_str() << std::endl;
            Mix_VolumeMusic(128);

            if (bestUser.score != "-1") {
                transform(bestUser.game.begin(), bestUser.game.end(), bestUser.game.begin(), ::tolower);
                for (int i = 0; i < bestUsers.size(); i++) {
                    if (bestUsers[i].game == bestUser.game) {
                        std::cout << "game name: " << bestUser.game.c_str() << std::endl;
                        bestUsers[i] = bestUser;
                    }
                }
                csvWrite("records.csv", bestUsers);
            }

            std::cout << "game ended.." << std::endl;
            windowManager->loadMedia("images/pixel_image.jpg");
        }
    }
}