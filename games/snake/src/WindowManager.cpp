//
// Created by Michael Berner on 04/10/2025.
//

#include "../include/WindowManager.h"
#include <sstream>
#include <algorithm>

WindowManager::WindowManager(float width, float height) {
    this->screen_width = width;
    this->screen_height = height;
}


bool WindowManager::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        this->Window = SDL_CreateWindow( "Snake - mikipiki", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screen_width, this->screen_height, SDL_WINDOW_SHOWN);
        if( this->Window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            ///Create renderer for window
            renderer = SDL_CreateRenderer( this->Window, -1, SDL_RENDERER_ACCELERATED );
            if( renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
            }


        }

    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError() << std::endl;
        success = false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! " << Mix_GetError() << std::endl;
        success = false;
    }

    TTF_Init();
    this->font = TTF_OpenFont("fonts/ARIALBI.TTF", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        success = false;
    }

    return success;
}

bool WindowManager::loadMedia(std::string path) {

    //Loading success flag
    bool success = true;

    //Load BMP texture
    this->backGround = loadTexture( path.c_str() );
    if( this->backGround == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }

    return success;
}

bool WindowManager::loadFont(std::string score, int textureIndex) {
    bool success = true;

    SDL_Color white = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, score.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (texture == NULL) {
        std::cout << "Failed to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }

    this->text[textureIndex] = texture;
    SDL_FreeSurface(textSurface);

    return success;
}

bool WindowManager::loadAudio(std::string path) {

    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;

    while (getline(ss, part, '.')) {
        // split by '.'
        parts.push_back(part);
    }

    transform(parts.back().begin(), parts.back().end(), parts.back().begin(), ::tolower);

    std::cout << "Audio format: " << parts.back() << std::endl;

    // Load music (MP3, OGG, etc.)
    if (parts.back() == "mp3") {
        Mix_Music* musicEffect = Mix_LoadMUS(path.c_str());
        if (!musicEffect) {
            std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
            return false;
        }
        this->music = musicEffect;
    } else if (parts.back() == "wav") {
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        if (!sound) {
            std::cerr << "Failed to load wav: " << Mix_GetError() << std::endl;
            return false;
        }
        this->soundEffect = sound;
    }

    return true;
}


SDL_Texture* WindowManager::loadTexture( std::string path ) {

    //The final texture
    SDL_Texture* newTexture = NULL;

    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;

    while (getline(ss, part, '.')) {
        // split by '.'
        parts.push_back(part);
    }

    transform(parts.back().begin(), parts.back().end(), parts.back().begin(), ::tolower);

    // std::cout << "image format: " << parts.back() << std::endl;

    SDL_Surface* loadedSurface;

    if (parts.back() == "png" || parts.back() == "jpg") {
        loadedSurface = IMG_Load( path.c_str() );
    } else if (parts.back() == "bmp") {
        loadedSurface = SDL_LoadBMP( path.c_str() );
    }

    //Load image at specified path
    // SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return NULL;
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( this->renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

void WindowManager::close() {

    //Free loaded image
    for (int i = 0; i < text.size(); i++) {
        SDL_DestroyTexture( this->text[i] );
        this->text[i] = NULL;
    }
    TTF_CloseFont(this->font);
    this->font = NULL;

    //Destroy window
    SDL_DestroyRenderer( this->renderer );
    SDL_DestroyWindow( this->Window );
    this->Window = NULL;
    this->renderer = NULL;

    Mix_FreeMusic(this->music);
    Mix_CloseAudio();

    //Quit SDL subsystems
    TTF_Quit();
    SDL_Quit();
}
