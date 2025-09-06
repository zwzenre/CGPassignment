#pragma once
#include <string>
#include <d3dx9math.h>

#ifndef GAME_RACECAR_H
#define GAME_RACECAR_H


class RaceCar {
private:
    D3DXVECTOR2  position;
    float maxSpeed;
    float currentSpeed;
    float direction;
    D3DXVECTOR2 velocity;
    float acceleration;
    float brakePower;
    float friction;
    std::string sprite;

public:
    RaceCar(D3DXVECTOR2 startPos);

    void render();
    void update();
    void setSpeed();
    void getSpeed();
    void accelerate();
    void brake();
    void turn();

    D3DXVECTOR2 getPlayerPosition();

};









#endif //GAME_RACECAR_H