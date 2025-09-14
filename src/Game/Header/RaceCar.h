#pragma once
#include <d3dx9math.h>
#include <d3dx9.h>

class RaceCar {
private:
    // Physics
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

    // Animation
    int   currentFrame;
    int   maxFrame;
    int   frameWidth;
    int   frameHeight;
    float frameTimer;
    float frameDelay;

    // Rendering
    D3DXVECTOR2 scale;
    int screenWidth;
    int screenHeight;

public:
    RaceCar(D3DXVECTOR2 startPos, int screenW, int screenH);

    // Getters / Setters
    D3DXVECTOR2 GetPosition() const { return position; }
    void SetPosition(const D3DXVECTOR2& pos) { position = pos; }

    float GetRotation() const { return rotation; }
    void  SetRotation(float rot) { rotation = rot; }

    D3DXVECTOR2 GetVelocity() const { return velocity; }
    void SetVelocity(const D3DXVECTOR2& vel) { velocity = vel; }

    int GetWidth()  const { return static_cast<int>(frameWidth  * scale.x); }
    int GetHeight() const { return static_cast<int>(frameHeight * scale.y); }

    // Collision
    RECT GetBoundingBox() const;
    bool CarRectCollision(const RECT& other) const;
    D3DXVECTOR2 GetCorner(int cornerIndex) const;

    // --- Update/Render ---
    void Update(float deltaTime, bool moveForward, bool moveBackward,
                bool turnLeft, bool turnRight);
    void Render(LPD3DXSPRITE spriteBrush, LPDIRECT3DTEXTURE9 texture);

private:
    void ApplyPhysics(float deltaTime, bool moveForward, bool moveBackward,
                      bool turnLeft, bool turnRight);
    void UpdateSteering(float deltaTime, bool turnLeft, bool turnRight);
    void UpdateAnimation(float deltaTime, bool turnLeft, bool turnRight);
    void ClampToScreen();
};