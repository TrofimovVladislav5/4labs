#include "ShipManager.h"

ShipManager::ShipManager(const std::vector<int>& shipsSize){
    for (auto& size : shipsSize){
        Ship* currentShip = new Ship(size);
        ships.push_back(currentShip);
    }
}

ShipManager::~ShipManager(){
    for (auto& ship : ships){
        delete ship;
    }
}

Ship* ShipManager::operator[](int index){
    if (index < 0 || index >= ships.size()) {
        throw std::out_of_range("Invalid index error");
    }
    return ships[index];
}
void ShipManager::addShip(int size){
    Ship* newShip = new Ship(size);
    ships.push_back(newShip);
}

void ShipManager::removeShipNumber(int indexRemoving){
    if (indexRemoving < 0 || indexRemoving >= ships.size()){
        throw std::out_of_range("Invalid Index for removing ship");
    }
    ships.erase(ships.begin() + indexRemoving);
}

std::vector<Ship*>& getShips(); { return ships;}
