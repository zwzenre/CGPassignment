#include "Header/RaceCar.h"
#include <cmath>
#include <algorithm>

#undef min
#undef max

RaceCar::RaceCar(D3DXVECTOR2 startPos)
    : position(startPos), velocity(0, 0), rotation(0.0f),
      enginePower(300.0f),
      maxSpeed(400.0f),
      currentSpeed(0.0f),
      accelerationRate(8.0f),
      decelerationRate(2.0f),
      brakePower(6.0f),
      dragCoefficient(0.05f),
      rollingResistance(0.02f),
      steeringAngle(0.0f),
      maxSteeringAngle(0.1f),
      steeringSpeed(0.1f),
      wheelBase(1.5f),
      tireGrip(0.8f),
      weightTransfer(0.1f),
      currentFrame(0),
      maxFrame(7),
      frameWidth(64),
      frameHeight(128),
      frameTimer(0.0f),
      frameDelay(0.08f) {}

void RaceCar::Update(float deltaTime, bool moveForward, bool moveBackward, bool turnLeft, bool turnRight) {
    ApplyPhysics(deltaTime, moveForward, moveBackward, turnLeft, turnRight);
    UpdateSteering(deltaTime, turnLeft, turnRight);
    UpdateAnimation(deltaTime, turnLeft, turnRight);
}

void RaceCar::ApplyPhysics(float deltaTime, bool moveForward, bool moveBackward, bool turnLeft, bool turnRight) {
    // ENGINE FORCE - MORE POWERFUL
    float engineForce = 0.0f;
    if (moveForward) {
        engineForce = enginePower * accelerationRate; // Added acceleration rate multiplier
    }

    // BRAKING/REVERSE FORCE - STRONGER
    float brakeForce = 0.0f;
    if (moveBackward) {
        if (currentSpeed > 0) {
            brakeForce = brakePower * 2.0f; // Stronger braking
        } else {
            engineForce = -enginePower * 0.8f; // Faster reverse
        }
    }

    // DRAG FORCE - reduced further
    float dragForce = dragCoefficient * fabsf(currentSpeed) * currentSpeed;

    // ROLLING RESISTANCE - reduced
    float rollingForce = rollingResistance * currentSpeed;

    // NET FORCE
    float netForce = engineForce - brakeForce - dragForce - rollingForce;

    // ACCELERATION - FASTER RESPONSE
    float acceleration = netForce;
    currentSpeed += acceleration * deltaTime * 2.0f; // Doubled acceleration effect

    // Clamp speed - faster maximums
    if (moveBackward && currentSpeed < 0) {
        currentSpeed = (std::max)(-maxSpeed * 0.8f, currentSpeed); // Faster reverse
    } else {
        currentSpeed = (std::min)(maxSpeed, (std::max)(-maxSpeed * 0.8f, currentSpeed));
    }

    // STEERING EFFECTS - SLOWER, MORE CONTROLLED
    if (fabsf(currentSpeed) > 1.0f && fabsf(steeringAngle) > 0.01f) {
        // Speed-dependent steering (more controlled at all speeds)
        float speedFactor = 0.5f + (0.5f * (1.0f - (fabsf(currentSpeed) / maxSpeed) * 0.3f));
        float turnRate = steeringAngle * currentSpeed * 0.2f * speedFactor; // Reduced turn rate
        rotation += turnRate * deltaTime;
    }

    // VELOCITY VECTOR
    D3DXVECTOR2 forward(sinf(rotation), -cosf(rotation));
    velocity = forward * currentSpeed;

    // POSITION UPDATE
    position += velocity * deltaTime;
}

void RaceCar::UpdateSteering(float deltaTime, bool turnLeft, bool turnRight) {
    float steeringInput = 0.0f;
    if (turnLeft) steeringInput -= 1.0f;
    if (turnRight) steeringInput += 1.0f;

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

void RaceCar::UpdateAnimation(float deltaTime, bool turnLeft, bool turnRight) {
    float animSpeedThreshold = 5.0f;

    if (fabsf(currentSpeed) > animSpeedThreshold) {
        frameTimer += deltaTime;

        if (turnLeft && !turnRight) {
            if (frameTimer >= frameDelay) {
                frameTimer = 0.0f;
                if (currentFrame >= 4 && currentFrame <= 6) {
                    currentFrame = 1;
                } else {
                    currentFrame = (currentFrame < 1 || currentFrame > 3) ? 1 : ((currentFrame - 1 + 1) % 3) + 1;
                }
            }
        }
        else if (turnRight && !turnLeft) {
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
    D3DXVECTOR2 scaling(1.0f, 1.0f);
    D3DXVECTOR2 pivot(center.x, center.y);

    D3DXMatrixTransformation2D(
        &mat,
        nullptr,
        0.0f,
        &scaling,
        &pivot,
        rotation,
        &trans
    );

    spriteBrush->SetTransform(&mat);
    spriteBrush->Draw(texture, &srcRect, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));

    // Reset transform
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}