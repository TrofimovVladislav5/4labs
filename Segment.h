#pragma once

class Segment {
private:
    int maxSegmentHealth;
    int currentSegmentHealth;
public:
    Segment(int maxSegmentHealth);
    void takeDamage(int damageCount);
    int getHitPoints() const;
    bool isDestroyed();
};