#pragma once
#include "../../Game/Header/RaceCar.h"

#ifndef GAME_PHYSICSMANAGER_H
#define GAME_PHYSICSMANAGER_H

class PhysicsManager {
private:
    int screenWidth;
    int screenHeight;

public:
    PhysicsManager(int winW, int winH)
        : screenWidth(winW), screenHeight(winH) {}

    // Keep car inside screen bounds
    void ConstrainToBounds(RaceCar& car);

    // Placeholder for future obstacle collisions
    void CheckCarObstacleCollision(RaceCar& car);
};

#endif
