#include "Header/Obstacle.h"
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
        case Stationary:
            // Do nothing, obstacle/box is stationary
            break;
        case Gliding: {
            D3DXVECTOR2 currentGlide = glideDirection * initialGlideSpeed * deltaTime;
            position += currentGlide;
            glidedDistance += D3DXVec2Length(&currentGlide);

            initialGlideSpeed -= initialGlideSpeed * 2.0f * deltaTime;

            if (glidedDistance >= glideDistance || initialGlideSpeed < 50.0f) {
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
            // Obstacle remains invisible. Level1 will handle actual deletion.
            break;
        }
    }
}

void Obstacle::Render(LPD3DXSPRITE spriteBrush) {
    if (!texture || !spriteBrush || !currentlyVisible) return;

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

        D3DXVECTOR2 velocity = car->GetVelocity();

        TriggerCollisionEffect(impactDir, velocity);
    }
}

void Obstacle::TriggerCollisionEffect(const D3DXVECTOR2& impactDirection, D3DXVECTOR2 velocity) {
    if (state == Stationary) {
        state = Gliding;
        glideTimer = 0.0f;
        glideDirection = impactDirection;
    }

    float carSpeedMagnitude = D3DXVec2Length(&velocity);

    initialGlideSpeed = (carSpeedMagnitude * 0.5f) < 500.0f ? (carSpeedMagnitude * 0.5f) : 500.0f;
    initialGlideSpeed = initialGlideSpeed > 150.0f ? initialGlideSpeed : 150.0f;

    glideDistance = initialGlideSpeed * 0.15f;
    glidedDistance = 0.0f;
}