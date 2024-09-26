#include <iostream>
#include "GameState.h"
#include "MenuGameState.h"
#include "OngoingGameState.h"
#include "GameView.h"

void MenuGameState::openState() {
    view->displayMessage("Game Menu");
    view->displayMessage("Start\t Save\t Exit\t");
}

void MenuGameState::closeState() {
    view->displayMessage("Quit the menu");
}

void MenuGameState::updateState() {
    std::getline(std::cin,latestCommand);
}

bool MenuGameState::transitToState(GameState* newState) {
    if (latestCommand == "resume"){
        newState = new OngoingGameState();
        return true;
    }
    return false;
        // else if (latestCommand == "exit"){
        //     newState = new ExitGamseState();
        //     return true;
        // }
        // else if (latestCommand == "save"){
        //     newState = new SaveGameState();
        //     return true;
        // }
        // return false;
}
