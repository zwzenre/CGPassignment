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

void PhysicsManager::CheckCarObstacleCollision(RaceCar& car) {
    // TODO: will be filled once Obstacle/Box class is ready
}