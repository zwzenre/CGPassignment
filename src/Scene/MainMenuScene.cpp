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

    // Load textures
    if (FAILED(D3DXCreateTextureFromFile(this->d3dDevice, "assets/menu/menuBackground.png", &this->menuBackground))) {
        MessageBox(nullptr, "Failed to load menu background", "Error", MB_OK);
    }
    if (FAILED(D3DXCreateTextureFromFile(this->d3dDevice, "assets/menu/playButton.png", &this->playButtonTex))) {
        MessageBox(nullptr, "Failed to load play button", "Error", MB_OK);
    }
    if (FAILED(D3DXCreateTextureFromFile(this->d3dDevice, "assets/menu/playButtonHover.png", &this->playButtonHoverTex))) {
        MessageBox(nullptr, "Failed to load play button hover", "Error", MB_OK);
    }

    this->CreateFontInterface();
    this->CreateTitleFont();
}

void MainMenuScene::Update(float deltaTime) {
    // Main menu update logic will go here
    // Handle button hover, clicks, etc.
}

void MainMenuScene::Render(LPD3DXSPRITE sprite) {
    if (!this->menuBackground) return;

    sprite->Begin(D3DXSPRITE_ALPHABLEND);

    // Render background
    D3DXVECTOR3 bgPos(0, 0, 0);
    sprite->Draw(this->menuBackground, nullptr, nullptr, &bgPos, D3DCOLOR_XRGB(255, 255, 255));

    // Render buttons and text will go here

    sprite->End();
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