#define NOMINMAX
#include "Header/PhysicsManager.h"
#include <algorithm>
#include <numeric>

void PhysicsManager::ConstrainToBounds(RaceCar& car) {
    D3DXVECTOR2 pos = car.GetPosition();
    RECT box = car.GetBoundingBox();

    // Width/Height come directly from bounding box
    int carWidth  = box.right - box.left;
    int carHeight = box.bottom - box.top;

    if (pos.x < 0) pos.x = 0;
    if (pos.y < 0) pos.y = 0;
    if (pos.x > screenWidth - carWidth)  pos.x = (float)(screenWidth - carWidth);
    if (pos.y > screenHeight - carHeight) pos.y = (float)(screenHeight - carHeight);

    car.SetPosition(pos);
}

PhysicsManager::Projection PhysicsManager::ProjectOBB(const std::vector<D3DXVECTOR2>& vertices, const D3DXVECTOR2& axis) {
    Projection p;
    p.min = std::numeric_limits<float>::max();
    p.max = std::numeric_limits<float>::lowest();

    for (const auto& v : vertices) {
        float dotProduct = D3DXVec2Dot(&v, &axis);
        p.min = std::min(p.min, dotProduct);
        p.max = std::max(p.max, dotProduct);
    }
    return p;
}

bool PhysicsManager::Overlap(const Projection& p1, const Projection& p2) {
    return p1.max >= p2.min && p2.max >= p1.min;
}

bool PhysicsManager::CheckCarObstacleCollisions(RaceCar& car, std::vector<Obstacle*>& obstacles) {
    std::vector<D3DXVECTOR2> carVertices = car.GetOBBVertices();
    std::vector<D3DXVECTOR2> carAxes = car.GetOBBAxes();

    for (Obstacle* obstacle : obstacles) {
        if (!obstacle->IsDisappearing() && !obstacle->IsCollided()) {
            std::vector<D3DXVECTOR2> obstacleVertices = obstacle->GetOBBVertices();
            std::vector<D3DXVECTOR2> obstacleAxes = obstacle->GetOBBAxes();

            if (CheckOBBCollision(carVertices, carAxes, obstacleVertices, obstacleAxes)) {
                obstacle->OnCollision(&car);
                return true;
            }
        }
    }
    return false;
}

bool PhysicsManager::CheckOBBCollision(const std::vector<D3DXVECTOR2>& obb1Vertices,
                                       const std::vector<D3DXVECTOR2>& obb1Axes,
                                       const std::vector<D3DXVECTOR2>& obb2Vertices,
                                       const std::vector<D3DXVECTOR2>& obb2Axes) {

    // Test axes from OBB1
    for (const auto& axis : obb1Axes) {
        Projection p1 = ProjectOBB(obb1Vertices, axis);
        Projection p2 = ProjectOBB(obb2Vertices, axis);
        if (!Overlap(p1, p2)) {
            return false;
        }
    }

    // Test axes from OBB2
    for (const auto& axis : obb2Axes) {
        Projection p1 = ProjectOBB(obb1Vertices, axis);
        Projection p2 = ProjectOBB(obb2Vertices, axis);
        if (!Overlap(p1, p2)) {
            return false;
        }
    }

    return true;
}