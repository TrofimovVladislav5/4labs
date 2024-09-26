#pragma once
#include "GameView.h"
#include "GameState.h"
#include "OngoingGameState.h"
#include "MenuGameState.h"

class OngoingGameState : public GameState{
private:
    std::string latestCommand;
public:

    void openState() override;
    void closeState() override;

    void updateState() override;

    bool transitToState(GameState* newState) override;
};
