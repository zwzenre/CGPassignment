#include "Header/MainMenuScene.h"
#include <windows.h>
#include <d3dx9.h>

MainMenuScene::MainMenuScene()
    : d3dDevice(nullptr), inputManager(nullptr), soundManager(nullptr),
      windowWidth(1280), windowHeight(720), hWnd(nullptr),
      menuBackground(nullptr), playButtonTex(nullptr), playButtonHoverTex(nullptr),
      fontBrush(nullptr), titleFont(nullptr) {}

MainMenuScene::~MainMenuScene() {
    Quit();
}

void MainMenuScene::Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr,
                        HWND hWnd, int screenWidth, int screenHeight) {
    this->d3dDevice = device;
    this->inputManager = inputMgr;
    this->soundManager = soundMgr;
    this->hWnd = hWnd;
    this->windowWidth = screenWidth;
    this->windowHeight = screenHeight;

    D3DDEVICE_CREATION_PARAMETERS params;
    device->GetCreationParameters(&params);
    hWnd = params.hFocusWindow;

    RECT rect;
    GetClientRect(hWnd, &rect);
    windowWidth = rect.right - rect.left;
    windowHeight = rect.bottom - rect.top;

    D3DXCreateTextureFromFile(d3dDevice, "assets/Mainmenu2.png", &menuBackground);
    D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_line.png", &playButtonTex);
    D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_depth_line.png", &playButtonHoverTex);

    int buttonWidth = 128;
    int buttonHeight = 128;

    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    int buttonOffsetX = 50;
    int buttonOffsetY = -10;

    playButtonRect.left   = centerX - buttonWidth/2 + buttonOffsetX;
    playButtonRect.top    = centerY - buttonHeight/2 + buttonOffsetY;
    playButtonRect.right  = playButtonRect.left + buttonWidth;
    playButtonRect.bottom = playButtonRect.top + buttonHeight;

    this->CreateFontInterface();
    this->CreateTitleFont();
}

void MainMenuScene::Update(float deltaTime) {
    // Main menu update logic will go here
    // Handle button hover, clicks, etc.
}

void MainMenuScene::Render(LPD3DXSPRITE spriteBrush) {
    if (!spriteBrush) return;

    if (menuBackground) {
        D3DSURFACE_DESC bgDesc;
        menuBackground->GetLevelDesc(0, &bgDesc);

        float scaleX = (float)windowWidth / bgDesc.Width;
        float scaleY = (float)windowHeight / bgDesc.Height;

        float scale = max(scaleX, scaleY);

        scale = 1.2f;

        D3DXVECTOR2 bgScale(scale, scale);

        float scaledWidth = bgDesc.Width scale;
        float scaledHeight = bgDesc.Height * scale;

        float offsetX = (windowWidth - scaledWidth) / 2.0f;
        float offsetY = (windowHeight - scaledHeight) / 2.0f;
        D3DXVECTOR2 bgPosition(offsetX, offsetY);

        D3DXMATRIX matBg;
        D3DXMatrixTransformation2D(&matBg, NULL, 0.0f, &bgScale, NULL, 0.0f, &bgPosition);

        spriteBrush->SetTransform(&matBg);
        D3DXVECTOR3 bgPos(0, 0, 0);
        spriteBrush->Draw(menuBackground, NULL, NULL, &bgPos, D3DCOLOR_XRGB(255, 255, 255));
    }

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);

    DrawButton(spriteBrush);
    DrawTitle(spriteBrush);
}

void MainMenuScene::Quit() {
    if (this->menuBackground) {
        this->menuBackground->Release();
        this->menuBackground = nullptr;
    }
    if (this->playButtonTex) {
        this->playButtonTex->Release();
        this->playButtonTex = nullptr;
    }
    if (this->playButtonHoverTex) {
        this->playButtonHoverTex->Release();
        this->playButtonHoverTex = nullptr;
    }
    this->CleanupFontInterface();
}

void MainMenuScene::CreateFontInterface() {
    if (this->d3dDevice) {
        D3DXCreateFont(this->d3dDevice, 25, 0, FW_NORMAL, 1, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, "Arial", &this->fontBrush);
    }
}

void MainMenuScene::CreateTitleFont() {
    if (this->d3dDevice) {
        D3DXCreateFont(this->d3dDevice, 48, 0, FW_BOLD, 1, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, "Arial", &this->titleFont);
    }
}

void MainMenuScene::CleanupFontInterface() {
    if (this->fontBrush) {
        this->fontBrush->Release();
        this->fontBrush = nullptr;
    }
    if (this->titleFont) {
        this->titleFont->Release();
        this->titleFont = nullptr;
    }
}