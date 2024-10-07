#pragma once
#include "GameField.h"
#include <iostream>
#include <vector>

class GameFieldView {
private:
    GameField gameField;
public:
    GameFieldView(GameField& gameField);
    void displayField();
    //void scanCellsNextToDestroyedShip(std::vector<std::vector<Cell>>& field, std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>>& shipsCoordinateMap);
};