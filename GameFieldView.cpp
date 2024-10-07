#include "GameField.h"
#include "Ship.h"
#include "ShipManager.h"
#include "GameFieldView.h"
#include <iostream>

GameFieldView::GameFieldView(GameField& gameField) 
    : gameField(gameField)
{}

// void GameFieldView::displayField() {
//     for (const auto& [ship, coordinates] : gameField.getShipCoordinatesMap()) {
//         int index = 0;
//         for (const auto& coord : coordinates) {
//             int currentHitPoints = ship->getSegmentHitPoints(index);
//             if (currentHitPoints == 0) {
//                 fieldView[coord.second][coord.first] =;
//             }
//             else if (currentHitPoints > 0 && currentHitPoints != ship->getMaxSegmentHealth()) {
//                 fieldView[coord.second][coord.first] = "damaged";
//             }
//             else if (currentHitPoints == ship->getMaxSegmentHealth()) {
//                 fieldView[coord.second][coord.first] = "int";
//             }
//             index++;
//         }
//     }

//     scanCellsNextToDestroyedShip(fieldView,gameField.getShipCoordinatesMap());

//     std::string result;
//         std::string upperBar = "№ ";
//         for (int x = 0; x < width; x++) {
//             upperBar += std::to_string(x) + " ";
//         }
//         std::cout << std::endl;
//         std::cout << upperBar << std::endl;
//         for (int y = 0; y < height;y++) {
//             result.clear();
//             result += std::to_string(y) + " ";
//             for (int x = 0; x < width;x++) {
//                 if (fieldView[y][x] == "*") {
//                     result += "+ ";
//                 }
//                 else if (fieldView[y][x] == "int") {
//                     result += "2 " maxCurrent;
//                 }
//                 else if (fieldView[y][x] == "damaged") {
//                     result += "1 ";
//                 }
//                 else if (fieldView[y][x] == "destroyed") {
//                     result += "0 ";
//                 }
//                 else if ()
//             std::cout << result << std::endl;
//         }
//     }
// }

bool isShipNear(std::unordered_map<std::pair<int, int>, int,hashFunc>& shipCoordinates, std::pair<int, int> scanCell) {
    return shipCoordinates.at(std::make_pair(scanCell.first, scanCell.second + 1)) ||
        shipCoordinates.at(std::make_pair(scanCell.first, scanCell.second - 1)) ||
        shipCoordinates.at(std::make_pair(scanCell.first - 1, scanCell.second)) ||
        shipCoordinates.at(std::make_pair(scanCell.first + 1, scanCell.second)) ||
        shipCoordinates.at(std::make_pair(scanCell.first - 1, scanCell.second - 1)) ||
        shipCoordinates.at(std::make_pair(scanCell.first + 1, scanCell.second + 1)) ||
        shipCoordinates.at(std::make_pair(scanCell.first + 1, scanCell.second - 1)) ||
        shipCoordinates.at(std::make_pair(scanCell.first - 1, scanCell.second + 1));
}

void GameFieldView::displayField() {
    std::unordered_map<std::pair<int, int>, int, hashFunc> shipCoordinates;

    for (const auto& [ship, coordinates] : gameField.getShipCoordinatesMap()) {
        int index = 0;
        for (const auto& coordinate : coordinates) {
            shipCoordinates[coordinate] = ship->getSegmentHitPoints(index);
            index++;
        }
    }

    std::string result;
    std::string upperBar = "№ ";
    for (int x = 0; x < gameField.getWidth(); x++) {
        upperBar += std::to_string(x) + " ";
    }
    std::cout << std::endl;
    std::cout << upperBar << std::endl;
    for (int y = 0; y < gameField.getHeight(); y++) {
        result.clear();
        result += std::to_string(y) + " ";
        for (int x = 0; x < gameField.getWidth(); x++) {
            const int cellHitPoints = shipCoordinates.at(std::make_pair(x, y));
            if (cellHitPoints) {
                result += std::to_string(cellHitPoints) + " ";
            } else if (isShipNear(shipCoordinates, std::make_pair(x, y))) {
                result += "+ ";
            } else {
                result += "* ";
            }
        }
    }
}

int main() {
    ShipManager manager({4,3,2,1});
    GameField gameField(10,10);
    GameFieldView viewField(gameField);
    Ship* newShip = new Ship(4);
    bool e = gameField.placeShip(manager[0], {0,0}, Direction::horizontal);
    e = gameField.placeShip(manager[1], {3,5}, Direction::horizontal);
    e = gameField.placeShip(manager[2], {7,7}, Direction::vertical);
    e = gameField.placeShip(manager[3], {9,9}, Direction::vertical);
    gameField.attack({0,0}, 1);
    gameField.attack({1,0}, 1);
    gameField.attack({1,0}, 1);
    viewField.displayField();
}