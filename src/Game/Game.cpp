#include "Header/Game.h"
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
    timer.Init(60); // Target 60 FPS

    // Create sprite brush
    if (FAILED(D3DXCreateSprite(d3dDevice, &spriteBrush))) {
        std::cerr << "Failed to create sprite brush!" << std::endl;
        return false;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return true;
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
            sceneManager.Update(timer.SecondsPerFrame());
        }

        // Render frame
        d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

        if (SUCCEEDED(d3dDevice->BeginScene())) {
            // First: Render game scenes
            if (spriteBrush) {
                spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);
                sceneManager.Render(spriteBrush);
                spriteBrush->End();
            }

            inputManager.Render(spriteBrush);

            d3dDevice->EndScene();
        }
        d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    }
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
        hInstance, nullptr
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

LRESULT CALLBACK Game::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
                return 0;
            }
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}