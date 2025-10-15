//
// Created by Michael Berner on 08/10/2025.
//


#ifndef GAMEMENU_MOUSEHANDLER_H
#define GAMEMENU_MOUSEHANDLER_H

#include "WindowManager.h"

class ButtonHandler {



    public:

    WindowManager* windowManager;
    SDL_Rect rect; //{ x , y } center of button
    SDL_Color normalColor;
    SDL_Color hoverColor;
    SDL_Color clickedColor;

    bool isHover = false;
    bool isPressed = false;

    std::string buttonText;

    ButtonHandler(WindowManager* window, SDL_Rect rect, SDL_Color normalColor, SDL_Color hoverColor, SDL_Color clickedColor);

    bool isInside();

    void buttonRender();

    void buttonStatus(SDL_Event* e, UserData (*GameFunction)(WindowManager*, UserData*));

};

#endif //GAMEMENU_MOUSEHANDLER_H