#include "Ship.h"
Ship::Ship(int length, int maxSegmentHealth = 2) 
    : maxSegmentHealth(maxSegmentHealth)
    , segments(std::vector<Segment>(length, maxSegmentHealth))
    {}

Ship::~Ship() = default;

int Ship::getLength() {return segments.size();}

int Ship::getSegmentHitPoints(int index){
    if (index < 0 || index >= segments.size()) {
        throw std::out_of_range("Invalid index error segment");
    }
    return segments[index].getHitPoints();
}

int Ship::getMaxSegmentHealth() {return maxSegmentHealth;}
bool Ship::takeDamage(int indexSegment, int damageCount);
bool Ship::isDestroyed();

void Ship::status(){
    std::string shipInfo;
    for (int i = segments.size() - 1;i >= 0;i--) {
        switch (segments[i].getSegmentHitPoints()){
            case maxSegmentHealth:
                shipInfo += " int ";
                break;
            case 0:
                shipInfo += " destroyed ";
                break;
            default:
                shipInfo += " damaged ";
                break;
        }
    }
    std::cout << "Segments info: " << shipInfo << std::endl;
}
