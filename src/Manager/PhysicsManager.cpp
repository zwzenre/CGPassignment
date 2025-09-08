#include "Header/PhysicsManager.h"
#include <algorithm>

void PhysicsManager::updateRaceCar(RaceCar& car, bool forward, bool backward, bool left, bool right, int winWidth, int winHeight) {
    D3DXVECTOR2 vel = car.GetVelocity();
    float rot = car.GetRotation();

    // acceleration
    if (forward) {
        vel.x += sin(rot) * accelerationRate;
        vel.y += -cos(rot) * accelerationRate;
    }
    if (backward) {
        vel.x -= sin(rot) * brakeRate;
        vel.y -= -cos(rot) * brakeRate;
    }

    // steering
    if (D3DXVec2Length(&vel) > 0.1f) {
        if (left)  rot -= steeringAngle;
        if (right) rot += steeringAngle;
    }

    // friction
    vel.x *= (1.0f - friction);
    vel.y *= (1.0f - friction);

    // clamp speed
    float speed = D3DXVec2Length(&vel);
    if (speed > maxSpeed) {
        D3DXVec2Normalize(&vel, &vel);
        vel *= maxSpeed;
    }

    D3DXVECTOR2 pos = car.GetPosition();
    pos += vel;

    // boundary check
    int w = car.GetWidth();
    int h = car.GetHeight();

    if (pos.x < 0) { pos.x = 0; vel.x *= -0.5f; }
    if (pos.y < 0) { pos.y = 0; vel.y *= -0.5f; }
    if (pos.x > winWidth - w) { pos.x = (float)(winWidth - w); vel.x *= -0.5f; }
    if (pos.y > winHeight - h) { pos.y = (float)(winHeight - h); vel.y *= -0.5f; }

    car.SetVelocity(vel);
    car.SetRotation(rot);
    car.SetPosition(pos);
}

bool PhysicsManager::checkCollision(const RaceCar& a, const RaceCar& b) {
    RECT r1 = a.GetBoundingBox();
    RECT r2 = b.GetBoundingBox();

    return (r1.left < r2.right &&
            r1.right > r2.left &&
            r1.top < r2.bottom &&
            r1.bottom > r2.top);
}