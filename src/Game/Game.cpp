#include "Header/Game.h"
#include <windows.h>

Game::Game()
    : player(D3DXVECTOR2(200.0f, 200.0f)) { // spawn car at (200,200)
}

Game::~Game() {
    Quit();
}

void Game::Initialization(IDirect3DDevice9* device, HWND hWnd) {
    d3dDevice = device;

    // Create sprite brush
    hr = D3DXCreateSprite(d3dDevice, &spriteBrush);
    if (FAILED(hr)) {
        MessageBox(NULL, "Unable to create sprite brush", "ERROR", MB_ICONSTOP | MB_OK);
        return;
    }

    // Load car sprite sheet (512x512, 7 frames used)
    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/car.png", &carTexture);
    if (FAILED(hr)) {
        MessageBox(NULL, "Unable to load car texture (assets/car.png)", "ERROR", MB_ICONSTOP | MB_OK);
        return;
    }

    // Initialize timer and input manager
    timer.Init(TARGET_FPS);

    // Initialize InputManager with the window handle
    inputManager.Initialize(hWnd);

    // If you have sound initialization, you can uncomment / adapt:
    // soundManager.InitializeAudio();
    // soundManager.LoadSounds();
    // soundManager.PlaySoundTrack();
}

void Game::Update() {
    // Ask timer how many fixed-update frames we should run
    int framesToUpdate = timer.FramesToUpdate();
    if (framesToUpdate <= 0) return;

    // Poll input once per game loop
    inputManager.Update();

    // Gather button states once
    bool forward  = inputManager.IsKeyDown(DIK_W) || inputManager.IsKeyDown(DIK_UP);
    bool backward = inputManager.IsKeyDown(DIK_S) || inputManager.IsKeyDown(DIK_DOWN);
    bool left     = inputManager.IsKeyDown(DIK_A) || inputManager.IsKeyDown(DIK_LEFT);
    bool right    = inputManager.IsKeyDown(DIK_D) || inputManager.IsKeyDown(DIK_RIGHT);

    // Use fixed delta time derived from TARGET_FPS
    const float deltaTime = 1.0f / static_cast<float>(Game::TARGET_FPS);

    // Step physics / car logic that many times
    for (int i = 0; i < framesToUpdate; ++i) {
        // If you plan to centralize physics, call physicsManager here.
        // For now RaceCar.Update handles movement + animation:
        player.Update(deltaTime, forward, left, right, backward);
        timer.SetTotalTime(time++);

        // If you later want PhysicsManager to update the car:
        // physicsManager.updateRaceCar(player, forward, backward, left, right, screenW, screenH);
    }
}

void Game::Render() {
    if (!d3dDevice) return;

    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET,
                     D3DCOLOR_XRGB(200, 200, 200), 1.0f, 0);
    d3dDevice->BeginScene();

    if (spriteBrush) spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

    // Draw the player using the sprite sheet and the player's current frame
    player.Render(spriteBrush, carTexture);

    if (spriteBrush) spriteBrush->End();
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Game::Quit() {
    if (spriteBrush) { spriteBrush->Release(); spriteBrush = nullptr; }
    if (fontBrush) { fontBrush->Release(); fontBrush = nullptr; }
    if (lineBrush) { lineBrush->Release(); lineBrush = nullptr; }
    if (carTexture) { carTexture->Release(); carTexture = nullptr; }
}
