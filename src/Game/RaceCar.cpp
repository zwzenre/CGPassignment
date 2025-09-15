#include "Header/RaceCar.h"
#include <cmath>
#include <algorithm>

#undef min
#undef max

RaceCar::RaceCar(D3DXVECTOR2 startPos, int screenW, int screenH, float carMass)
    : position(startPos), velocity(0,0), rotation(0.0f),
      mass(carMass), enginePower(300.0f), maxSpeed(400.0f), currentSpeed(0.0f),
      accelerationRate(5.0f), decelerationRate(2.0f),
      brakePower(6.0f), dragCoefficient(0.05f),
      rollingResistance(0.02f), steeringAngle(0.0f),
      maxSteeringAngle(0.1f), steeringSpeed(0.1f),
      currentFrame(0), maxFrame(7), frameWidth(64), frameHeight(128),
      frameTimer(0.0f), frameDelay(0.08f),
      screenWidth(screenW), screenHeight(screenH)
{
    scale = D3DXVECTOR2(
        static_cast<float>(screenWidth)  / 1920.0f,
        static_cast<float>(screenHeight) / 1080.0f
    );
}

void RaceCar::Update(float dt, bool forward, bool backward,
                     bool left, bool right)
{
    ApplyPhysics(dt, forward, backward, left, right);
    UpdateSteering(dt, left, right);
    UpdateAnimation(dt, left, right);
    ClampToScreen();
}

void RaceCar::ApplyPhysics(float dt, bool forward, bool backward,
                           bool left, bool right)
{
    float engineForce = forward ? enginePower * accelerationRate : 0.0f;

    float brakeForce = 0.0f;
    if (backward) {
        if (currentSpeed > 0)  brakeForce = brakePower * 2.0f;
        else                   engineForce = -enginePower * 0.8f;
    }

    float dragForce    = dragCoefficient   * fabsf(currentSpeed) * currentSpeed;
    float rollingForce = rollingResistance * currentSpeed;

    float netForce = engineForce - brakeForce - dragForce - rollingForce;
    float acceleration = netForce;

    currentSpeed += acceleration * dt * 2.0f;

    if (backward && currentSpeed < 0)
        currentSpeed = (std::max)(-maxSpeed * 0.8f, currentSpeed);
    else
        currentSpeed = (std::min)(maxSpeed,
                       (std::max)(-maxSpeed * 0.8f, currentSpeed));

    if (fabsf(currentSpeed) > 1.0f && fabsf(steeringAngle) > 0.01f) {
        float speedFactor = 0.5f + 0.5f * (1.0f - (fabsf(currentSpeed)/maxSpeed)*0.3f);
        float turnRate = steeringAngle * currentSpeed * 0.2f * speedFactor;
        rotation += turnRate * dt;
    }

    D3DXVECTOR2 forwardVec(sinf(rotation), -cosf(rotation));
    velocity = forwardVec * currentSpeed;
    position += velocity * dt;
}

void RaceCar::UpdateSteering(float dt, bool left, bool right)
{
    float input = 0.0f;
    if (left)  input -= 1.0f;
    if (right) input += 1.0f;

    float speedFactor = 0.7f + 0.3f * (1.0f - fabsf(currentSpeed) / maxSpeed);
    steeringAngle += input * steeringSpeed * speedFactor * dt;

    if (input == 0.0f) {
        float returnSpeed = steeringSpeed * 2.5f;
        if (steeringAngle > 0)
            steeringAngle = (std::max)(0.0f, steeringAngle - returnSpeed * dt);
        else if (steeringAngle < 0)
            steeringAngle = (std::min)(0.0f, steeringAngle + returnSpeed * dt);
    }

    steeringAngle = (std::max)(-maxSteeringAngle,
                     (std::min)(steeringAngle, maxSteeringAngle));
}

void RaceCar::UpdateAnimation(float dt, bool left, bool right)
{
    float animSpeedThreshold = 5.0f;

    if (fabsf(currentSpeed) > animSpeedThreshold) {
        frameTimer += dt;

        if (left && !right) {
            if (frameTimer >= frameDelay) {
                frameTimer = 0.0f;
                if (currentFrame >= 4 && currentFrame <= 6) currentFrame = 1;
                else currentFrame = (currentFrame < 1 || currentFrame > 3)
                                   ? 1 : ((currentFrame - 1 + 1) % 3) + 1;
            }
        }
        else if (right && !left) {
            if (frameTimer >= frameDelay) {
                frameTimer = 0.0f;
                if (currentFrame >= 1 && currentFrame <= 3) currentFrame = 4;
                else currentFrame = (currentFrame < 4 || currentFrame > 6)
                                   ? 4 : ((currentFrame - 4 + 1) % 3) + 4;
            }
        }
        else if (currentFrame != 0) {
            frameTimer += dt * 1.5f;
            if (frameTimer >= frameDelay * 1.5f) {
                frameTimer = 0.0f;
                currentFrame = 0;
            }
        }
    }
    else if (currentFrame != 0) {
        frameTimer += dt;
        if (frameTimer >= frameDelay * 2.0f) {
            frameTimer = 0.0f;
            currentFrame = 0;
        }
    }
}

void RaceCar::Render(LPD3DXSPRITE spriteBrush, LPDIRECT3DTEXTURE9 texture)
{
    RECT srcRect;
    srcRect.left   = currentFrame * frameWidth;
    srcRect.top    = 0;
    srcRect.right  = srcRect.left + frameWidth;
    srcRect.bottom = srcRect.top  + frameHeight;

    D3DXVECTOR2 center(frameWidth / 2.0f, frameHeight / 2.0f);
    D3DXVECTOR2 trans(position.x, position.y);

    D3DXMATRIX mat;
    D3DXMatrixTransformation2D(&mat, nullptr, 0.0f,
                               &scale, &center, rotation, &trans);

    spriteBrush->SetTransform(&mat);
    spriteBrush->Draw(texture, &srcRect, nullptr, nullptr,
                      D3DCOLOR_XRGB(255,255,255));

    // reset transform
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}

// Collision helpers
RECT RaceCar::GetBoundingBox() const
{
    RECT box;
    box.left   = static_cast<LONG>(position.x);
    box.top    = static_cast<LONG>(position.y);
    box.right  = static_cast<LONG>(position.x + frameWidth  * scale.x);
    box.bottom = static_cast<LONG>(position.y + frameHeight * scale.y);
    return box;
}

bool RaceCar::CarRectCollision(const RECT& other) const
{
    RECT me = GetBoundingBox();
    return !(me.right  < other.left  ||
             me.left   > other.right ||
             me.bottom < other.top   ||
             me.top    > other.bottom);
}

D3DXVECTOR2 RaceCar::GetCorner(int cornerIndex) const {
    D3DXVECTOR2 localCorners[4];
    localCorners[0] = D3DXVECTOR2(-frameWidth / 2.0f, -frameHeight / 2.0f);
    localCorners[1] = D3DXVECTOR2( frameWidth / 2.0f, -frameHeight / 2.0f);
    localCorners[2] = D3DXVECTOR2( frameWidth / 2.0f,  frameHeight / 2.0f);
    localCorners[3] = D3DXVECTOR2(-frameWidth / 2.0f,  frameHeight / 2.0f);

    D3DXMATRIX mat;
    D3DXVECTOR2 center(0.0f, 0.0f);
    D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scale, &center, rotation, nullptr);

    D3DXVECTOR2 transformedCorner;
    D3DXVec2TransformCoord(&transformedCorner, &localCorners[cornerIndex], &mat);

    D3DXVECTOR2 carRenderCenter = position + D3DXVECTOR2(GetWidth()/2.0f, GetHeight()/2.0f);

    return carRenderCenter + transformedCorner;
}

void RaceCar::ClampToScreen()
{
    float w = frameWidth  * scale.x;
    float h = frameHeight * scale.y;

    if (position.x < 0)                 position.x = 0;
    if (position.y < 0)                 position.y = 0;
    if (position.x > screenWidth  - w)  position.x = screenWidth  - w;
    if (position.y > screenHeight - h)  position.y = screenHeight - h;
}