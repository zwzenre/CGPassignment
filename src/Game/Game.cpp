#include "Header/Game.h"
#include "../Scene/Header/MainMenuScene.h"  // Add this include
#include "../Scene/Header/Level1.h"         // Add this include
#include <iostream>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

Game::Game(HINSTANCE hInstance)
    : hWnd(nullptr), d3dDevice(nullptr), spriteBrush(nullptr), isRunning(true) {}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize() {
    if (!CreateGameWindow(GetModuleHandle(nullptr))) {
        std::cerr << "Failed to create game window!" << std::endl;
        return false;
    }

    if (!InitializeDirectX()) {
        std::cerr << "Failed to initialize DirectX!" << std::endl;
        return false;
    }

    // Initialize input manager
    if (!inputManager.Initialize(hWnd, d3dDevice, windowWidth, windowHeight)) {
        std::cerr << "Failed to initialize input manager!" << std::endl;
        return false;
    }

    // Initialize timer
    timer.Init(60);

    // Create sprite brush
    if (FAILED(D3DXCreateSprite(d3dDevice, &spriteBrush))) {
        std::cerr << "Failed to create sprite brush!" << std::endl;
        return false;
    }

    // Initialize with MainMenu scene
    InitializeMainMenu();

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return true;
}

void Game::InitializeMainMenu() {
    MainMenuScene* mainMenu = new MainMenuScene();
    sceneManager.ChangeScene(mainMenu, d3dDevice, &inputManager, &soundManager, hWnd, windowWidth, windowHeight);
    std::cout << "Game started with Main Menu" << std::endl;
}

void Game::TransitionToLevel1() {
    Level1* level1Scene = new Level1();
    sceneManager.ChangeScene(level1Scene, d3dDevice, &inputManager, &soundManager, hWnd, windowWidth, windowHeight);
    std::cout << "Transitioned to Level 1!" << std::endl;
}

void Game::TransitionToMainMenu() {
    MainMenuScene* mainMenu = new MainMenuScene();
    sceneManager.ChangeScene(mainMenu, d3dDevice, &inputManager, &soundManager, hWnd, windowWidth, windowHeight);
    std::cout << "Returned to Main Menu" << std::endl;
}

bool Game::WindowIsRunning() {
    MSG msg;
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            isRunning = false;
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return isRunning;
}

void Game::Run() {
    while (WindowIsRunning()) {
        // Update game state
        int framesToUpdate = timer.FramesToUpdate();
        for (int i = 0; i < framesToUpdate; ++i) {
            inputManager.Update();

            // Handle scene transitions BEFORE updating the scene
            HandleSceneTransitions();

            sceneManager.Update(timer.SecondsPerFrame());
        }

        // Render frame
        d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

        if (SUCCEEDED(d3dDevice->BeginScene())) {
            if (spriteBrush) {
                spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

                sceneManager.Render(spriteBrush);
                inputManager.Render(spriteBrush);

                spriteBrush->End();
            }
            d3dDevice->EndScene();
        }
        d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    }
}

void Game::HandleSceneTransitions() {
    Scene* currentScene = sceneManager.GetCurrentScene();
    if (!currentScene) return;

    // Check if we're in MainMenu and play button was clicked
    MainMenuScene* mainMenu = dynamic_cast<MainMenuScene*>(currentScene);
    if (mainMenu && mainMenu->IsPlayButtonClicked()) {
        mainMenu->ResetPlayButton();
        TransitionToLevel1();
        return;
    }

    // Check if we're in Level1 and should return to menu (ESC key)
    Level1* level1 = dynamic_cast<Level1*>(currentScene);
    if (level1 && inputManager.IsKeyDown(DIK_ESCAPE)) {
        TransitionToMainMenu();
        return;
    }

    // Add more transition logic here as needed
    // if (level1 && level1->IsLevelComplete()) { TransitionToLevel2(); }
}

void Game::Shutdown() {
    sceneManager.Quit();

    if (spriteBrush) {
        spriteBrush->Release();
        spriteBrush = nullptr;
    }

    if (inputManager.IsInitialized()) {
        inputManager.Quit();
    }

    if (d3dDevice) {
        d3dDevice->Release();
        d3dDevice = nullptr;
    }

    if (hWnd) {
        DestroyWindow(hWnd);
        hWnd = nullptr;
    }
}

void Game::ShowCursor(bool show) {
    if (inputManager.GetCursor()) {
        inputManager.GetCursor()->SetVisible(show);
        ::ShowCursor(show);
    }
}

bool Game::IsCursorVisible() const {
    return inputManager.GetCursor() ? inputManager.GetCursor()->IsVisible() : false;
}

bool Game::CreateGameWindow(HINSTANCE hInstance) {
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GameWindowClass";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, "Failed to register window class!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    hWnd = CreateWindow(
        "GameWindowClass",
        "Car Racing Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        nullptr, nullptr,
        hInstance, this
    );

    if (!hWnd) {
        MessageBox(nullptr, "Failed to create window!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

bool Game::InitializeDirectX() {
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) {
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = windowWidth;
    d3dpp.BackBufferHeight = windowHeight;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (FAILED(d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3dDevice
    ))) {
        d3d->Release();
        return false;
    }

    d3d->Release();
    return true;
}

void Game::ToggleFullscreen() {
    if (!isFullScreen) {
        // Save current window rect
        GetWindowRect(hWnd, &windowRect);

        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP, 0, 0,
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN),
            SWP_FRAMECHANGED | SWP_SHOWWINDOW);

        isFullScreen = true;
    } else {
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP,
            windowRect.left, windowRect.top,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW);

        isFullScreen = false;
    }
}

LRESULT CALLBACK Game::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Game* game = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        game = reinterpret_cast<Game*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)game);
    } else {
        game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (game) {
        switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            // Remove ESC quit from here since Level1 uses ESC to return to menu
            if (wParam == 'F') {
                game->ToggleFullscreen();
                return 0;
            }
            break;
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}