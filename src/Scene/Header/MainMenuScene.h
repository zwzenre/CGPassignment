#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Scene/Header/Scene.h"
#include "../../Manager/Header/InputManager.h"
#include "../../Manager/Header/SoundManager.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override;

    // Scene interface implementation
    void Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE spriteBrush) override;  // 注意：spriteBrush由SceneManager传入
    void Quit() override;

    bool IsPlayButtonClicked() const { return playButtonClicked; }
    void ResetPlayButton() { playButtonClicked = false; }

private:
    IDirect3DDevice9* d3dDevice;
    InputManager* inputManager;
    SoundManager* soundManager;

    int windowWidth;
    int windowHeight;
    HWND hWnd;

    LPDIRECT3DTEXTURE9 menuBackground;
    LPD3DXFONT titleFont;
    LPD3DXFONT fontBrush;

    RECT playButtonRect;
    bool playButtonHovered;
    bool playButtonPressed;
    bool playButtonClicked;
    LPDIRECT3DTEXTURE9 playButtonTex;
    LPDIRECT3DTEXTURE9 playButtonHoverTex;

    void CreateFontInterface();
    void CreateTitleFont();
    void DrawTitle(LPD3DXSPRITE spriteBrush);
    void DrawButton(LPD3DXSPRITE spriteBrush);
    void CleanupTextures();
    void CleanupFonts();
};