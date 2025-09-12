#pragma once
#include "Header/Item.h"

class Obstacle : public Item {
public:
    Obstacle(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath);
    ~Obstacle() override;

    bool Initialize(IDirect3DDevice9* device) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE spriteBrush) override;

    RECT GetBoundingBox() { return Item::GetBoundingBox(); }

    void OnCollision(class RaceCar* car) override;

    // New: State management for collision
    void TriggerCollisionEffect(const D3DXVECTOR2& impactDirection);
    bool IsDisappearing() const { return state == Disappearing; }
    bool IsCollided() const { return state != Stationary; }

private:
    enum State {
        Stationary,
        Gliding,
        Blinking,
        Disappearing
    };
    State state;

    float glideTimer;
    D3DXVECTOR2 glideDirection;
    float glideSpeed;

    float blinkTimer;
    float blinkInterval;
    int blinkCount;
    float disappearTimer;
    float disappearDuration;
    bool currentlyVisible;
};