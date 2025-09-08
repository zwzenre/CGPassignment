#include "Header/RaceCar.h"
#include <cmath>
#include <algorithm>

#undef min
#undef max

RaceCar::RaceCar(D3DXVECTOR2 startPos)
    : position(startPos), velocity(0, 0), rotation(0.0f),
      enginePower(100.0f),
      maxSpeed(200.0f),
      currentSpeed(0.0f),
      accelerationRate(4.0f),
      decelerationRate(1.0f),
      brakePower(3.0f),
      dragCoefficient(0.08f),
      rollingResistance(0.03f),
      steeringAngle(0.0f),
      maxSteeringAngle(0.4f),
      steeringSpeed(2.5f),
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
    // ENGINE FORCE
    float engineForce = 0.0f;
    if (moveForward) {
        engineForce = enginePower;
    }

    // BRAKING/REVERSE FORCE
    float brakeForce = 0.0f;
    if (moveBackward) {
        if (currentSpeed > 0) {
            brakeForce = brakePower; // Brake if moving forward
        } else {
            engineForce = -enginePower * 0.6f; // Reverse (slower than forward)
        }
    }

    // DRAG FORCE (air resistance) - reduced for higher speeds
    float dragForce = dragCoefficient * fabsf(currentSpeed) * currentSpeed;

    // ROLLING RESISTANCE - reduced
    float rollingForce = rollingResistance * currentSpeed;

    // NET FORCE
    float netForce = engineForce - brakeForce - dragForce - rollingForce;

    // ACCELERATION
    float acceleration = netForce;
    currentSpeed += acceleration * deltaTime;

    // Clamp speed - faster maximums
    if (moveBackward && currentSpeed < 0) {
        currentSpeed = (std::max)(-maxSpeed * 0.7f, currentSpeed); // Faster reverse
    } else {
        currentSpeed = (std::min)(maxSpeed, (std::max)(-maxSpeed * 0.7f, currentSpeed));
    }

    // STEERING EFFECTS - responsive even at higher speeds
    if (fabsf(currentSpeed) > 0.5f && fabsf(steeringAngle) > 0.01f) {
        // Speed-dependent steering (less sensitive at high speeds)
        float speedFactor = 1.0f - (fabsf(currentSpeed) / maxSpeed) * 0.5f;
        float turnRate = steeringAngle * currentSpeed * 0.4f * speedFactor;
        rotation += turnRate * deltaTime;
    }

    // VELOCITY VECTOR
    D3DXVECTOR2 forward(sinf(rotation), -cosf(rotation));
    velocity = forward * currentSpeed;

    // POSITION UPDATE
    position += velocity * deltaTime;
}

void RaceCar::UpdateSteering(float deltaTime, bool turnLeft, bool turnRight) {
    // Update steering angle
    float steeringInput = 0.0f;
    if (turnLeft) steeringInput -= 1.0f;
    if (turnRight) steeringInput += 1.0f;

    steeringAngle += steeringInput * steeringSpeed * deltaTime;

    // Return to center when no input
    if (steeringInput == 0.0f) {
        float returnSpeed = steeringSpeed * 1.5f; // Faster return to center
        if (steeringAngle > 0) {
            steeringAngle -= returnSpeed * deltaTime;
            steeringAngle = (std::max)(0.0f, steeringAngle);
        } else if (steeringAngle < 0) {
            steeringAngle += returnSpeed * deltaTime;
            steeringAngle = (std::min)(0.0f, steeringAngle);
        }
    }

    // Clamp steering angle
    steeringAngle = (std::max)(-maxSteeringAngle, (std::min)(steeringAngle, maxSteeringAngle));
}

void RaceCar::UpdateAnimation(float deltaTime, bool turnLeft, bool turnRight) {
    // Faster animation that works at higher speeds
    float animSpeedThreshold = 3.0f; // Lower threshold for faster response

    if (turnLeft && fabsf(currentSpeed) > animSpeedThreshold) {
        frameTimer += deltaTime;
        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f;
            currentFrame = (currentFrame + 1) % 3 + 1;
        }
    } else if (turnRight && fabsf(currentSpeed) > animSpeedThreshold) {
        frameTimer += deltaTime;
        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f;
            currentFrame = (currentFrame + 1) % 3 + 4;
        }
    } else {
        // Smooth transition back to center frame
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