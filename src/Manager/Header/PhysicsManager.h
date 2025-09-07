#pragma once
#include <d3dx9.h>
#include <vector>

class PhysicsManager {
public:
    // 移除单一物体属性，改为管理多个物体
    std::vector<class GameObject*> physicsObjects; // 需要一个GameObject基类


    PhysicsManager();
    ~PhysicsManager();

    void initialization(); // Game.cpp中调用的方法
    void update(float deltaTime);

    // 碰撞检测
    bool checkCollision(const D3DXVECTOR2& pos1, float radius1,
                       const D3DXVECTOR2& pos2, float radius2);

    // 添加/移除物理对象
    void addPhysicsObject(class GameObject* obj);
    void removePhysicsObject(class GameObject* obj);

    // 应用物理效果
    void applyPhysics(float deltaTime);
};