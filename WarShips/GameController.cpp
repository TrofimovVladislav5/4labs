#include "GameController.h"
#include "MenuGameState.h"

GameController::GameController(){
    currentState = new MenuGameState();
    view = new GameView();
}

GameController::~GameController(){
    delete currentState;
    delete view;
}

void GameController::run(){
    while (true){
        GameState* newState = nullptr;
        if (currentState){
            if(currentState->transitToState(newState)){
                currentState->closeState();
                currentState = newState;
                currentState->openState();
                }
            currentState->updateState();
        }
    }
}