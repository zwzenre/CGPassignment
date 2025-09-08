#pragma once
#include <d3dx9.h>
#include <vector>

class PhysicsManager {
public:
    std::vector<class GameObject*> physicsObjects;


    PhysicsManager();
    ~PhysicsManager();

    void initialization();
    void update(float deltaTime);

    bool checkCollision(const D3DXVECTOR2& pos1, float radius1,
                       const D3DXVECTOR2& pos2, float radius2);

    void addPhysicsObject(class GameObject* obj);
    void removePhysicsObject(class GameObject* obj);

    void applyPhysics(float deltaTime);
};