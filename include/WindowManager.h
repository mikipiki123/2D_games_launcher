//
// Created by Michael Berner on 04/10/2025.
//

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_mixer.h>
#include <vector>
#include <array>


#ifndef SNAKE_WINDOWMANAGER_H
#define SNAKE_WINDOWMANAGER_H

//Screen dimension constants
// const int SCREEN_WIDTH = 640;
// const int SCREEN_HEIGHT = 480;

typedef struct {
    std::string game;
    std::string name;
    std::string score;
}UserData;


class WindowManager {
    public:

    float screen_width;
    float screen_height;

    //The window we'll be rendering to
    SDL_Window* Window = NULL;

    //The window renderer
    SDL_Renderer* renderer = NULL;

    //Current displayed text
    std::array<SDL_Texture*,10> text; //{ "score", score value, "Best score", best score value + name

    //start photo BMP/png
    SDL_Texture* backGround = NULL;

    TTF_Font* font = NULL;

    Mix_Music* music = NULL;

    Mix_Chunk* soundEffect = NULL;

    WindowManager(float width, float height);

    //Starts up SDL and creates window
    bool init();

    //Loads media
    bool loadMedia(std::string path);

    //Loads font
    bool loadFont(std::string score,  int textureIndex);

    //loads audio
    bool loadAudio(std::string path);

    //Frees media and shuts down SDL
    void close();

    private:
    //Loads individual image as texture
    SDL_Texture* loadTexture( std::string path );

};


#endif //SNAKE_WINDOWMANAGER_H