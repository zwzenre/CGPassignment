#pragma once
#include "../../Game/Header/RaceCar.h"
#include "../../Game/Header/Obstacle.h"
#include <vector>

class PhysicsManager {
private:
    int screenWidth;
    int screenHeight;

public:
    PhysicsManager(int winW, int winH)
        : screenWidth(winW), screenHeight(winH) {}

    // Keep car inside screen bounds
    void ConstrainToBounds(RaceCar& car);

    // Checks car collision with a list of obstacles
    void CheckCarObstacleCollisions(RaceCar& car, std::vector<Obstacle*>& obstacles);
};
