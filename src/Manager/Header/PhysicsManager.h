#pragma once
#include "../../Game/Header/RaceCar.h"

#ifndef GAME_PHYSICSMANAGER_H
#define GAME_PHYSICSMANAGER_H

class PhysicsManager {
private:
    float maxSpeed = 10.0f;
    float accelerationRate = 0.2f;
    float brakeRate = 0.3f;
    float friction = 0.05f;
    float steeringAngle = 0.05f;

public:
    void updateRaceCar(RaceCar& car, bool forward, bool backward, bool left, bool right, int winWidth, int winHeight);
    bool checkCollision(const RaceCar& a, const RaceCar& b);

};

#endif