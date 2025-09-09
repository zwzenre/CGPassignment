#include "Header/Level1.h"
#include <string>
#include <windows.h>
#include <d3dx9.h>

Level1::Level1()
    : device(nullptr), carTexture(nullptr), input(nullptr),
      sound(nullptr), playerCar(nullptr), fontBrush(nullptr) {}

Level1::~Level1() {
    Quit();
}

void Level1::Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr) {
    this->device = device;
    input = inputMgr;
    sound = soundMgr;

    if (FAILED(D3DXCreateTextureFromFile(device, "assets/car.png", &carTexture))) {
        MessageBox(nullptr, "Failed to load car.png", "Error", MB_OK);
    }

    playerCar = new RaceCar(D3DXVECTOR2(400, 300));
    CreateFont();
}

void Level1::Update(float deltaTime) {
    if (!input || !playerCar) return;

    // Get input for both WASD and Arrow keys
    bool moveForward = input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_W);
    bool moveBackward = input->IsKeyDown(DIK_DOWN) || input->IsKeyDown(DIK_S);
    bool turnLeft = input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_A);
    bool turnRight = input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_D);

    // Update car physics with both control schemes
    playerCar->Update(deltaTime, moveForward, moveBackward, turnLeft, turnRight);

    // Handle screen boundaries
    D3DXVECTOR2 pos = playerCar->GetPosition();
    if (pos.x < 0) pos.x = 1280;
    if (pos.x > 1280) pos.x = 0;
    if (pos.y < 0) pos.y = 720;
    if (pos.y > 720) pos.y = 0;
    playerCar->SetPosition(pos);
}

void Level1::Render(LPD3DXSPRITE sprite) {
    if (!carTexture || !playerCar) return;

    sprite->Begin(D3DXSPRITE_ALPHABLEND);
    playerCar->Render(sprite, carTexture);

    RECT textRect;
    textRect.left = 600;
    textRect.top = 50;
    textRect.right = 800;
    textRect.bottom = 100;

    fontBrush->DrawText(sprite, gameTimer.GetTimer().c_str(), -1, &textRect, DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
    sprite->End();
}

void Level1::Quit() {
    if (carTexture) {
        carTexture->Release();
        carTexture = nullptr;
    }
    if (playerCar) {
        delete playerCar;
        playerCar = nullptr;
    }
    CleanupFont();
}

void Level1::CreateFont() {
    if (device) {
        D3DXCreateFont(device, 25, 0, 0, 1, false,
                      DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
    }
}

void Level1::CleanupFont() {
    if (fontBrush) {
        fontBrush->Release();
        fontBrush = nullptr;
    }
}