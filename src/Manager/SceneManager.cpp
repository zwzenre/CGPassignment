#include "Header/SceneManager.h"
#include <stdexcept>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

SceneManager::SceneManager()
    : hWnd(nullptr), d3dDevice(nullptr), currentScene(nullptr),
      inputManager(new InputManager()), soundManager(new SoundManager()) {}

SceneManager::~SceneManager() {
    Quit();
    delete inputManager;
    delete soundManager;
}

bool SceneManager::Init() {
    CreateGameWindow();

    if (!hWnd) {
        MessageBox(nullptr, "Failed to create game window!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    if (!CreateDirectX()) {
        MessageBox(nullptr, "Failed to initialize Direct3D!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    inputManager->Initialize(hWnd);
    frameTimer.Init(120);
    return true;
}

bool SceneManager::WindowIsRunning() {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

float SceneManager::GetDeltaTime() {
    return 1.0f / 120.0f;
}

void SceneManager::ChangeScene(Scene* newScene) {
    if (currentScene) {
        currentScene->Quit();
        delete currentScene;
    }
    currentScene = newScene;
    if (currentScene) {
        currentScene->Init(d3dDevice, inputManager, soundManager);
    }
}

void SceneManager::Update() {
    inputManager->Update();
    if (currentScene) {
        currentScene->Update(GetDeltaTime());
    }
}

void SceneManager::Render() {
    if (!d3dDevice) return;

    d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

    if (SUCCEEDED(d3dDevice->BeginScene())) {
        if (currentScene) {
            LPD3DXSPRITE sprite;
            D3DXCreateSprite(d3dDevice, &sprite);
            currentScene->Render(sprite);
            sprite->Release();
        }
        d3dDevice->EndScene();
    }
    d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void SceneManager::Quit() {
    if (currentScene) {
        currentScene->Quit();
        delete currentScene;
        currentScene = nullptr;
    }
    CleanupDirectX();
    CleanupWindow();
}

void SceneManager::CreateGameWindow() {
    wndClass = {};
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WindowProcedure;
    wndClass.hInstance = GetModuleHandle(nullptr);
    wndClass.lpszClassName = "GameWindowClass";
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClass(&wndClass)) {
        MessageBox(nullptr, "Failed to register window class!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    hWnd = CreateWindow("GameWindowClass", "Car Racing Game",
                        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                        windowWidth, windowHeight, nullptr, nullptr,
                        GetModuleHandle(nullptr), nullptr);

    if (!hWnd) {
        MessageBox(nullptr, "Failed to create window!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
}

bool SceneManager::CreateDirectX() {
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) return false;

    ZeroMemory(&d3dPP, sizeof(d3dPP));
    d3dPP.Windowed = TRUE;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferWidth = windowWidth;
    d3dPP.BackBufferHeight = windowHeight;
    d3dPP.hDeviceWindow = hWnd;
    d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice))) {
        d3d->Release();
        return false;
    }

    d3d->Release();
    return true;
}

void SceneManager::CleanupWindow() {
    if (hWnd) {
        DestroyWindow(hWnd);
        hWnd = nullptr;
    }
    UnregisterClass("GameWindowClass", GetModuleHandle(nullptr));
}

void SceneManager::CleanupDirectX() {
    if (d3dDevice) {
        d3dDevice->Release();
        d3dDevice = nullptr;
    }
}

LRESULT CALLBACK SceneManager::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}