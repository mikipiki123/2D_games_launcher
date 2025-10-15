//
// Created by Michael Berner on 11/10/2025.
//

#include "SnakeMain.h"
#include <string>

#ifndef GAMEMENU_CSVHANDLER_H
#define GAMEMENU_CSVHANDLER_H

enum GameLine {
    CSV_HEADER,
    CSV_SNAKE,
    CSV_TETRIS
};

bool csvWrite(std::string file, std::vector<UserData> users);
std::vector<UserData> csvRead(std::string file); //all data

std::vector<UserData> overWriteLine(std::vector<UserData> &data, int lineNum, UserData &user);

void csvInit(std::string file);

#endif //GAMEMENU_CSVHANDLER_H