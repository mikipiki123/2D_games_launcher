//
// Created by Michael Berner on 08/10/2025.
//

#include <iostream>
#include "WindowManager.h"

#ifndef SNAKE_SNAKEMAIN_H
#define SNAKE_SNAKEMAIN_H

// #ifdef __cplusplus
// extern "C" {
// #endif
//     Start the main loop


typedef struct {
    std::string game;
    std::string name;
    std::string score;
}UserData;

UserData snakePlay(WindowManager* window, UserData* bestUser);

// #ifdef __cplusplus
// }
// #endif
#endif //SNAKE_SNAKEMAIN_H