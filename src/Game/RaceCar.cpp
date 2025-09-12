#include "Header/RaceCar.h"
#include <cmath>
#include <algorithm>

#undef min
#undef max

RaceCar::RaceCar(D3DXVECTOR2 startPos, int screenW, int screenH)
    : position(startPos), velocity(0, 0), rotation(0.0f),
      enginePower(300.0f),
      maxSpeed(400.0f),
      currentSpeed(0.0f),
      accelerationRate(5.0f),
      decelerationRate(2.0f),
      brakePower(6.0f),
      dragCoefficient(0.05f),
      rollingResistance(0.02f),
      steeringAngle(0.0f),
      maxSteeringAngle(0.1f),
      steeringSpeed(0.1f),
      currentFrame(0),
      maxFrame(7),
      frameWidth(64),
      frameHeight(128),
      frameTimer(0.0f),
      frameDelay(0.08f),
      screenWidth(screenW),
      screenHeight(screenH)
{
    scale = D3DXVECTOR2(
        static_cast<float>(screenWidth) / 1920.0f,
        static_cast<float>(screenHeight) / 1080.0f
    );
}

void RaceCar::Update(float deltaTime, bool forward, bool backward, bool left, bool right) {
    ApplyPhysics(deltaTime, forward, backward, left, right);
    UpdateSteering(deltaTime, left, right);
    UpdateAnimation(deltaTime, left, right);
    ClampToScreen(); // prevent teleporting
}

void RaceCar::ApplyPhysics(float deltaTime, bool forward, bool backward, bool left, bool right) {
    float engineForce = 0.0f;
    if (forward) {
        engineForce = enginePower * accelerationRate;
    }

    float brakeForce = 0.0f;
    if (backward) {
        if (currentSpeed > 0) {
            brakeForce = brakePower * 2.0f;
        } else {
            engineForce = -enginePower * 0.8f;
        }
    }

    float dragForce = dragCoefficient * fabsf(currentSpeed) * currentSpeed;
    float rollingForce = rollingResistance * currentSpeed;

    float netForce = engineForce - brakeForce - dragForce - rollingForce;
    float acceleration = netForce;

    currentSpeed += acceleration * deltaTime * 2.0f;

    if (backward && currentSpeed < 0) {
        currentSpeed = (std::max)(-maxSpeed * 0.8f, currentSpeed);
    } else {
        currentSpeed = (std::min)(maxSpeed, (std::max)(-maxSpeed * 0.8f, currentSpeed));
    }

    if (fabsf(currentSpeed) > 1.0f && fabsf(steeringAngle) > 0.01f) {
        float speedFactor = 0.5f + (0.5f * (1.0f - (fabsf(currentSpeed) / maxSpeed) * 0.3f));
        float turnRate = steeringAngle * currentSpeed * 0.2f * speedFactor;
        rotation += turnRate * deltaTime;
    }

    D3DXVECTOR2 forwardVec(sinf(rotation), -cosf(rotation));
    velocity = forwardVec * currentSpeed;
    position += velocity * deltaTime;
}

void RaceCar::UpdateSteering(float deltaTime, bool left, bool right) {
    float steeringInput = 0.0f;
    if (left)  steeringInput -= 1.0f;
    if (right) steeringInput += 1.0f;

    float speedFactor = 0.7f + (0.3f * (1.0f - fabsf(currentSpeed) / maxSpeed));
    steeringAngle += steeringInput * steeringSpeed * speedFactor * deltaTime;

    if (steeringInput == 0.0f) {
        float returnSpeed = steeringSpeed * 2.5f;
        if (steeringAngle > 0) {
            steeringAngle -= returnSpeed * deltaTime;
            steeringAngle = (std::max)(0.0f, steeringAngle);
        } else if (steeringAngle < 0) {
            steeringAngle += returnSpeed * deltaTime;
            steeringAngle = (std::min)(0.0f, steeringAngle);
        }
    }

    steeringAngle = (std::max)(-maxSteeringAngle, (std::min)(steeringAngle, maxSteeringAngle));
}

void RaceCar::UpdateAnimation(float deltaTime, bool left, bool right) {
    float animSpeedThreshold = 5.0f;

    if (fabsf(currentSpeed) > animSpeedThreshold) {
        frameTimer += deltaTime;

        if (left && !right) {
            if (frameTimer >= frameDelay) {
                frameTimer = 0.0f;
                if (currentFrame >= 4 && currentFrame <= 6) {
                    currentFrame = 1;
                } else {
                    currentFrame = (currentFrame < 1 || currentFrame > 3) ? 1 : ((currentFrame - 1 + 1) % 3) + 1;
                }
            }
        }
        else if (right && !left) {
            if (frameTimer >= frameDelay) {
                frameTimer = 0.0f;
                if (currentFrame >= 1 && currentFrame <= 3) {
                    currentFrame = 4;
                } else {
                    currentFrame = (currentFrame < 4 || currentFrame > 6) ? 4 : ((currentFrame - 4 + 1) % 3) + 4;
                }
            }
        }
        else {
            if (currentFrame != 0) {
                frameTimer += deltaTime * 1.5f;
                if (frameTimer >= frameDelay * 1.5f) {
                    frameTimer = 0.0f;
                    currentFrame = 0;
                }
            }
        }
    }
    else {
        if (currentFrame != 0) {
            frameTimer += deltaTime;
            if (frameTimer >= frameDelay * 2.0f) {
                frameTimer = 0.0f;
                currentFrame = 0;
            }
        }
    }
}

void RaceCar::Render(LPD3DXSPRITE spriteBrush, LPDIRECT3DTEXTURE9 texture) {
    RECT srcRect;
    srcRect.left   = currentFrame * frameWidth;
    srcRect.top    = 0;
    srcRect.right  = srcRect.left + frameWidth;
    srcRect.bottom = srcRect.top + frameHeight;

    D3DXVECTOR2 center(frameWidth / 2.0f, frameHeight / 2.0f);
    D3DXVECTOR2 trans(position.x, position.y);

    D3DXMATRIX mat;
    D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scale, &center, rotation, &trans);

    spriteBrush->SetTransform(&mat);
    spriteBrush->Draw(texture, &srcRect, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));

    // Reset transform
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}

RECT RaceCar::GetBoundingBox() const {
    RECT box;
    box.left   = static_cast<LONG>(position.x);
    box.top    = static_cast<LONG>(position.y);
    box.right  = static_cast<LONG>(position.x + frameWidth * scale.x);
    box.bottom = static_cast<LONG>(position.y + frameHeight * scale.y);
    return box;
}

void RaceCar::ClampToScreen() {
    float carWidth  = frameWidth * scale.x;
    float carHeight = frameHeight * scale.y;

    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > screenWidth - carWidth)  position.x = screenWidth - carWidth;
    if (position.y > screenHeight - carHeight) position.y = screenHeight - carHeight;
}