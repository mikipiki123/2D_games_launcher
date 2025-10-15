# 2D game's platform
A simple 2D games platform based on SDL2 multimedia library, It's designed to append easily new games that developed separately.
The platform already has button,CSV,window handlers for managing games and store their data.

## Install instructions
#### Build
Building requires Cmake and package manager (like Homebrew) to be installed.

#### Dependencies 
Use Homebrew to install SDL2 with the next extention:
<pre>
  brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
</pre>

From directory path:
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

## Append games
You can look at the structures of current games appended, the basic rules:<br>
1. Each game compiled as shared library.
2. Each game receive WindowManager* and UserData*, and returns UserData.
3. If game exits illigally the score will be -1, and will not store at records.csv
4. If record didn't breaked, the score also be -1 at return.



