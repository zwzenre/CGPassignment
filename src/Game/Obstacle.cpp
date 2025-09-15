#include "Header/Obstacle.h"

Obstacle::Obstacle(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath, float mass)
        : Item(position, size, texturePath),
          state(Stationary),
          mass(mass),
          frictionCoefficient(1.2f),
          glideTimer(0.0f), glideDirection(0, 0), glideSpeed(0.0f),
          initialGlideSpeed(0.0f), glidedDistance(0.0f), maxGlideDistance(0.0f),
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
            if (glideSpeed > 0.1f) {
                D3DXVECTOR2 currentDisplacement = glideDirection * glideSpeed * deltaTime;
                position += currentDisplacement;
                glidedDistance += D3DXVec2Length(&currentDisplacement);

                // Apply friction to slow down the obstacle
                glideSpeed -= frictionCoefficient * deltaTime * 100.0f;

                if (glidedDistance >= maxGlideDistance || glideSpeed <= 0.1f) {
                    state = Blinking;
                    blinkTimer = 0.0f;
                    blinkCount = 0;
                    currentlyVisible = true;
                    glideSpeed = 0.0f;
                }
            } else {
                state = Blinking;
                blinkTimer = 0.0f;
                blinkCount = 0;
                currentlyVisible = true;
                glideSpeed = 0.0f;
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

        TriggerCollisionEffect(impactDir, car->GetMass(), car->GetVelocity());
    }
}

void Obstacle::TriggerCollisionEffect(const D3DXVECTOR2& impactDirection, float carMass, D3DXVECTOR2 carVelocity) {
    if (state == Stationary) {
        state = Gliding;
        glideTimer = 0.0f;
        glideDirection = impactDirection;
        glidedDistance = 0.0f;

        // Calculate initial glide speed
        float carSpeedMagnitude = D3DXVec2Length(&carVelocity);
        float effectiveImpulse = carMass * carSpeedMagnitude;

        initialGlideSpeed = effectiveImpulse / this->mass * 0.5f;

        // Clamp initial speed to a reasonable range
        initialGlideSpeed = max(100.0f, min(initialGlideSpeed, 600.0f));

        glideSpeed = initialGlideSpeed;

        maxGlideDistance = ((initialGlideSpeed * initialGlideSpeed) / (2 * (frictionCoefficient * 100.0f))) * 0.6f;
        maxGlideDistance = max(50.0f, min(maxGlideDistance, 500.0f));
    }
}

std::vector<D3DXVECTOR2> Obstacle::GetOBBVertices() const {
    std::vector<D3DXVECTOR2> vertices(4);

    vertices[0] = position;
    vertices[1] = position + D3DXVECTOR2(size.x, 0.0f);
    vertices[2] = position + D3DXVECTOR2(size.x, size.y);
    vertices[3] = position + D3DXVECTOR2(0.0f, size.y);

    return vertices;
}

std::vector<D3DXVECTOR2> Obstacle::GetOBBAxes() const {
    std::vector<D3DXVECTOR2> axes(2);

    axes[0] = D3DXVECTOR2(1.0f, 0.0f);
    axes[1] = D3DXVECTOR2(0.0f, 1.0f);
    return axes;
}