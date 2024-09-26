#pragma once
#include "GameState.h"
#include "GameView.h"
class GameController{
private:
    GameState* currentState;
    GameView* view;
public:
    GameController();

    ~GameController();
    
    void run();
};