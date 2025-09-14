#pragma once
#include "Item.h"

class Obstacle : public Item {
public:
    Obstacle(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath);
    ~Obstacle() override;

    bool Initialize(IDirect3DDevice9* device) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE spriteBrush) override;

    RECT GetBoundingBox() { return Item::GetBoundingBox(); }
    D3DXVECTOR2 GetPosition() const { return Item::GetPosition(); }
    D3DXVECTOR2 GetSize() const { return Item::GetSize(); }

    void OnCollision(class RaceCar* car) override;
    void TriggerCollisionEffect(const D3DXVECTOR2& impactDirection, D3DXVECTOR2 velocity);

    bool IsDisappearing() const { return state == Disappearing; }
    bool IsGliding() const { return state == Gliding; }
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
    float initialGlideSpeed;
    float glideDistance;
    float glidedDistance;

    float blinkTimer;
    float blinkInterval;
    int blinkCount;
    float disappearTimer;
    float disappearDuration;
    bool currentlyVisible;
};