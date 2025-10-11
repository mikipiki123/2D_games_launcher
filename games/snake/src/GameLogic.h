//
// Created by Michael Berner on 04/10/2025.
//

#include "../include/WindowManager.h"

#ifndef SNAKE_SNAKELOGIC_H
#define SNAKE_SNAKELOGIC_H

#define GAME_DIM 20 //assume square dimensions
#define SHIFT_ON_EACH_SIDE 10

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

typedef struct {
    float minSizeScreen;
    float maxSizeScreen;
    float center[2];
    float sizeOfCell;
} ScreenCalc;

class Entity {

    public:
    Entity(WindowManager* window, ScreenCalc* screen_calc);
    virtual void draw(float x,float y) = 0;
    bool move(int direction);
    float coord[2] = { GAME_DIM/2, GAME_DIM/2 }; // { x , y }
    float prevCoord[2] = {coord[0], coord[1]};
    int lastMove = NULL;

    virtual ~Entity() = 0;

    protected:
    WindowManager* windowManager;
    ScreenCalc* screenCalc;

};

class Head : public Entity {
    public:
    Head(WindowManager* window, ScreenCalc* screenCalc) : Entity(window, screenCalc) {}
    void draw(float x,float y) override;

    ~Head() override;
};

class Body :public Entity {
    public:
    Body(WindowManager* window, ScreenCalc* screenCalc) : Entity(window, screenCalc) {}
    void draw(float x,float y) override;

    ~Body() override;
};

class Apple :public Entity {
    public:
    Apple(WindowManager* window, ScreenCalc* screenCalc) : Entity(window, screenCalc) {
        this->coord[0] = 3;
        this->coord[1] = 3;
    }
    void draw(float x,float y) override;

    ~Apple() override;
};


class GameLogic {
    public:

    WindowManager* windowManager;
    ScreenCalc screenCalc;

    bool matrix[GAME_DIM][GAME_DIM] = {false};

    std::vector<Entity*> entities;

    Apple* apple;

    void updateMatrixAndSkeleton();

    GameLogic(WindowManager* window);

    //draw borders relative to window size
    void drawGame();

    //matrix borders
    void createBorders();

    void grow();

    void genApple();

    bool isEaten(int direction);

    bool isCorner(int direction);

    ~GameLogic();

    private:
    void drawBorders();
    void drawGameObjects();


};

#endif //SNAKE_SNAKELOGIC_H