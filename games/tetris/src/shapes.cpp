//
// Created by Michael Berner on 12/10/2025.
//

#include "gameLogic.h"
#include <map>

// std::map<Color, std::array<int, 3>> colorMap = {
//     { RED,     {255, 0, 0} },
//     { GREEN,   {0, 255, 0} },
//     { BLUE,    {0, 0, 255} },
//     { YELLOW,  {255, 255, 0} },
//     { CYAN,    {0, 255, 255} },
//     { MAGENTA, {255, 0, 255} },
//     { WHITE,   {255, 255, 255} },
//     { BLACK,   {0, 0, 0} }
// };

Shapes::Shapes(WindowManager *window, ScreenCalc *screen_calc, int rawColor) {
    this->windowManager = window;
    this->screenCalc = screen_calc;

    std::array<int,3> tmp = parseColor(rawColor); //{ r , g , b }
    this->color.r = tmp[0];
    this->color.g = tmp[1];
    this->color.b = tmp[2];
}

std::array<int, 3> parseColor(int colorValue) { // 1 - 8
    std::array<int,3> color{};
    switch (colorValue - 1) {
        case RED:
            color = {255, 0, 0};
            return color;
        case GREEN:
            color = {0, 255, 0};
            return color;
        case BLUE:
            color = {0, 0, 255};
            return color;
        case YELLOW:
            color = {255, 255, 0};
            return color;
        case CYAN:
            color = {0, 255, 255};
            return color;
        case MAGENTA:
            color = {255, 0, 255};
            return color;
        case WHITE:
            color = {255, 255, 255};
            return color;
        case BLACK:
            color = {0, 0, 0};
            return color;
    }
}


void Matrix3x3::move(int direction) {//todo validate corners
    switch (direction) {
        case 0:
            this->shapeMatrixCoords[1]++;
            break;
        case 1:
            this->shapeMatrixCoords[0]--;
            break;
        case 2:
            this->shapeMatrixCoords[0]++;
            break;
        default:
            std::cerr << "Unrecognized direction: move()" << std::endl;
    }
}

void Matrix3x3::rotate() { //3x3 CW
    int n = this->shapeMatrix.size();
    std::cout << "matrix size: " << n << std::endl;

    // Step 1: Transpose the matrix
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            std::swap(this->shapeMatrix[i][j], this->shapeMatrix[j][i]);
        }
    }

    // Step 2: Reverse each row
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            std::swap(this->shapeMatrix[i][j], this->shapeMatrix[i][n - j - 1]);
        }
    }
}

void Matrix3x3::draw(float x, float y) { //colors { r, g , b }
    int matSize = this->shapeMatrix.size();
    for (int i = 0; i < matSize; ++i) {
        for (int j = 0; j < matSize; ++j) {
            if (this->shapeMatrix[i][j]) {
                SDL_FRect fillRect = {x + j*screenCalc->sizeOfCell, y + i*screenCalc->sizeOfCell, screenCalc->sizeOfCell, screenCalc->sizeOfCell};
                SDL_SetRenderDrawColor( windowManager->renderer, this->color.r, this->color.g, this->color.b, 0xFF );
                SDL_RenderFillRectF( windowManager->renderer, &fillRect );
                SDL_SetRenderDrawColor( windowManager->renderer, 0, 0, 0, 0xFF );
                SDL_RenderDrawRectF(windowManager->renderer, &fillRect);
            }
        }
    }
}


void Matrix4x4::move(int direction) {
    switch (direction) {
        case 0:
            this->shapeMatrixCoords[1]++;
            break;
        case 1:
            this->shapeMatrixCoords[0]--;
            break;
        case 2:
            this->shapeMatrixCoords[0]++;
            break;
        default:
            std::cerr << "Unrecognized direction: move()" << std::endl;
    }
}

void Matrix4x4::rotate() {
    int n = this->shapeMatrix.size();
    std::cout << "matrix size: " << n << std::endl;

    // Step 1: Transpose the matrix
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            std::swap(this->shapeMatrix[i][j], this->shapeMatrix[j][i]);
        }
    }

    // Step 2: Reverse each row
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            std::swap(this->shapeMatrix[i][j], this->shapeMatrix[i][n - j - 1]);
        }
    }
}

void Matrix4x4::draw(float x, float y) {
    int matSize = this->shapeMatrix.size();
    for (int i = 0; i < matSize; ++i) {
        for (int j = 0; j < matSize; ++j) {
            if (this->shapeMatrix[i][j]) {
                SDL_FRect fillRect = {x + j*screenCalc->sizeOfCell, y + i*screenCalc->sizeOfCell, screenCalc->sizeOfCell, screenCalc->sizeOfCell};
                SDL_SetRenderDrawColor( windowManager->renderer, this->color.r, this->color.g, this->color.b, 0xFF );
                SDL_RenderFillRectF( windowManager->renderer, &fillRect );
                SDL_SetRenderDrawColor( windowManager->renderer, 0, 0, 0, 0xFF );
                SDL_RenderDrawRectF(windowManager->renderer, &fillRect);
            }
        }
    }
}


