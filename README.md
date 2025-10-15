# 2D Games platform
A simple 2D games platform based on the SDL2 multimedia library.
It’s designed to easily append new games that are developed separately.
The platform already includes button, CSV, and window handlers for managing games and storing their data.<br>

## Install instructions
#### Build
Building requires CMake and a package manager (such as Homebrew) to be installed.

#### Dependencies 
Use Homebrew to install SDL2 and the following extensions:
<pre>
  brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
</pre> <br>

Then build from the project directory:
<pre>
  cd games/snake 
  cmake .
  make

  cd ../tetris
  cmake .
  make

  cd ../..
  cmake .
  make
</pre>

## Adding new games
You can look at the structures of existing games and append new ones, the basic rules:<br>
1. Each game must be compiled as a shared library.
2. Each game receives pointers to WindowManager* and UserData*, and returns a UserData object.
3. If a game exits illegally, the score will be set to -1 and will not be stored in records.csv.
4. If no new record is achieved, the score will also be -1 upon return.



