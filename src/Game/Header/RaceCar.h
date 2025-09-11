#pragma once
#include <d3dx9math.h>
#include <d3dx9.h>

class RaceCar {
private:
    // --- Physics ---
    D3DXVECTOR2 position;
    D3DXVECTOR2 velocity;
    float rotation;
    float currentSpeed;

    // Physics params
    float enginePower;
    float maxSpeed;
    float accelerationRate;
    float decelerationRate;
    float brakePower;
    float dragCoefficient;
    float rollingResistance;
    float steeringAngle;
    float maxSteeringAngle;
    float steeringSpeed;

    // --- Animation ---
    int currentFrame;
    int maxFrame;
    int frameWidth;
    int frameHeight;
    float frameTimer;
    float frameDelay;

public:
    RaceCar(D3DXVECTOR2 startPos);

    // Getters / Setters
    D3DXVECTOR2 GetPosition() const { return position; }
    void SetPosition(const D3DXVECTOR2& pos) { position = pos; }

    float GetRotation() const { return rotation; }
    void SetRotation(float rot) { rotation = rot; }

    D3DXVECTOR2 GetVelocity() const { return velocity; }
    void SetVelocity(const D3DXVECTOR2& vel) { velocity = vel; }

    int GetWidth() const { return frameWidth; }
    int GetHeight() const { return frameHeight; }

    RECT GetBoundingBox() const;

    // Update/Render
    void Update(float deltaTime, bool moveForward, bool moveBackward, bool turnLeft, bool turnRight);
    void Render(LPD3DXSPRITE spriteBrush, LPDIRECT3DTEXTURE9 texture);

private:
    void ApplyPhysics(float deltaTime, bool moveForward, bool moveBackward, bool turnLeft, bool turnRight);
    void UpdateSteering(float deltaTime, bool turnLeft, bool turnRight);
    void UpdateAnimation(float deltaTime, bool turnLeft, bool turnRight);
};