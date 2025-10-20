//
// Created by Michael Berner on 12/10/2025.
//


#ifndef TETRIS_GAMELOGIC_H
#define TETRIS_GAMELOGIC_H

#include "../../../include/WindowManager.h"

#define GAME_DIM_WIDTH 12 //including borders
#define GAME_DIM_HEIGHT 22
#define SHIFT_ON_EACH_SIDE 10
#define SHAPE_DIM_3x3 3
#define SHAPE_DIM_4x4 4

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
};

enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
    BLACK
};

typedef struct {
    float minSizeScreen;
    float maxSizeScreen;
    float center[2];
    float sizeOfCell;
} ScreenCalc;

std::array<int, 3> parseColor(int colorValue);

class Shapes {
    public:
    Shapes(WindowManager* window, ScreenCalc* screen_calc, int rawColor);
    virtual void draw(float x,float y) = 0;

    virtual void move(int direction) = 0; // down,left,right
    virtual void rotate() = 0; //space = rotate 90 degrees CW

    /*each shape represented by matrix 3x3 or 4x4, implemented on each shape
    each index as coordinates of the original matrix represented by { x , y }*/

    virtual int getMatrixSize() = 0;
    virtual int getMatrixValue(int i, int j) = 0;
    virtual void changeMatrixValue(int i, int j, int value) = 0;

    int shapeMatrixCoords[2] = {5,0}; // { x , y }

    bool isLanded = false;


    virtual ~Shapes() = default;

    SDL_Color color{}; //colors 0-7

    protected:
    WindowManager* windowManager;
    ScreenCalc* screenCalc;
};

class Matrix3x3 :public Shapes {
public:
    Matrix3x3(WindowManager* window, ScreenCalc* screen_calc, int rawColor) : Shapes(window, screen_calc, rawColor) {};
    std::array<std::array<int,SHAPE_DIM_3x3>,SHAPE_DIM_3x3> shapeMatrix{};
    void move(int direction) override;
    void draw(float x,float y) override;
    void rotate() override;
    int getMatrixSize() override { return SHAPE_DIM_3x3; }
    int getMatrixValue(int i, int j) override { return shapeMatrix[i][j]; }
    void changeMatrixValue(int i, int j, int value) override { shapeMatrix[i][j] = value; }
};

class ShapeJ : public Matrix3x3 {
    public:
    ShapeJ(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix3x3(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, c, 0},
            {0, c, 0},
            {c, c, 0}
        }};
    }
};

class ShapeL : public Matrix3x3 {
public:
    ShapeL(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix3x3(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, c, 0},
            {0, c, 0},
            {0, c, c}
        }};
    }
};

class ShapeZ : public Matrix3x3 {
    public:
    ShapeZ(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix3x3(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, 0, 0},
            {c, c, 0},
            {0, c, c}
        }};
    }
};

class ShapeS : public Matrix3x3 {
public:
    ShapeS(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix3x3(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, 0, 0},
            {0, c, c},
            {c, c, 0}
        }};
    }
};

class ShapeT : public Matrix3x3 {
public:
    ShapeT(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix3x3(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, c, 0},
            {c, c, c},
            {0, 0, 0}
        }};
    }
};


class Matrix4x4 :public Shapes {
public:
    Matrix4x4(WindowManager* window, ScreenCalc* screen_calc, int rawColor) : Shapes(window, screen_calc, rawColor){};
    std::array<std::array<int,SHAPE_DIM_4x4>,SHAPE_DIM_4x4> shapeMatrix{};
    void move(int direction) override;
    void draw(float x,float y) override;
    void rotate() override;
    int getMatrixSize() override { return SHAPE_DIM_4x4; }
    int getMatrixValue(int i, int j) override { return shapeMatrix[i][j]; }
    void changeMatrixValue(int i, int j, int value) override { shapeMatrix[i][j] = value; }
};

class ShapeI : public Matrix4x4 {
public:
    ShapeI(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix4x4(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, c, 0, 0},
            {0, c, 0, 0},
            {0, c, 0, 0},
            {0, c, 0, 0}
        }};
    }
};

class ShapeO : public Matrix4x4 {
public:
    ShapeO(WindowManager *window, ScreenCalc *screen_calc, int c) : Matrix4x4(window, screen_calc, c) {
        this->shapeMatrix =  {{
            {0, 0, 0, 0},
            {0, c, c, 0},
            {0, c, c, 0},
            {0, 0, 0, 0}
        }};
    }
};



class GameLogic {
    public:
    WindowManager* windowManager;
    ScreenCalc screenCalc;

    int savedMatrix[GAME_DIM_HEIGHT][GAME_DIM_WIDTH] = {false};
    int gameMatrix[GAME_DIM_HEIGHT][GAME_DIM_WIDTH] = {false}; //0 - empty, 0 > - colored block

    Shapes* currentShape = nullptr;

    int score = 0;
    int speed = 500; //ms

    void updateMatrix();


    GameLogic(WindowManager* window);

    //draw borders relative to window size
    void drawGame();


    void checkAndMove(int direction);
    void checkAndRotate();

    bool checkAndGenShape(); //todo collapse and generate objects

    ~GameLogic();

private:
    void destroyRows(int buttomLimit, int upperLimit); //line must be between checkAndMove() and checkAndGenShape()
    void drawBorders();
    void drawGameObjects();
    void createBorders();
};


#endif //TETRIS_GAMELOGIC_H