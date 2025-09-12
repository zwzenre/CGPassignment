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
              HWND hWnd = nullptr, int screenWidth = 1920, int screenHeight = 1080) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE sprite) override;
    void Quit() override;

    bool WantsToGoToEndScene() const { return goToEndScene; }
    void ResetEndSceneRequest() { goToEndScene = false; }

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

    bool goToEndScene;

    void CreateFont();
    void CleanupFont();
};