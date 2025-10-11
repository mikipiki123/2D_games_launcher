//
// Created by Michael Berner on 04/10/2025.
//

#include "GameLogic.h"
// #include <cstdlib>
// #include <ctime>

GameLogic::GameLogic(WindowManager *window) {

    createBorders();

    this->windowManager = window;

    screenCalc.minSizeScreen = this->windowManager->screen_width < this->windowManager->screen_height ? this->windowManager->screen_width : this->windowManager->screen_height;
    screenCalc.maxSizeScreen = this->windowManager->screen_width > this->windowManager->screen_height ? this->windowManager->screen_width : this->windowManager->screen_height;
    screenCalc.center[0] = (float) this->windowManager->screen_width / 2; //{x,y}
    screenCalc.center[1] =  (float) this->windowManager->screen_height / 2;
    screenCalc.sizeOfCell = (screenCalc.minSizeScreen - SHIFT_ON_EACH_SIDE * 2) / GAME_DIM;

    this->entities.push_back(new Head(window, &screenCalc));
    this->entities[0]->lastMove = KEY_PRESS_SURFACE_RIGHT;
    grow();
    this->entities[1]->coord[0] = this->entities[0]->coord[0] - 1;

    this->apple = new Apple(window, &screenCalc);
    genApple();
}

void GameLogic::drawGame() {

    drawBorders();
    drawGameObjects();

}

void GameLogic::createBorders() {

    for (int i = 0; i < GAME_DIM; i++) {
        if (i == 0 || i == GAME_DIM - 1) {
            for (int j = 0; j < GAME_DIM; j++) {
                matrix[i][j] = true;
            }
        } else {
            matrix[i][0] = true;
            matrix[i][GAME_DIM - 1] = true;
        }
    }
}

void GameLogic::drawBorders() {

    for (int i = 0; i < GAME_DIM; i++) {
        for (int j = 0; j < GAME_DIM; j++) {
            if (matrix[i][j] && i == 0 || matrix[i][j] && j == 0 || matrix[i][j] && i == GAME_DIM - 1 || matrix[i][j] && j == GAME_DIM - 1) {
                float leftUpperCorner[2] = {this->screenCalc.center[0] - ((GAME_DIM/2 - j) * this->screenCalc.sizeOfCell), this->screenCalc.center[1] - ((GAME_DIM/2 - i) * this->screenCalc.sizeOfCell) }; // {x,y}

                SDL_FRect fillRect = { leftUpperCorner[0], leftUpperCorner[1], this->screenCalc.sizeOfCell, this->screenCalc.sizeOfCell };
                SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderFillRectF( windowManager->renderer, &fillRect );
            }
        }
    }
}

void GameLogic::drawGameObjects() {
    /*relative to center
    * transform to upper-left coord system on each dim, adding coord number (multiplied by cell size in logic pixels) */
    float x;
    float y;
    for (int i = 0; i < entities.size(); i++) {
        x = this->screenCalc.center[0] -1 * (float)(GAME_DIM/2 - this->entities[i]->coord[0]) * screenCalc.sizeOfCell;
        y = this->screenCalc.center[1] -1 * (float)(GAME_DIM/2 - this->entities[i]->coord[1]) * screenCalc.sizeOfCell;
        this->entities[i]->draw(x,y);
    }

    x = this->screenCalc.center[0] -1 * (float)(GAME_DIM/2 - this->apple->coord[0]) * screenCalc.sizeOfCell;
    y = this->screenCalc.center[1] -1 * (float)(GAME_DIM/2 - this->apple->coord[1]) * screenCalc.sizeOfCell;

    this->apple->draw(x,y);
}


void GameLogic::grow() {
    Body* body = new Body(this->windowManager, &screenCalc);
    body->coord[0] = this->entities.back()->prevCoord[0];
    body->coord[1] = this->entities.back()->prevCoord[1];

    this->entities.push_back(body);
}

bool GameLogic::isCorner(int direction) {
    if (direction == KEY_PRESS_SURFACE_UP && this->matrix[(int)this->entities[0]->coord[1] - 1][(int)this->entities[0]->coord[0]]) {
        return true;
    }
    if (direction == KEY_PRESS_SURFACE_DOWN && this->matrix[(int)this->entities[0]->coord[1] + 1][(int)this->entities[0]->coord[0]]) {
        return true;
    }
    if (direction == KEY_PRESS_SURFACE_LEFT && this->matrix[(int)this->entities[0]->coord[1]][(int)this->entities[0]->coord[0] - 1]) {
        return true;
    }
    if (direction == KEY_PRESS_SURFACE_RIGHT && this->matrix[(int)this->entities[0]->coord[1]][(int)this->entities[0]->coord[0] + 1]) {
        return true;
    }
    return false;
}

bool GameLogic::isEaten(int direction) {
    if (direction == KEY_PRESS_SURFACE_UP && this->entities[0]->coord[0] == this->apple->coord[0] && this->entities[0]->coord[1] - 1 == this->apple->coord[1]) {
        return true;
    }
    if (direction == KEY_PRESS_SURFACE_DOWN && this->entities[0]->coord[0] == this->apple->coord[0] && this->entities[0]->coord[1] + 1 == this->apple->coord[1]) {
        return true;
    }
    if (direction == KEY_PRESS_SURFACE_LEFT && this->entities[0]->coord[0] - 1 == this->apple->coord[0] && this->entities[0]->coord[1] == this->apple->coord[1]) {
        return true;
    }
    if (direction == KEY_PRESS_SURFACE_RIGHT && this->entities[0]->coord[0] + 1 == this->apple->coord[0] && this->entities[0]->coord[1] == this->apple->coord[1]) {
        return true;
    }
    return false;
}


void GameLogic::updateMatrixAndSkeleton() {


    for (int i = 0; i < this->entities.size() ; i++) {
        if (i != 0) {
            //before moving
            this->entities[i]->prevCoord[0] = this->entities[i]->coord[0];
            this->entities[i]->prevCoord[1] = this->entities[i]->coord[1];

            //during moving
            this->entities[i]->coord[0] = this->entities[i - 1]->prevCoord[0];
            this->entities[i]->coord[1] = this->entities[i - 1]->prevCoord[1];
        }
        // std::cout << "x " << i << ": " << this->entities[i]->coord[0] << std::endl;
        // std::cout << "y " << i << ": " << this->entities[i]->coord[1] << std::endl;
        // std::cout << "x " << i << " previous: " << this->entities[i]->prevCoord[0] << std::endl;
        // std::cout << "y " << i << " previous: " << this->entities[i]->prevCoord[1] << std::endl;
        matrix[(int)this->entities[i]->coord[1]][(int)this->entities[i]->coord[0]] = true;
    }

    matrix[(int)this->entities.back()->prevCoord[1]][(int)this->entities.back()->prevCoord[0]] = false;
}

void GameLogic::genApple() {

    bool free = false;

    srand(time(0));
    do {
        for (int i = 0; i < 2; i++) {//generate each coord on free cells
            this->apple->coord[i] = (float)((rand() % (GAME_DIM - 2)) + 1);
        }
        if (!matrix[(int)this->apple->coord[1]][(int)this->apple->coord[0]]) {
            free = true;
        }
    }while (!free);
}

GameLogic::~GameLogic() {
    std::cout << "game ended - destructor called" << std::endl;
    for (int i = 0; i < this->entities.size() ; i++) {
        delete this->entities[i];
    }
    std::cout << "Snake deleted" << std::endl;

    delete this->apple;
    std::cout << "Apple deleted" << std::endl;
}


