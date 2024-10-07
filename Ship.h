#pragma once 
#include <vector>
#include <iostream>
#include "Segment.h"
class Ship {
private:
    int maxSegmentHealth;
    std::vector<Segment> segments;
public:
    Ship(int length, int maxSegmentHealth = 2);
    ~Ship() = default;
    int getLength();
    int getSegmentHitPoints(int index);
    int getMaxSegmentHealth();
    bool takeDamage(int indexSegment, int damageCount);
    bool isDestroyed();
    void status();
};