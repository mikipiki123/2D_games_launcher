//
// Created by Michael Berner on 04/10/2025.
//

#include "GameLogic.h"

Entity::Entity(WindowManager* window, ScreenCalc* screen_calc) {
    this->windowManager = window;
    this->screenCalc = screen_calc;
}

// move = draw + update matrix + update coordinates
bool Entity::move(int direction) {

    if (direction == KEY_PRESS_SURFACE_UP) {
        std::cout << "\nCurrent move: UP" << std::endl;
        std::cout << "Last move is " << this->lastMove << std::endl;
        this->prevCoord[1] = this->coord[1];
        this->prevCoord[0] = this->coord[0];
        this->coord[1]--;
        return true;
    } else if (direction == KEY_PRESS_SURFACE_DOWN) {
        std::cout << "\nCurrent move: DOWN" << std::endl;
        std::cout << "Last move is " << this->lastMove << std::endl;
        this->prevCoord[1] = this->coord[1];
        this->prevCoord[0] = this->coord[0];
        this->coord[1]++;
        return true;
    } else if (direction == KEY_PRESS_SURFACE_LEFT) {
        std::cout << "\nCurrent move: LEFT" << std::endl;
        std::cout << "Last move is " << this->lastMove << std::endl;
        this->prevCoord[0] = this->coord[0];
        this->prevCoord[1] = this->coord[1];
        this->coord[0]--;
        return true;
    } else if (direction == KEY_PRESS_SURFACE_RIGHT) {
        std::cout << "\nCurrent move: RIGHT" << std::endl;
        std::cout << "Last move is " << this->lastMove << std::endl;
        this->prevCoord[0] = this->coord[0];
        this->prevCoord[1] = this->coord[1];
        this->coord[0]++;
        return true;
    }
    return false;
}


void Head::draw(float x, float y) { // {x,y} are in logic pixels

    SDL_FRect fillRect = { x, y + screenCalc->sizeOfCell/5, screenCalc->sizeOfCell, 3*screenCalc->sizeOfCell/5 };
    SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect );

    SDL_FRect fillRect2 = { x  + screenCalc->sizeOfCell/5, y, 3*screenCalc->sizeOfCell/5, screenCalc->sizeOfCell};
    SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect2 );

    SDL_FRect fillRect3 = { x + screenCalc->sizeOfCell/5, y + screenCalc->sizeOfCell/5, 3 * screenCalc->sizeOfCell/5, 3 *screenCalc->sizeOfCell/5};
    SDL_SetRenderDrawColor( windowManager->renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect3 );

    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 2; j++) {
    //         SDL_FRect fillRect3 = { (x + j * (4*screenCalc->sizeOfCell/5)),( y + i * (4*screenCalc->sizeOfCell/5)), screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/5};
    //         SDL_SetRenderDrawColor( windowManager->renderer, 0x00, 0x00, 0x00, 0xFF );
    //         SDL_RenderFillRectF( windowManager->renderer, &fillRect3 );
    //     }
    // }
}

void Body::draw(float x, float y) {


    SDL_FRect fillRect = { x, y + screenCalc->sizeOfCell/5, screenCalc->sizeOfCell, 3*screenCalc->sizeOfCell/5 };
    SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect );

    SDL_FRect fillRect2 = { x  + screenCalc->sizeOfCell/5, y, 3*screenCalc->sizeOfCell/5, screenCalc->sizeOfCell};
    SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect2 );

    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 2; j++) {
    //         SDL_FRect fillRect3 = { (x + j * (4*screenCalc->sizeOfCell/5)),( y + i * (4*screenCalc->sizeOfCell/5)), screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/5};
    //         SDL_SetRenderDrawColor( windowManager->renderer, 0x00, 0x00, 0x00, 0xFF );
    //         SDL_RenderFillRectF( windowManager->renderer, &fillRect3 );
    //     }
    // }
}

void Apple::draw(float x, float y) {

    SDL_FRect fillRect = { x, y + screenCalc->sizeOfCell/5, screenCalc->sizeOfCell, 3*screenCalc->sizeOfCell/5 };
    SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect );

    SDL_FRect fillRect2 = { x  + screenCalc->sizeOfCell/5, y, 3*screenCalc->sizeOfCell/5, screenCalc->sizeOfCell};
    SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect2 );


    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 2; j++) {
    //         SDL_FRect fillRect3 = { (x + j * (4*screenCalc->sizeOfCell/5)),( y + i * (4*screenCalc->sizeOfCell/5)), screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/5};
    //         SDL_SetRenderDrawColor( windowManager->renderer, 0, 0, 0, 0xFF);
    //         // SDL_RenderCopy(windowManager->renderer, windowManager->backGround, nullptr, nullptr);
    //         SDL_RenderFillRectF( windowManager->renderer, &fillRect3 );
    //     }
    // }


    SDL_FRect fillRect3 = { x + 2 * screenCalc->sizeOfCell/5, y - screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/2 };
    SDL_SetRenderDrawColor( windowManager->renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect3 );

    SDL_FRect fillRect4 = { x + screenCalc->sizeOfCell/5, y - 2 * screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/5, screenCalc->sizeOfCell/5 };
    SDL_SetRenderDrawColor( windowManager->renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderFillRectF( windowManager->renderer, &fillRect4 );


}


Entity::~Entity() = default;
Head::~Head() = default;
Body::~Body() = default;
Apple::~Apple() = default;
