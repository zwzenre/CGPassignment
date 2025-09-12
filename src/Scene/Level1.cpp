#include "Header/Level1.h"
#include "../Game/Header/Collectible.h"
#include "../Game/Header/Obstacle.h"
#include <string>
#include <windows.h>
#include <d3dx9.h>
#include <sstream>

Level1::Level1()
        : device(nullptr), carTexture(nullptr),
          input(nullptr), sound(nullptr), playerCar(nullptr),
          gameCursor(nullptr), fontBrush(nullptr),
          screenWidth(1920), screenHeight(1080),
          goToEndScene(false) {}

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

    Collectible *coin = new Collectible(D3DXVECTOR2(500, 200), "assets/coin.png");
    coin->Initialize(device);
    collectibles.push_back(coin);

    Obstacle *box = new Obstacle(D3DXVECTOR2(300, 300), D3DXVECTOR2(64, 64), "assets/box.jpg");
    box->Initialize(device);
    obstacles.push_back(box);
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

    for (auto &collectible: collectibles) {
        if (!collectible->IsCollected()) {
            collectible->Update(deltaTime);
        }
    }

    for (auto &obstacle: obstacles) {
        obstacle->Update(deltaTime);
    }

    CheckCollisions();

    if (input->IsKeyDown(DIK_RETURN)) {
        goToEndScene = true;
    }
}

void Level1::Render(LPD3DXSPRITE sprite) {
    if (!carTexture || !playerCar) return;
    playerCar->Render(sprite, carTexture);

    for (auto &obstacle: obstacles) {
        obstacle->Render(sprite);
    }

    for (auto &collectible: collectibles) {
        if (!collectible->IsCollected()) {
            collectible->Render(sprite);
        }
    }
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
    }
}

void Level1::CleanupFont() {
    if (fontBrush) {
        fontBrush->Release();
        fontBrush = nullptr;
    }
}

void Level1::CheckCollisions() {
    if (!playerCar) return;
    RECT playerBox = playerCar->GetBoundingBox();

    for (auto &collectible: collectibles) {
        if (!collectible->IsCollected()) {
            RECT collectibleBox = collectible->GetBoundingBox();
            if (IntersectRect(nullptr, &playerBox, &collectibleBox)) {
                HandleCollectibleCollision(collectible);
            }
        }
    }

    collectibles.erase(
            std::remove_if(collectibles.begin(), collectibles.end(), [](Collectible *c) {
                if (c->IsCollected()) {
                    delete c;
                    return true;
                }
                return false;
            }),
            collectibles.end());


    for (auto &obstacle: obstacles) {
        RECT obstacleBox = obstacle->GetBoundingBox();
        if (IntersectRect(nullptr, &playerBox, &obstacleBox)) {
            HandleObstacleCollision(obstacle);
        }
    }
}

void Level1::HandleCollectibleCollision(Collectible *collectible) {
    if (!collectible->IsCollected()) {
        collectible->Collect();
        sound->PlayButtonSound(1.0f, 0.0f); // later add coin sound
        }
    }

void Level1::HandleObstacleCollision(Obstacle *obstacle) {
    sound->PlayHitSound(1.0f, 0.0f);


}