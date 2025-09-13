#include "Header/Level1.h"
#include "../Game/Header/Collectible.h"
#include "../Game/Header/Obstacle.h"
#include <string>
#include <windows.h>
#include <d3dx9.h>
#include <sstream>
#include <algorithm>

Level1::Level1()
        : device(nullptr), carTexture(nullptr),
          input(nullptr), sound(nullptr), playerCar(nullptr),
          gameCursor(nullptr), fontBrush(nullptr),
          screenWidth(1920), screenHeight(1080),
          goToEndScene(false),
          physicsManager(screenWidth, screenHeight),
          collectedCoinCount(0), collisionCount(0) {}

Level1::~Level1() {
    Quit();
}

void Level1::Init(IDirect3DDevice9 *device, InputManager *inputMgr, SoundManager *soundMgr,
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

    playerCar = new RaceCar(D3DXVECTOR2(100, 100), screenWidth, screenHeight);

    gameCursor = input->GetCursor();

    CreateFont();

    collectibles.push_back(new Collectible(D3DXVECTOR2(200, 200), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(500, 150), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(800, 300), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(400, 500), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(100, 700), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1200, 250), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1500, 400), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1700, 100), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1300, 700), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(900, 900), "assets/coin.png"));

    for (auto &collectible : collectibles) {
        collectible->Initialize(device);
    }

    obstacles.push_back(new Obstacle(D3DXVECTOR2(300, 300), D3DXVECTOR2(64, 64), "assets/box.jpg"));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(700, 250), D3DXVECTOR2(64, 64), "assets/box.jpg"));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(150, 550), D3DXVECTOR2(64, 64), "assets/box.jpg"));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(900, 600), D3DXVECTOR2(64, 64), "assets/box.jpg"));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(600, 800), D3DXVECTOR2(64, 64), "assets/box.jpg"));

    for (auto &obstacle : obstacles) {
        obstacle->Initialize(device);
    }

    levelTimer.Start();
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
    physicsManager.ConstrainToBounds(*playerCar);

    for (auto &collectible: collectibles) {
        if (!collectible->IsCollected()) {
            collectible->Update(deltaTime);
        }
    }

    for (auto &obstacle: obstacles) {
        obstacle->Update(deltaTime);
    }

    CheckCollectibleCollisions();
    CheckObstacleCollisions();

    if (input->IsKeyDown(DIK_RETURN)) {
        goToEndScene = true;
    }

    bool allCoinsCollected = true;
    for (const auto& collectible : collectibles) {
        if (!collectible->IsCollected()) {
            allCoinsCollected = false;
            break;
        }
    }

    if (allCoinsCollected) {
        goToEndScene = true;
    }

    if (goToEndScene && levelTimer.IsRunning()) {
        levelTimer.Stop();
    }
}

void Level1::Render(LPD3DXSPRITE sprite) {
    if (!carTexture || !playerCar) return;

    for (auto &obstacle: obstacles) {
        obstacle->Render(sprite);
    }

    for (auto &collectible: collectibles) {
        if (!collectible->IsCollected()) {
            collectible->Render(sprite);
        }
    }

    playerCar->Render(sprite, carTexture);
    DrawUI(sprite);
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

    for (auto &collectible: collectibles) {
        delete collectible;
    }
    collectibles.clear();

    for (auto &obstacle: obstacles) {
        delete obstacle;
    }
    obstacles.clear();

    CleanupFont();
}

void Level1::CreateFont() {
    if (device) {
        D3DXCreateFont(device, 18, 0, FW_BOLD, 1, false,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);

        D3DXCreateFont(device, 36, 0, FW_BOLD, 1, false,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_DONTCARE, "Consolas", &timerFont);
    }
}

void Level1::CleanupFont() {
    if (fontBrush) {
        fontBrush->Release();
        fontBrush = nullptr;
    }

    if (timerFont) {
        timerFont->Release();
        timerFont = nullptr;
    }
}

void Level1::CheckCollectibleCollisions() {
    if (!playerCar) return;

    std::vector<Collectible*> collectedThisFrame;

    for (Collectible *collectible: collectibles) {
        if (!collectible->IsCollected()) {
            if (playerCar->CarRectCollision(collectible->GetBoundingBox())) {
                HandleCollectibleCollision(collectible);
                collectedThisFrame.push_back(collectible);
            }
        }
    }

    for (Collectible* collectedItem : collectedThisFrame) {
        collectibles.erase(std::remove(collectibles.begin(), collectibles.end(), collectedItem), collectibles.end());
        delete collectedItem;
    }
}

void Level1::CheckObstacleCollisions() {
    if (!playerCar) return;

    if (physicsManager.CheckCarObstacleCollisions(*playerCar, obstacles)) {
        sound->PlayBoxSound(0.0f);
        collisionCount++;
    }

    obstacles.erase(
            std::remove_if(obstacles.begin(), obstacles.end(), [](Obstacle *o) {
                if (o->IsDisappearing()) {
                    delete o;
                    return true;
                }
                return false;
            }),
            obstacles.end());
}

void Level1::HandleCollectibleCollision(Collectible *collectible) {
    if (!collectible->IsCollected()) {
        collectible->Collect();
        collectedCoinCount++;
        sound->PlayCoinSound(0.0f);
    }
}

void Level1::DrawUI(LPD3DXSPRITE sprite) {
    if (!timerFont || !fontBrush) return;

    std::string timerText = "Time: " + levelTimer.GetTimer();
    RECT timerRect = {screenWidth - 350, 20, screenWidth - 20, 100};
    timerFont->DrawTextA(sprite, timerText.c_str(), -1, &timerRect, DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 0));

    std::stringstream ssCoins;
    ssCoins << "Coins: " << collectedCoinCount << "/" << TOTAL_COINS_FOR_STAR;
    RECT coinsRect = {20, 20, 250, 100};
    fontBrush->DrawTextA(sprite, ssCoins.str().c_str(), -1, &coinsRect, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 255, 0));

    std::stringstream ssCollisions;
    ssCollisions << "Collisions: " << collisionCount;
    RECT collisionsRect = {20, 100, 250, 180};
    fontBrush->DrawTextA(sprite, ssCollisions.str().c_str(), -1, &collisionsRect, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 100, 0));
}