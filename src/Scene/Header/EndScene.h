#pragma once
#include "../Scene//Header/Scene.h"
#include <string>

class EndScene : public Scene {
private:
    IDirect3DDevice9* device = nullptr;
    InputManager* inputManager = nullptr;
    SoundManager* soundManager = nullptr;
    HWND hWnd = nullptr;

    int screenWidth = 1920;
    int screenHeight = 1080;

    // Textures
    LPDIRECT3DTEXTURE9 bgTexture = nullptr;        // EndBg.png
    LPDIRECT3DTEXTURE9 resultTexture = nullptr;    // resultScreen.png
    LPDIRECT3DTEXTURE9 restartBtnTexture = nullptr; // restartBtn.png
    LPDIRECT3DTEXTURE9 exitBtnTexture = nullptr;    // exitBtn.png
    LPDIRECT3DTEXTURE9 starCompleteTexture = nullptr; // complete_star.png
    LPDIRECT3DTEXTURE9 starIncompleteTexture = nullptr; // incomplete_star.png

    LPD3DXFONT font = nullptr;

    std::string message = "Thanks for playing!";

    // State
    bool restartSelected = false;
    bool exitRequested = false;

    // Button rects
    RECT restartBtnRect;
    RECT exitBtnRect;

    int totalStars = 0;

public:
    EndScene() = default;
    ~EndScene() override = default;

    void Init(IDirect3DDevice9* device,
              InputManager* inputMgr,
              SoundManager* soundMgr,
              HWND hWnd,
              int screenWidth,
              int screenHeight) override;

    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE spriteBrush) override;
    void Quit() override;

    bool IsRestartSelected() const { return restartSelected; }
    bool IsExitSelected() const { return exitRequested; }
    void ResetSelections() { restartSelected = false; exitRequested = false; }

    void SetTotalStars(int stars) { totalStars = stars; }
};