#pragma once
#include "../../Manager/Header/InputManager.h"
#include "../../Manager/Header/SoundManager.h"
#include "../../Scene/Header/Scene.h"
#include "../../Game/Header/RaceCar.h"
#include <d3dx9.h>

class Level1 : public Scene {
public:
    Level1();
    ~Level1() override;

    void Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr,
              HWND hWnd = nullptr, int screenWidth = 1280, int screenHeight = 720) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE sprite) override;
    void Quit() override;

private:
    IDirect3DDevice9* device;
    IDirect3DTexture9* carTexture;
    InputManager* input;
    SoundManager* sound;
    RaceCar* playerCar;
    Cursor* gameCursor;
    LPD3DXFONT fontBrush;
    int screenWidth;
    int screenHeight;
    HWND hWnd;

    void CreateFont();
    void CleanupFont();
};