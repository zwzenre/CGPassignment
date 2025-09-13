#pragma once
#include "Scene.h"
#include "../../Manager/Header/InputManager.h"
#include "../../Manager/Header/SoundManager.h"
#include "../../Manager/Header/PhysicsManager.h"
#include "../../Timer/Header/Timer.h"
#include "../../Game/Header/RaceCar.h"
#include "../../Game/Header/Collectible.h"
#include "../../Game/Header/Obstacle.h"
#include <vector>

class Level1 : public Scene {
public:
    Level1();
    ~Level1() override;

    void Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr,
              HWND hWnd = nullptr, int screenWidth = 1920, int screenHeight = 1080) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE sprite) override;
    void Quit() override;

    bool WantsToGoToEndScene() const { return goToEndScene; }
    void ResetEndSceneRequest() { goToEndScene = false; }

private:
    IDirect3DDevice9* device;
    IDirect3DTexture9* carTexture;
    InputManager* input;
    SoundManager* sound;
    RaceCar* playerCar;
    Cursor* gameCursor;
    LPD3DXFONT fontBrush;
    LPD3DXFONT timerFont;
    int screenWidth;
    int screenHeight;
    HWND hWnd;

    bool goToEndScene;

    std::vector<Collectible*> collectibles;
    std::vector<Obstacle*> obstacles;

    Timer levelTimer;
    PhysicsManager physicsManager;

    int collectedCoinCount;
    int collisionCount;

    float countdownDuration;
    float currentCountdownTime;

    const int TOTAL_COINS_FOR_STAR = 10;
    const float TIME_FOR_TWO_STARS = 120.0f;
    const int COLLISION_COUNT_FOR_THREE_STARS = 0;

    void CreateFont();
    void CleanupFont();

    void CheckCollectibleCollisions();
    void CheckObstacleCollisions();
    void HandleCollectibleCollision(Collectible* collectible);
    void DrawUI(LPD3DXSPRITE sprite);
};