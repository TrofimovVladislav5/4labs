#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"
#include "GameView.h"

void OngoingGameState::openState() {
    view->displayMessage("Ongoing game");
}
void OngoingGameState::closeState() {
    view->displayMessage("Quit the game");
}

void OngoingGameState::updateState() {
    std::getline(std::cin, latestCommand);
}

bool OngoingGameState::transitToState(GameState* newState) {
    if (latestCommand == "resume"){
        newState = new OngoingGameState();
        return true;
    }
    else if (latestCommand == "menu"){
        newState = new MenuGameState();
        return true;
    }
    return false;
}
