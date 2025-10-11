//
// Created by Michael Berner on 11/10/2025.
//
#include "CsvHandler.h"
#include <sstream>
#include <iostream>
#include <fstream>


bool csvWrite(std::string file, std::vector<UserData> users) {
    std::ofstream f(file.c_str());
    if (!f.is_open()) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return false;
    }

    // Writing header and some data
    for (int i = 0; i < users.size(); i++) {
        f << users[i].game + "," + users[i].name + "," + users[i].score << std::endl;
    }
    f.close();

    return true;
}

std::vector<UserData> csvRead(std::string file) {
    UserData user;
    std::vector<UserData> data;
    std::ifstream f(file.c_str());
    if (!f.is_open()) {
        std::cerr << "Error opening file for reading!" << std::endl;
        user.game = "default game";
        user.name = "default name";
        user.score = "-1";
        return {user};
    }

    std::string line;


    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

            user.game = row[0];
            user.name = row[1];
            user.score = row[2];
        data.push_back(user);
    }

    f.close();
    return data;

}

void csvInit(std::string file) {
    if (!std::filesystem::exists(file)) {
        std::cerr << "CSV file doesn't exist, generating new.." << std::endl;

        std::vector<UserData> data;
        UserData user;
        user.game = "Game"; user.name = "Name"; user.score = "Score";
        data.push_back(user);

        for (const auto& entry : std::filesystem::directory_iterator("./games")) {
            if (entry.is_directory()) {
                user.name = "default";
                user.score = "-1";
                user.game = entry.path().filename();
                data.push_back(user);
                std::cout << "Directory: " << entry.path().filename() << '\n';
            }

            csvWrite(file, data);
        }
    }
};