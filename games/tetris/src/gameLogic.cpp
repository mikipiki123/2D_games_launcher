//
// Created by Michael Berner on 12/10/2025.
//

#include "gameLogic.h"
#include <random>

GameLogic::GameLogic(WindowManager *window) {
    this-> windowManager = window;

    createBorders();

    if (this->windowManager->screen_width < this->windowManager->screen_height) { //vertical screen
        this->screenCalc.minSizeScreen = this->windowManager->screen_width;
        this->screenCalc.maxSizeScreen = this->windowManager->screen_height;

        this->screenCalc.sizeOfCell = (screenCalc.minSizeScreen - SHIFT_ON_EACH_SIDE * 2) / GAME_DIM_WIDTH;

    } else { //horizontal screen
        this->screenCalc.minSizeScreen = this->windowManager->screen_height;
        this->screenCalc.maxSizeScreen = this->windowManager->screen_width;

        this->screenCalc.sizeOfCell = (screenCalc.minSizeScreen - SHIFT_ON_EACH_SIDE * 2) / GAME_DIM_HEIGHT;

    }

    screenCalc.center[0] = (float) this->windowManager->screen_width / 2; //{x,y}
    screenCalc.center[1] =  (float) this->windowManager->screen_height / 2;

    // currentShape.push_back(new ShapeJ(this->windowManager, &this->screenCalc, 2));
    currentShape.push_back(new ShapeZ(this->windowManager, &this->screenCalc, 5));
    currentShape[0]->shapeMatrixCoords[0] = 5;
    currentShape[0]->shapeMatrixCoords[1] = 14;

}

void GameLogic::createBorders() {
    for (int i = 0; i < GAME_DIM_HEIGHT; i++) {
        if (i == GAME_DIM_HEIGHT - 1) {
            for (int j = 0; j < GAME_DIM_WIDTH; j++) { //under border
                gameMatrix[i][j] = true;
            }
        } else {
            gameMatrix[i][0] = true; //sides
            gameMatrix[i][GAME_DIM_WIDTH - 1] = true;
            for (int j = 1; j < GAME_DIM_WIDTH - 1; j++) { //internal
                gameMatrix[i][j] = savedMatrix[i][j];
                // gameMatrix[i][j] = false;
            }
        }
    }

    // for (int i = 0; i < GAME_DIM_HEIGHT; i++) {
    //     if (i == GAME_DIM_HEIGHT - 1) {
    //         for (int j = 0; j < GAME_DIM_WIDTH; j++) {
    //             gameMatrix[i][j] = true;
    //         }
    //     } else {
    //         gameMatrix[i][0] = true;
    //         gameMatrix[i][GAME_DIM_WIDTH - 1] = true;
    //     }
    // }
}

void GameLogic::drawBorders() {
    for (int i = 0; i < GAME_DIM_HEIGHT; i++) {
        for (int j = 0; j < GAME_DIM_WIDTH; j++) {

            float leftUpperCorner[2] = {this->screenCalc.center[0] - ((GAME_DIM_WIDTH/2 - j) * this->screenCalc.sizeOfCell), this->screenCalc.center[1] - ((GAME_DIM_HEIGHT/2 - i) * this->screenCalc.sizeOfCell) }; // {x,y}

            if (gameMatrix[i][j] && i == 0 || gameMatrix[i][j] && j == 0 || gameMatrix[i][j] && i == GAME_DIM_HEIGHT - 1 || gameMatrix[i][j] && j == GAME_DIM_WIDTH - 1) {

                SDL_FRect fillRect = { leftUpperCorner[0], leftUpperCorner[1], this->screenCalc.sizeOfCell, this->screenCalc.sizeOfCell };
                SDL_SetRenderDrawColor( windowManager->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderFillRectF( windowManager->renderer, &fillRect );
            } else {
                if (savedMatrix[i][j]) {
                    std::array<int,3> tmp = parseColor(savedMatrix[i][j]);

                    SDL_FRect fillRect = { leftUpperCorner[0], leftUpperCorner[1], this->screenCalc.sizeOfCell, this->screenCalc.sizeOfCell };
                    SDL_SetRenderDrawColor( windowManager->renderer, tmp[0],tmp[1] , tmp[2], 0xFF );
                    SDL_RenderFillRectF( windowManager->renderer, &fillRect );
                }
            }
        }
    }
}

void GameLogic::drawGameObjects() {
    /*relative to center
   * transform to upper-left coord system on each dim, adding coord number (multiplied by cell size in logic pixels) */
    float x;
    float y;

        x = this->screenCalc.center[0] -1 * (float)(GAME_DIM_WIDTH/2 - this->currentShape.back()->shapeMatrixCoords[0]) * screenCalc.sizeOfCell;
        y = this->screenCalc.center[1] -1 * (float)(GAME_DIM_HEIGHT/2 - this->currentShape.back()->shapeMatrixCoords[1]) * screenCalc.sizeOfCell;
        int color[3] = {255,255,255};
        this->currentShape.back()->draw(x,y);

}


void GameLogic::drawGame() {
    drawBorders();
    drawGameObjects();
}

void GameLogic::updateMatrix() {

    if (currentShape[0]->isLanded) { //save last position
        for (int i = 0; i < currentShape.back()->getMatrixSize(); i++) {
            for (int j = 0; j < currentShape.back()->getMatrixSize(); j++) {
                if (currentShape.back()->getMatrixValue(i,j) && currentShape.back()->shapeMatrixCoords[1] + i < GAME_DIM_HEIGHT) {
                    this->savedMatrix[currentShape.back()->shapeMatrixCoords[1] + i][currentShape.back()->shapeMatrixCoords[0] + j] = currentShape.back()->getMatrixValue(i,j);
                }
            }
        }
    }

    createBorders();

    if (!currentShape.back()->isLanded) {
        for (int i = 0; i < currentShape.back()->getMatrixSize(); i++) {
            for (int j = 0; j < currentShape.back()->getMatrixSize(); j++) {
                if (currentShape.back()->getMatrixValue(i,j)) {
                    this->gameMatrix[currentShape.back()->shapeMatrixCoords[1] + i][currentShape.back()->shapeMatrixCoords[0] + j] = currentShape.back()->getMatrixValue(i,j);
                }
            }
        }
    }


}

void GameLogic::checkAndMove(int direction) {

    int n = currentShape.back()->getMatrixSize();

    bool canMove = true;
    const std::array<int,2> tmp = {0,0};

    if (direction == KEY_PRESS_SURFACE_LEFT) {
        std::vector<std::array<int,2>> leftCorner(n); //{ i , j }
        /*
         1) find the closest shape's point to move from left - save their coord
         2) check if the next (left) coordinate is empty
         */
        for (int i = 0 ; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (leftCorner[i] == tmp && this->currentShape.back()->getMatrixValue(i,j)) {
                    leftCorner[i] = {currentShape.back()->shapeMatrixCoords[1] + i,currentShape.back()->shapeMatrixCoords[0] + j};
                }
            }
        }
        for (int i = 0 ; i < n; i++) { //if at least one of borders cant move
            if (this->gameMatrix[leftCorner[i][0]][leftCorner[i][1] - 1] && leftCorner[i] != tmp) {
                canMove = false;
            }
        }
        if (canMove) {
            currentShape.back()->move(direction);
        }
    } else if (direction == KEY_PRESS_SURFACE_RIGHT) {
        std::vector<std::array<int,2>> rightCorner(n); //{ i , j }
        /*
         1) find the closest shape's point to move from left - save their coord
         2) check if the next (left) coordinate is empty
         */
        for (int i = n - 1 ; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (rightCorner[i] == tmp && this->currentShape.back()->getMatrixValue(i,j)) {
                    rightCorner[i] = {currentShape.back()->shapeMatrixCoords[1] + i,currentShape.back()->shapeMatrixCoords[0] + j};
                }
            }
        }
        for (int i = 0 ; i < n; i++) { //if at least one of borders cant move
            if (this->gameMatrix[rightCorner[i][0]][rightCorner[i][1] + 1] && rightCorner[i] != tmp) {
                canMove = false;
            }
        }
        if (canMove) {
            currentShape.back()->move(direction);
        }

    } else if (direction == KEY_PRESS_SURFACE_DOWN) {
        std::vector<std::array<int,2>> underCorner(n); //{ i , j }
        /*
         1) find the closest shape's point to move from left - save their coord
         2) check if the next (left) coordinate is empty
         */
        for (int i = n - 1 ; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (underCorner[j] == tmp && this->currentShape.back()->getMatrixValue(i,j)) {
                    underCorner[j] = {currentShape.back()->shapeMatrixCoords[1] + i,currentShape.back()->shapeMatrixCoords[0] + j};
                }
            }
        }
        for (int i = 0 ; i < n; i++) { //if at least one of borders cant move
            if (this->gameMatrix[underCorner[i][0] + 1][underCorner[i][1]] && underCorner[i] != tmp) {
                canMove = false;
            }
        }
        if (canMove) {
            this->currentShape.back()->move(direction);
        } else {
            this->currentShape.back()->isLanded = true;
            Mix_PlayChannel(-1, windowManager->soundEffect, 0);
            std::cout << "Shape landed" << std::endl;
        }
    }
}

void GameLogic::checkAndRotate() {
    int n = currentShape.back()->getMatrixSize();

    bool canRotate = true;

    std::vector<std::vector<int>> tempMatrix(n,std::vector<int>(n));

    for (int i = 0 ; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tempMatrix[i][j] = currentShape.back()->getMatrixValue(i,j);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            std::cout << tempMatrix[i][j] << ' ';
        std::cout << '\n';
    }
    std::cout << "\n\n" << std::endl;

    //rotate start
    // Step 1: Transpose the matrix
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            std::swap(tempMatrix[i][j], tempMatrix[j][i]);
        }
    }

    // Step 2: Reverse each row
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            std::swap(tempMatrix[i][j], tempMatrix[i][n - j - 1]);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            std::cout << tempMatrix[i][j] << ' ';
        std::cout << '\n';
    }
    std::cout << "\n\n" << std::endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tempMatrix[i][j] && this->gameMatrix[this->currentShape.back()->shapeMatrixCoords[1] + i][this->currentShape.back()->shapeMatrixCoords[0] + j] && !currentShape.back()->getMatrixValue(i,j)) {
                canRotate = false;
            }
        }
    }

    if (canRotate) {
        currentShape.back()->rotate();
    }
}

bool GameLogic::checkAndGenShape() {

    if (this->currentShape.back()->isLanded && this->currentShape.back()->shapeMatrixCoords[1] <= 1) { //end row
        std::cout << "shape y: " << currentShape.back()->shapeMatrixCoords[1] << std::endl;
        return false;
    }

    destroyRows( GAME_DIM_HEIGHT - 2 , 1);

    if (currentShape.back()->isLanded) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> shapesRange(0, 6); // 7 types
        static std::uniform_int_distribution<int> randomColor(1, 8); // 8 types

        int shape = shapesRange(gen);
        std::cout << "shape: " << shape << std::endl;
        int color = randomColor(gen);
        std::cout << "color: " << color << std::endl;

        switch (shape) {
            case 0:
                this->currentShape.push_back(new ShapeJ(this->windowManager,&this->screenCalc, color));
                break;
            case 1:
                this->currentShape.push_back(new ShapeZ(this->windowManager,&this->screenCalc, color));
                break;
            case 2:
                this->currentShape.push_back(new ShapeL(this->windowManager,&this->screenCalc, color));
                break;
            case 3:
                this->currentShape.push_back(new ShapeS(this->windowManager,&this->screenCalc, color));
                break;
            case 4:
                this->currentShape.push_back(new ShapeT(this->windowManager,&this->screenCalc, color));
                break;
            case 5:
                this->currentShape.push_back(new ShapeI(this->windowManager,&this->screenCalc, color));
                break;
            case 6:
                this->currentShape.push_back(new ShapeO(this->windowManager,&this->screenCalc, color));
                break;
            default:
                std::cerr << "shape error" << std::endl;
        }
    }
    return true;
}

void GameLogic::destroyRows(int buttomLimit, int upperLimit) {

    if (buttomLimit == upperLimit) {
        return;
    }

    if (this->currentShape.back()->isLanded) {
        for (int i = buttomLimit; i > upperLimit; i--) {
            bool emptyRow = true;

            bool rowIsFull = true;
            for (int j = 1 ; j < GAME_DIM_WIDTH - 1; j++) {
                if (!this->savedMatrix[i][j]) {
                    rowIsFull = false;
                } else {
                    emptyRow = false;
                }
            }

            if (emptyRow) {
                return;
            }

            if (rowIsFull) {
                for (int j = 1 ; j < GAME_DIM_WIDTH - 1; j++) {
                    this->savedMatrix[i][j] = false;
                }
                int minFall = GAME_DIM_HEIGHT;
                for (int inner = i - 1; inner >= 1; inner--) {//start falling
                    if (inner == i - 1) {
                        for (int jinner = 1 ; jinner < GAME_DIM_WIDTH - 1; jinner++) {
                            int k = 1;
                            while (savedMatrix[inner + k][jinner] == 0 && inner + k != GAME_DIM_HEIGHT - 1) {
                                k++;
                            }
                            k--;
                            minFall = minFall > k ? k : minFall;
                        }
                    }
                    for (int jinner = 1 ; jinner < GAME_DIM_WIDTH - 1; jinner++) {
                        savedMatrix[inner + minFall][jinner] = savedMatrix[inner][jinner];
                        savedMatrix[inner][jinner] = 0;
                    }
                }
                this->score++;
                if (this->score < 10) {
                    this->speed -= 40;
                }
                destroyRows(GAME_DIM_HEIGHT - 2, i - 1);
            }
        }
    }
}
