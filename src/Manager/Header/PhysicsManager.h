#pragma once
#include "../../Game/Header/RaceCar.h"
#include "../../Game/Header/Obstacle.h"
#include <vector>

class PhysicsManager {
private:
    int screenWidth;
    int screenHeight;

    // Helper for SAT
    struct Projection {
        float min;
        float max;
    };

    Projection ProjectOBB(const std::vector<D3DXVECTOR2>& vertices, const D3DXVECTOR2& axis);

    bool Overlap(const Projection& p1, const Projection& p2);

public:
    PhysicsManager(int winW, int winH)
        : screenWidth(winW), screenHeight(winH) {}

    // Keep car inside screen bounds
    void ConstrainToBounds(RaceCar& car);

    // Checks OBB collision between two entities
    bool CheckOBBCollision(const std::vector<D3DXVECTOR2>& obb1Vertices,
                           const std::vector<D3DXVECTOR2>& obb1Axes,
                           const std::vector<D3DXVECTOR2>& obb2Vertices,
                           const std::vector<D3DXVECTOR2>& obb2Axes);

    // Checks car collision with a list of obstacles
    bool CheckCarObstacleCollisions(RaceCar& car, std::vector<Obstacle*>& obstacles);
};
