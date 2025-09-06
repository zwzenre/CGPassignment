#pragma once
#include "../../Game/Header/RaceCar.h"
#include <d3dx9.h>
#include <vector>

#ifndef GAME_PHYSICSMANAGER_H
#define GAME_PHYSICSMANAGER_H


class PhysicsManager {
private:
    float speed;
    D3DXVECTOR2 initial;
    bool collision;

public:
    PhysicsManager(float initialSpeed = 0.0f, D3DXVECTOR2 initialPos = D3DXVECTOR2(0.0f, 0.0f), bool enableCollision = true);

    void initialization();
    void update(float deltaTime);
    bool checkCollision(const D3DXVECTOR2& objectPos, float radius, const D3DXVECTOR2& otherPos, float otherRadius);
    void resolveCollision(const CollisionInfo& collisionInfo);
    void applyPhysics(float deltaTime);

    D3DXVECTOR2 getPosition();
    void setPosition;

    D3DXVECTOR2 getVelocity;
    void setVelocity;

    float getSpeed();
    void setSpeed();

    bool isCollisionEnabled() const { return collision; }
    void setCollisionEnabled(bool enable) { collision = enable; }




};


#endif //GAME_PHYSICSMANAGER_H