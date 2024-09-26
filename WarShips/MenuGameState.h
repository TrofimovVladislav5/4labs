#pragma once
#include "GameState.h"
class MenuGameState : public GameState{
private:
    std::string latestCommand;
public:
    void openState() override;
    void closeState() override;
    void updateState() override;
    bool transitToState(GameState* newState) override;
};