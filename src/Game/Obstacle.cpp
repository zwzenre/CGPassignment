#include "Header/Obstacle.h"
#include "../Game/Header/RaceCar.h"
#include <algorithm>

Obstacle::Obstacle(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath)
        : Item(position, size, texturePath),
          state(Stationary),
          glideTimer(0.0f), glideDirection(0, 0), glideSpeed(100.0f),
          blinkTimer(0.0f), blinkInterval(0.1f), blinkCount(0),
          disappearTimer(0.0f), disappearDuration(0.5f), currentlyVisible(true) {}

Obstacle::~Obstacle() = default;

bool Obstacle::Initialize(IDirect3DDevice9* device) {
    return Item::Initialize(device);
}

void Obstacle::Update(float deltaTime) {
    switch (state) {
        case Normal:
            // Do nothing, obstacle is static
            break;
        case Gliding: {
            position += glideDirection * glideSpeed * deltaTime;
            glideTimer += deltaTime;
            if (glideTimer >= 0.2f) {
                state = Blinking;
                blinkTimer = 0.0f;
                blinkCount = 0;
                currentlyVisible = true;
            }
            break;
        }
        case Blinking: {
            blinkTimer += deltaTime;
            if (blinkTimer >= blinkInterval) {
                currentlyVisible = !currentlyVisible;
                blinkTimer = 0.0f;
                blinkCount++;
                if (blinkCount >= 6) {
                    state = Disappearing;
                    disappearTimer = 0.0f;
                    currentlyVisible = false;
                }
            }
            break;
        }
        case Disappearing: {
            disappearTimer += deltaTime;
            if (disappearTimer >= disappearDuration) {
                // Obstacle should be "removed" by the scene manager after this state
                // For now, we'll keep it invisible.
                // In Level1, you would check IsDisappearing() and mark for removal.
            }
            break;
        }
    }
}

void Obstacle::Render(LPD3DXSPRITE spriteBrush) {
    if (!texture || !spriteBrush || (state == Disappearing && !currentlyVisible)) return;

    if (state == Blinking && !currentlyVisible) {
        return;
    }

    D3DSURFACE_DESC desc;
    texture->GetLevelDesc(0, &desc);
    D3DXVECTOR2 textureSize((float)desc.Width, (float)desc.Height);

    D3DXVECTOR2 scaleVec(size.x / textureSize.x, size.y / textureSize.y);

    D3DXMATRIX mat;
    D3DXMatrixTransformation2D(
            &mat,
            nullptr,
            0.0f,
            &scaleVec,
            nullptr,
            0.0f,
            &position
    );
    spriteBrush->SetTransform(&mat);

    D3DXVECTOR3 drawPos(0.0f, 0.0f, 0.0f);
    spriteBrush->Draw(texture, NULL, NULL, &drawPos, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}

void Obstacle::OnCollision(RaceCar* car) {
    if (state == Stationary) {
        D3DXVECTOR2 carCenter = car->GetPosition() + D3DXVECTOR2(car->GetWidth() / 2.0f, car->GetHeight() / 2.0f);
        D3DXVECTOR2 obstacleCenter = position + D3DXVECTOR2(size.x / 2.0f, size.y / 2.0f);
        D3DXVECTOR2 impactDir = obstacleCenter - carCenter;
        D3DXVec2Normalize(&impactDir, &impactDir);

        TriggerCollisionEffect(impactDir);
    }
}

void Obstacle::TriggerCollisionEffect(const D3DXVECTOR2& impactDirection) {
    state = Gliding;
    glideTimer = 0.0f;
    glideDirection = impactDirection;
}