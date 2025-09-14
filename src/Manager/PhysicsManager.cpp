#include "Header/PhysicsManager.h"

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

bool PhysicsManager::CheckCarObstacleCollisions(RaceCar& car, std::vector<Obstacle*>& obstacles) {
    for (Obstacle* obstacle : obstacles) {
        if (!obstacle->IsDisappearing() && !obstacle->IsCollided()) {
            RECT obstacleBox = obstacle->GetBoundingBox();

            for (int i = 0; i < 4; ++i) {
                D3DXVECTOR2 corner = car.GetCorner(i);
                if (corner.x >= obstacleBox.left && corner.x <= obstacleBox.right &&
                    corner.y >= obstacleBox.top  && corner.y <= obstacleBox.bottom) {
                    obstacle->OnCollision(&car);
                    return true;
                }
            }
        }
    }
    return false;
}