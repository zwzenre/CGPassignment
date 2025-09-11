#pragma once
#include "../../Manager/Header/InputManager.h"
#include "../../Manager/Header/SoundManager.h"
#include "../../Scene/Header/Scene.h"
#include <d3dx9.h>

class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override;

    void Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr,
              HWND hWnd = nullptr, int screenWidth = 1280, int screenHeight = 720) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE sprite) override;
    void Quit() override;

private:
    IDirect3DDevice9* d3dDevice;
    InputManager* inputManager;
    SoundManager* soundManager;
    int windowWidth;
    int windowHeight;
    HWND hWnd;

    LPDIRECT3DTEXTURE9 menuBackground;
    LPDIRECT3DTEXTURE9 playButtonTex;
    LPDIRECT3DTEXTURE9 playButtonHoverTex;
    LPD3DXFONT fontBrush;
    LPD3DXFONT titleFont;

    void CreateFontInterface();
    void CreateTitleFont();
    void CleanupFontInterface();
};