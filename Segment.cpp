#include "Segment.h"

Segment::Segment(int maxSegmentHealth) 
        : maxSegmentHealth(maxSegmentHealth)
        , currentSegmentHealth(maxSegmentHealth)
    {}

void Segment::takeDamage(int damageCount) {
    currentSegmentHealth -= damageCount;
    if (currentSegmentHealth < 0) {
        currentSegmentHealth = 0;
    }
}
    
int Segment::getHitPoints() const {
    return currentSegmentHealth;
}

bool Segment::isDestroyed() {
    return currentSegmentHealth == 0;
}
