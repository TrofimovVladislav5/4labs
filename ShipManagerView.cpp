#include "ShipManagerView.h"

ShipManagerView::ShipManagerView(ShipManager& manager) 
    : ships(manager.getShips())
{}

void ShipManagerView::displayShips(){
    for (int i = 0; i < ships.size();i++) {
        std::cout << "Ship " << std::to_string(i + 1) 
                  << " length " << ships[i]->getLength() 
                  << " "; ships[i]->status(); 
    }
}