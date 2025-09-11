#include "Header/Level1.h"
#include <string>
#include <windows.h>
#include <d3dx9.h>
#include <sstream>

Level1::Level1()
    : device(nullptr), carTexture(nullptr),
      input(nullptr), sound(nullptr), playerCar(nullptr),
      gameCursor(nullptr), fontBrush(nullptr), screenWidth(1280), screenHeight(720) {}

Level1::~Level1() {
    Quit();
}

void Level1::Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr,
                 HWND hWnd, int screenWidth, int screenHeight) {
    this->device = device;
    this->input = inputMgr;
    this->sound = soundMgr;
    this->hWnd = hWnd;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // Load car texture
    if (FAILED(D3DXCreateTextureFromFile(device, "assets/car.png", &carTexture))) {
        MessageBox(nullptr, "Failed to load car.png", "Error", MB_OK);
    }

    // Initialize game objects
    playerCar = new RaceCar(D3DXVECTOR2(screenWidth / 2, screenHeight / 2));

    // Use the cursor from InputManager instead of creating our own
    gameCursor = input->GetCursor();  // Get the existing cursor from InputManager

    CreateFont();
}

void Level1::Update(float deltaTime) {
    if (!input || !playerCar) return;

    input->SetCursorVisible(true);
    // Get input for both WASD and Arrow keys
    bool moveForward = input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_W);
    bool moveBackward = input->IsKeyDown(DIK_DOWN) || input->IsKeyDown(DIK_S);
    bool turnLeft = input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_A);
    bool turnRight = input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_D);

    playerCar->Update(deltaTime, moveForward, moveBackward, turnLeft, turnRight);

    D3DXVECTOR2 pos = playerCar->GetPosition();
    if (pos.x < 0) pos.x = screenWidth;
    if (pos.x > screenWidth) pos.x = 0;
    if (pos.y < 0) pos.y = screenHeight;
    if (pos.y > screenHeight) pos.y = 0;
    playerCar->SetPosition(pos);
}

void Level1::Render(LPD3DXSPRITE sprite) {
    if (!carTexture || !playerCar) return;

    // Render the race car
    playerCar->Render(sprite, carTexture);
}

void Level1::Quit() {
    if (carTexture) {
        carTexture->Release();
        carTexture = nullptr;
    }

    gameCursor = nullptr;

    if (playerCar) {
        delete playerCar;
        playerCar = nullptr;
    }

    CleanupFont();
}

void Level1::CreateFont() {
    if (device) {
        D3DXCreateFont(device, 18, 0, FW_BOLD, 1, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
    }
}

void Level1::CleanupFont() {
    if (fontBrush) {
        fontBrush->Release();
        fontBrush = nullptr;
    }
}