#pragma once
#include "Structures.h"
#include "Ship.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

bool validateCoordinates(std::pair<int, int> coordToCheck);

class GameField {
private:
    int width;
    int height;
    std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>> shipsCoordinateMap;
    std::unordered_set<std::pair<int, int>, hashFunc> attackCoordinateMap;
    bool shipCoordinatesInField(std::pair<int, int> coords, int length, Direction direction) const;
    bool shipsAreContacting(std::pair<int, int> coords) const;
    bool intersectionShips(std::pair<int , int> coordinates, int length, Direction direction) const;
public:
    GameField(int width, int height);
    int getHeight() const;
    int getWidth() const;
    std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>>& getShipCoordinatesMap() const;
    bool placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction);
    bool attack(std::pair<int, int> initialCoordinate, int damageCount);
};