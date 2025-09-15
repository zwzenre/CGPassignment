#include "Header/Level1.h"
#include "../Game/Header/Collectible.h"
#include "../Game/Header/Obstacle.h"
#include <string>
#include <windows.h>
#include <d3dx9.h>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iostream>

Level1::Level1()
        : device(nullptr), carTexture(nullptr),
          input(nullptr), sound(nullptr), playerCar(nullptr), levelBg(nullptr),
          gameCursor(nullptr), fontBrush(nullptr),
          screenWidth(1920), screenHeight(1080),
          goToEndScene(false), wasMovingLastFrame(false),
          physicsManager(screenWidth, screenHeight),
          collectedCoinCount(0), collisionCount(0),
          countdownDuration(120.0f),
          currentCountdownTime(120.0f) {}

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

    if (FAILED(D3DXCreateTextureFromFile(device, "assets/rock_road.png", &levelBg))) {
        MessageBox(nullptr, "Failed to load rock_road.png", "Error", MB_OK);
    }

    playerCar = new RaceCar(D3DXVECTOR2(1280, 720), screenWidth, screenHeight, 100.0f);

    gameCursor = input->GetCursor();

    CreateFont();

    collectibles.push_back(new Collectible(D3DXVECTOR2(200, 400), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(500, 150), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(800, 300), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(400, 500), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(100, 700), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1200, 250), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1500, 400), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1700, 100), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(1400, 700), "assets/coin.png"));
    collectibles.push_back(new Collectible(D3DXVECTOR2(900, 900), "assets/coin.png"));

    for (auto &collectible : collectibles) {
        collectible->Initialize(device);
    }

    obstacles.push_back(new Obstacle(D3DXVECTOR2(100, 100), D3DXVECTOR2(72, 72), "assets/box.jpg", 50.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(200, 120), D3DXVECTOR2(64, 64), "assets/box.jpg", 70.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(150, 250), D3DXVECTOR2(80, 80), "assets/box.jpg", 120.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(280, 200), D3DXVECTOR2(56, 56), "assets/box.jpg", 40.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(600, 80), D3DXVECTOR2(64, 64), "assets/box.jpg", 80.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(700, 150), D3DXVECTOR2(72, 72), "assets/box.jpg", 100.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(850, 100), D3DXVECTOR2(56, 56), "assets/box.jpg", 60.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1500, 180), D3DXVECTOR2(80, 80), "assets/box.jpg", 150.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1650, 250), D3DXVECTOR2(64, 64), "assets/box.jpg", 90.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1400, 300), D3DXVECTOR2(72, 72), "assets/box.jpg", 110.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(100, 450), D3DXVECTOR2(56, 56), "assets/box.jpg", 45.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(250, 500), D3DXVECTOR2(64, 64), "assets/box.jpg", 75.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(180, 650), D3DXVECTOR2(72, 72), "assets/box.jpg", 95.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(400, 800), D3DXVECTOR2(80, 80), "assets/box.jpg", 130.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(500, 900), D3DXVECTOR2(64, 64), "assets/box.jpg", 85.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(300, 950), D3DXVECTOR2(72, 72), "assets/box.jpg", 105.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1200, 850), D3DXVECTOR2(64, 64), "assets/box.jpg", 70.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1400, 900), D3DXVECTOR2(56, 56), "assets/box.jpg", 55.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1600, 800), D3DXVECTOR2(80, 80), "assets/box.jpg", 140.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(900, 450), D3DXVECTOR2(64, 64), "assets/box.jpg", 80.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1050, 500), D3DXVECTOR2(72, 72), "assets/box.jpg", 100.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(750, 550), D3DXVECTOR2(56, 56), "assets/box.jpg", 65.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(1150, 700), D3DXVECTOR2(80, 80), "assets/box.jpg", 125.0f));
    obstacles.push_back(new Obstacle(D3DXVECTOR2(650, 700), D3DXVECTOR2(64, 64), "assets/box.jpg", 90.0f));

    for (auto &obstacle : obstacles) {
        obstacle->Initialize(device);
    }

    levelTimer.Start();
    currentCountdownTime = countdownDuration;

    totalStars = 0;
    timeRemaining = true;
    finalTime = 0.0f;
}

void Level1::Update(float deltaTime) {
    if (!input || !playerCar) return;

    input->SetCursorVisible(true);

    // Get input for both WASD and Arrow keys
    bool moveForward = input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_W);
    bool moveBackward = input->IsKeyDown(DIK_DOWN) || input->IsKeyDown(DIK_S);
    bool turnLeft = input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_A);
    bool turnRight = input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_D);

    bool isMovingNow = moveForward || moveBackward;

    if (isMovingNow && !wasMovingLastFrame) {
        sound->PlayCarSound(1.0f, 0.0f, 2.0f);
    } else if (!isMovingNow && wasMovingLastFrame) {
        sound->StopCarSound();
    }

    wasMovingLastFrame = isMovingNow;

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

    if (levelTimer.IsRunning()) {
        currentCountdownTime -= deltaTime;
        if (currentCountdownTime <= 0.0f) {
            currentCountdownTime = 0.0f;
            timeRemaining = false;
        }
    }

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

    bool levelShouldEnd = allCoinsCollected || !timeRemaining || input->IsKeyDown(DIK_RETURN);

    if (levelShouldEnd && !goToEndScene) {
        if (levelTimer.IsRunning()) {
            levelTimer.Stop();
            finalTime = countdownDuration - currentCountdownTime;
        }
        CalculateStars();
        goToEndScene = true;
    }
}

void Level1::Render(LPD3DXSPRITE sprite) {
    if (!carTexture || !playerCar) return;

    if (levelBg) {
        D3DSURFACE_DESC bgDesc;
        levelBg->GetLevelDesc(0, &bgDesc);

        float scaleX = (float)screenWidth / bgDesc.Width;
        float scaleY = (float)screenHeight / bgDesc.Height;

        D3DXVECTOR2 scaling(scaleX, scaleY);
        D3DXVECTOR2 position(0.0f, 0.0f);

        D3DXMATRIX mat;
        D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaling, NULL, 0.0f, &position);
        sprite->SetTransform(&mat);

        sprite->Draw(levelBg, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

        D3DXMATRIX identity;
        D3DXMatrixIdentity(&identity);
        sprite->SetTransform(&identity);
    }

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
    if (levelBg) {
        levelBg->Release();
        levelBg = nullptr;
    }

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
        D3DXCreateFont(device, 36, 0, FW_BOLD, 1, false,
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

void Level1::CalculateStars() {
    if (!timeRemaining) {
        totalStars = 0;
    } else if (collisionCount <= COLLISION_COUNT_FOR_THREE_STARS) {
        totalStars = 3;
    } else if (collisionCount <= COLLISION_COUNT_FOR_TWO_STARS) {
        totalStars = 2;
    } else {
        totalStars = 1;
    }

    std::cout << "Level End Results -" << std::endl;
    std::cout << "  Coins Collected: " << collectedCoinCount << "/" << TOTAL_COINS_FOR_STAR << std::endl;
    std::cout << "  Collisions: " << collisionCount << std::endl;

    if (finalTime < 120.0f) {
        std::cout << "  Final Time: " << std::fixed << std::setprecision(2) << finalTime << "s" << std::endl;
    } else {
        std::cout << "  Final Time: DNF" << std::endl;
    }
    std::cout << "  Total Stars: " << totalStars << std::endl;
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

    float remainingTime = fmax(0.0f, currentCountdownTime);
    int totalSeconds = static_cast<int>(remainingTime);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::stringstream ssTimer;
    ssTimer << "Time Left: " << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;

    RECT timerRect = {screenWidth - 350, 20, screenWidth - 20, 50};
    timerFont->DrawTextA(sprite, ssTimer.str().c_str(), -1, &timerRect, DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 0));

    std::stringstream ssCoins;
    ssCoins << "Coins: " << collectedCoinCount << "/" << TOTAL_COINS_FOR_STAR;
    RECT coinsRect = {20, 10, 250, 50};
    fontBrush->DrawTextA(sprite, ssCoins.str().c_str(), -1, &coinsRect, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 255, 0));

    std::stringstream ssCollisions;
    ssCollisions << "Box Broken: " << collisionCount;
    RECT collisionsRect = {20, 30, 250, 100};
    fontBrush->DrawTextA(sprite, ssCollisions.str().c_str(), -1, &collisionsRect, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 100, 0));
}