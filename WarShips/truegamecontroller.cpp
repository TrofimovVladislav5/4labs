#include <iostream>
#include <vector>
#include "GameController.h"
#include "GameState.h"
#include "MenuGameState.h"
#include "OngoingGameState.h"
#include "GameView.h"
// class GameState{
// protected:
//     GameView* view;
// public:
//     virtual void openState();

//     virtual void updateState();

//     virtual void closeState();

//     virtual bool transitToState(GameState* newState);
// };

// class GameView{
// public:
//     void displayMessage(const std::string& message){
//         std::cout << message << std::endl;
//     }

//     void displayGameField(){

//     }
// };

// class OngoingGameState : public GameState{
// private:
//     std::string latestCommand;
// public:

//     void openState() override {
//         view->displayMessage("Ongoing game");
//     }
//      void closeState() override {
//         view->displayMessage("Quit the game");
//     }

//     void updateState() override{
//         std::getline(std::cin, latestCommand);
//     }

//     bool transitToState(GameState* newState) override {
//         if (latestCommand == "resume"){
//             newState = new OngoingGameState();
//             return true;
//         }
//         else if (latestCommand == "menu"){
//             newState = new MenuGameState();
//             return true;
//         }
//         return false;
//     }
// };

// class MenuGameState : public GameState{
// private:
//     std::string latestCommand;
// public:
//     void openState() override {
//         view->displayMessage("Game Menu");
//         view->displayMessage("Start\t Save\t Exit\t");
//     }

//     void closeState() override {
//         view->displayMessage("Quit the menu");
//     }

//     void updateState() override{
//         std::getline(std::cin,latestCommand);
//     }
//     bool transitToState(GameState* newState) override {
//         if (latestCommand == "resume"){
//             newState = new OngoingGameState();
//             return true;
//         }
//         return false;
//         // else if (latestCommand == "exit"){
//         //     newState = new ExitGamseState();
//         //     return true;
//         // }
//         // else if (latestCommand == "save"){
//         //     newState = new SaveGameState();
//         //     return true;
//         // }
//         // return false;
//     }
// };

// class OngoingGameState : public GameState{
// private:
//     std::string latestCommand;
// public:

//     void openState() override {
//         view->displayMessage("Ongoing game");
//     }
//      void closeState() override {
//         view->displayMessage("Quit the game");
//     }

//     void updateState() override{
//         std::getline(std::cin, latestCommand);
//     }

//     bool transitToState(GameState* newState) override {
//         if (latestCommand == "resume"){
//             newState = new OngoingGameState();
//             return true;
//         }
//         else if (latestCommand == "menu"){
//             newState = new MenuGameState();
//             return true;
//         }
//         return false;
//     }
// };
// class GameController{
// private:
//     GameState* currentState;
//     GameView* view;
// public:
//     GameController(){
//         currentState = new MenuGameState();
//         view = new GameView();
//     }

//     ~GameController(){
//         delete currentState;
//         delete view;
//     }

//     void run(){
//         while (true){
//             GameState* newState = nullptr;
//             if(currentState->transitToState(newState)){
//                 currentState->closeState();
//                 currentState = newState;
//                 currentState->openState();
//             }
//             currentState->updateState();
//         }
//     }
// };

int main(){
    GameController controller;
    controller.run();
    return 0;
}