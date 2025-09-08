#pragma once
#include <d3dx9math.h>
#include <d3dx9.h>

class RaceCar {
private:
    D3DXVECTOR2 position;
    D3DXVECTOR2 velocity;
    float rotation;

    // Realistic physics parameters
    float enginePower;
    float maxSpeed;
    float currentSpeed;
    float accelerationRate;
    float decelerationRate;
    float brakePower;
    float dragCoefficient;
    float rollingResistance;

    // Steering parameters
    float steeringAngle;
    float maxSteeringAngle;
    float steeringSpeed;
    float wheelBase; // Distance between front and rear wheels

    // Tire parameters
    float tireGrip;
    float weightTransfer;

    // Animation
    int currentFrame;
    int maxFrame;
    int frameWidth;
    int frameHeight;
    float frameTimer;
    float frameDelay;

public:
    RaceCar(D3DXVECTOR2 startPos);

    void Update(float deltaTime, bool moveForward, bool moveBackward, bool turnLeft, bool turnRight);
    void Render(LPD3DXSPRITE spriteBrush, LPDIRECT3DTEXTURE9 texture);

    D3DXVECTOR2 GetVelocity() const { return velocity; }
    float GetRotation() const { return rotation; }
    D3DXVECTOR2 GetPosition() const { return position; }
    float GetWidth() const { return static_cast<float>(frameWidth); }
    float GetHeight() const { return static_cast<float>(frameHeight); }

    void SetPosition(const D3DXVECTOR2& pos) { position = pos; }
    void SetVelocity(const D3DXVECTOR2& vel) { velocity = vel; }
    void SetRotation(float rot) { rotation = rot; }

    RECT GetBoundingBox() const {
        RECT r;
        r.left   = static_cast<LONG>(position.x);
        r.top    = static_cast<LONG>(position.y);
        r.right  = static_cast<LONG>(position.x + frameWidth);
        r.bottom = static_cast<LONG>(position.y + frameHeight);
        return r;
    }

private:
    void ApplyPhysics(float deltaTime, bool accelerate, bool brake, bool turnLeft, bool turnRight);
    void UpdateSteering(float deltaTime, bool turnLeft, bool turnRight);
    void UpdateAnimation(float deltaTime, bool turnLeft, bool turnRight);
};