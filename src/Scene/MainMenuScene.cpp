#include "Header/MainMenuScene.h"
#include <windows.h>
#include <d3dx9.h>
#include <iostream>

MainMenuScene::MainMenuScene()
    : d3dDevice(nullptr), inputManager(nullptr), soundManager(nullptr),
      windowWidth(1280), windowHeight(720), hWnd(nullptr),
      menuBackground(nullptr), titleFont(nullptr), fontBrush(nullptr),
      playButtonHovered(false), playButtonPressed(false), playButtonClicked(false),
      playButtonTex(nullptr), playButtonHoverTex(nullptr) {
    ZeroMemory(&playButtonRect, sizeof(playButtonRect));
}

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

    // Load textures with error checking
    if (FAILED(D3DXCreateTextureFromFile(d3dDevice, "assets/Mainmenu2.png", &menuBackground))) {
        MessageBox(nullptr, "Failed to load Mainmenu2.png", "Error", MB_OK);
    }
    if (FAILED(D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_line.png", &playButtonTex))) {
        MessageBox(nullptr, "Failed to load button_round_line.png", "Error", MB_OK);
    }
    if (FAILED(D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_depth_line.png", &playButtonHoverTex))) {
        MessageBox(nullptr, "Failed to load button_round_depth_line.png", "Error", MB_OK);
    }

    // Calculate button position
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
    if (!inputManager) return;

    // Get mouse position
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(hWnd, &mousePos);

    // Check if mouse is over play button
    playButtonHovered = (mousePos.x >= playButtonRect.left &&
                        mousePos.x <= playButtonRect.right &&
                        mousePos.y >= playButtonRect.top &&
                        mousePos.y <= playButtonRect.bottom);

    // Check for mouse click
    if (playButtonHovered && inputManager->IsMouseButtonPressed(0)) {
        playButtonClicked = true;
    }
}

void MainMenuScene::Render(LPD3DXSPRITE spriteBrush) {
    if (!spriteBrush || !menuBackground) return;

    spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

    // Draw background with scaling
    D3DSURFACE_DESC bgDesc;
    menuBackground->GetLevelDesc(0, &bgDesc);

    float scaleX = (float)windowWidth / bgDesc.Width;
    float scaleY = (float)windowHeight / bgDesc.Height;
    float scale = max(scaleX, scaleY);
    scale = 1.2f; // Fixed scale factor

    D3DXVECTOR2 bgScale(scale, scale);
    float scaledWidth = bgDesc.Width * scale; // Fixed multiplication
    float scaledHeight = bgDesc.Height * scale;
    float offsetX = (windowWidth - scaledWidth) / 2.0f;
    float offsetY = (windowHeight - scaledHeight) / 2.0f;
    D3DXVECTOR2 bgPosition(offsetX, offsetY);

    D3DXMATRIX matBg;
    D3DXMatrixTransformation2D(&matBg, NULL, 0.0f, &bgScale, NULL, 0.0f, &bgPosition);
    spriteBrush->SetTransform(&matBg);

    D3DXVECTOR3 bgPos(0, 0, 0);
    spriteBrush->Draw(menuBackground, NULL, NULL, &bgPos, D3DCOLOR_XRGB(255, 255, 255));

    // Reset transform for other elements
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);

    // Draw button and title
    DrawButton(spriteBrush);
    DrawTitle(spriteBrush);

    spriteBrush->End();
}

void MainMenuScene::Quit() {
    CleanupTextures();
    CleanupFontInterface();
}

void MainMenuScene::CreateFontInterface() {
    if (d3dDevice) {
        D3DXCreateFont(d3dDevice, 25, 0, FW_NORMAL, 1, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
    }
}

void MainMenuScene::CreateTitleFont() {
    if (d3dDevice) {
        D3DXCreateFont(d3dDevice, 48, 0, FW_BOLD, 1, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, "Arial", &titleFont);
    }
}

void MainMenuScene::CleanupFontInterface() {
    if (fontBrush) {
        fontBrush->Release();
        fontBrush = nullptr;
    }
    if (titleFont) {
        titleFont->Release();
        titleFont = nullptr;
    }
}

void MainMenuScene::CleanupTextures() {
    if (menuBackground) {
        menuBackground->Release();
        menuBackground = nullptr;
    }
    if (playButtonTex) {
        playButtonTex->Release();
        playButtonTex = nullptr;
    }
    if (playButtonHoverTex) {
        playButtonHoverTex->Release();
        playButtonHoverTex = nullptr;
    }
}

void MainMenuScene::DrawTitle(LPD3DXSPRITE spriteBrush) {
    if (!titleFont) return;

    RECT titleRect;
    titleRect.left = windowWidth / 2 - 200;
    titleRect.top = 100;
    titleRect.right = windowWidth / 2 + 200;
    titleRect.bottom = 200;

    titleFont->DrawText(spriteBrush, reinterpret_cast<LPCSTR>(L"CAR RACING GAME"), -1, &titleRect,
                       DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
}

void MainMenuScene::DrawButton(LPD3DXSPRITE spriteBrush) {
    LPDIRECT3DTEXTURE9 buttonTexture = playButtonHovered ? playButtonHoverTex : playButtonTex;
    if (!buttonTexture) return;

    D3DXVECTOR3 buttonPos((float)playButtonRect.left, (float)playButtonRect.top, 0);
    spriteBrush->Draw(buttonTexture, NULL, NULL, &buttonPos, D3DCOLOR_XRGB(255, 255, 255));

    // Draw "PLAY" text on button
    if (fontBrush) {
        RECT textRect = playButtonRect;
        textRect.top += 40; // Center vertically
        fontBrush->DrawText(spriteBrush, reinterpret_cast<LPCSTR>(L"PLAY"), -1, &textRect,
                           DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
    }
}