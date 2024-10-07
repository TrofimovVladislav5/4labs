#pragma once
#include "ShipManager.h"

class ShipManagerView {
private:
    std::vector<Ship*> ships
public:
    ShipManagerView(ShipManager& manager);
    void displayShips();
};