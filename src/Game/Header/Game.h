#pragma once
#include "../../Timer/Header/Timer.h"
#include "../../Manager/Header/SoundManager.h"
#include "../../Manager/Header/PhysicsManager.h"
#include "../../Manager/Header/InputManager.h"
#include "RaceCar.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifndef GAME_GAME_H
#define GAME_GAME_H

class Game {
private:
    Timer timer;
    SoundManager soundManager;
    PhysicsManager physicsManager;
    InputManager inputManager;

    int level = 1;
    HRESULT hr;

    LPD3DXSPRITE spriteBrush = nullptr;
    LPD3DXFONT fontBrush = nullptr;
    LPD3DXLINE lineBrush = nullptr;
    D3DPRESENT_PARAMETERS d3dPP {};
    IDirect3DDevice9* d3dDevice = nullptr;

    LPDIRECT3DTEXTURE9 carTexture = nullptr;

    RaceCar player;

    static constexpr int TARGET_FPS = 120;

public:
    Game();
    ~Game();

    void Initialization(IDirect3DDevice9* device, HWND hWnd);
    void Update();
    void Render();
    void Quit();
};

#endif