#pragma once
#include "Ship.h"
#include <vector>

class ShipManager {
private:
    std::vector<Ship*> ships;
public:
    ShipManager(const std::vector<int>& shipsSize);
    ~ShipManager();
    std::vector<Ship*>& getShips();
    Ship* operator[](int index);
    void addShip(int size);
    void removeShipNumber(int indexRemoving);
};