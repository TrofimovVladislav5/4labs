#include "GameField.h"
#include "Structures.h"
#include <unordered_map>
#include <unordered_set>

bool validateCoordinates(std::pair<int, int> coordToCheck) {
    return coordToCheck.first > 0 && coordToCheck.first < width && coordToCheck.second > 0 && coordToCheck.second < height;
}

GameField::GameField(int width, int height)
    : width(width)
    , height(height)
    , field(std::vector<std::vector<Cell>>(height, std::vector<Cell>(width, Cell::empty)))
{}

int GameField::getHeight() const {
    return height;
}

int GameField::getWidth() const {
    return width;
}
bool GameField::placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction) {
    int length = ship->getLength();
    if (!shipCoordinatesInField(initialCoordinate, length, direction) || intersectionShips(initialCoordinate, length, direction)) {
        std::cout << "Can't place ship" << std::endl;
        return false;
    }

    for (int i = 0; i < length;i++) {
        std::pair<int, int> newCoordinate = initialCoordinate;
        if (direction == Direction::horizontal) {
            newCoordinate.first += i;
        }
        else {
            newCoordinate.second += i;
        }
        shipsCoordinateMap[ship].insert(newCoordinate);
    }

    return true;
}

bool GameField::shipCoordinatesInField(std::pair<int, int> coords, int length, Direction direction) const {
    if (direction == Direction::horizontal) {
        return coords.first + length <= width;
    
    else {
        return coords.second + length <= height;
    }
    }
}

bool GameField::shipsAreContacting(std::pair<int, int> coords) const {
    for (int dy = -1;dy <= 1;dy++){
        for (int dx = -1;dx <= 1;dx++){
            int newX = coords.first + dx;
            int newY = coords.second + dy;
            if (newX >= 0 && newX < width && newY >= 0 && newY < height){
                std::pair<int, int> neighborCoords = {newX, newY};
                for (const auto& [ship, coordinates] : shipsCoordinateMap){
                    if (coordinates.find(neighborCoords) != coordinates.end()){
                        return true;
                        }
                    }
                }
            }
        }
    return false;
    }

bool GameField::intersectionShips(std::pair<int , int> coordinates, int length, Direction direction) const {
    for (int i = 0;i < length;i++){
        std::pair<int, int> tempCoordinates = coordinates;
        if (direction == Direction::horizontal) {
            tempCoordinates.first += i;
            }
        else if (direction == Direction::vertical){
            tempCoordinates.second += i;
            }

        for (const auto& [ship, coords] : shipsCoordinateMap) {
            if (coords.find(tempCoordinates) != coords.end()) {
                return true;
                }
            }

        if (shipsAreContacting(tempCoordinates)) return true;
        }
    return false;
}


bool GameField::attack(std::pair<int, int> initialCoordinate, int damageCount) {
    if (initialCoordinate.first < 0 || initialCoordinate.first >= width 
    ||  initialCoordinate.second < 0 || initialCoordinate.second >= height) throw std::out_of_range("Invalid coordinates to attack");


    for (const auto& [ship, coordinate] : shipsCoordinateMap) {
        int index = 0;
        for (auto& coord : coordinate) {
            if (coord == initialCoordinate) {
                SegmentState currentState = ship->getSegment(index);
                ship->takeDamage(index, damageCount);
                attackCoordinateMap.insert(coord);
                return true;
                }
            index++;
            }
        }
    return false;
}


std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>>& GameField::getShipCoordinatesMap() {return shipsCoordinateMap;}