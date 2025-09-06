#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <string>
#include <dinput.h>
#include "src/Timer/Header/Timer.h"
#include "src/Manager/Header/InputManager.h"
#include "src/Manager/Header/SoundManager.h"

using namespace std;
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

HRESULT hr;
HWND g_hWnd = NULL;
WNDCLASS wndClass;
MSG msg;

int windowWidth = 1280;
int windowHeight = 720;

LPD3DXSPRITE spriteBrush = NULL;
LPD3DXFONT fontBrush = NULL;
LPD3DXLINE lineBrush = NULL;

LPDIRECT3DTEXTURE9 playerTexture = NULL;
RECT player1Rect;
D3DXVECTOR2 player1Pos = D3DXVECTOR2(150, 150);
D3DXVECTOR2 playerScaling = D3DXVECTOR2(0.7f, 0.7f);
int player1Frame = 0;

// --- Sprite sheet info (64x128 per frame, 7 frames in one row) ---
int player1SpriteWidth = 64;
int player1SpriteHeight = 128;
int player1Col = 7;
int player1Row = 1;
int player1MaxFrame = 7;

float player1Rotation = 0.0f;
float playerRotationSpeed = 0.1f;
D3DXVECTOR2 playerCenter = D3DXVECTOR2(player1SpriteWidth / 2.0f, player1SpriteHeight / 2.0f);

D3DXVECTOR2 player1Velocity = D3DXVECTOR2(0, 0);

D3DXVECTOR2 player1Translation;
D3DXMATRIX player1Matrix;

float maxSpeed = 10.0f;
float accelerationRate = 0.2f;
float brakeRate = 0.3f;
float drag = 0.05f;
float steeringAngle = 0.05f;

bool windowScreen = true;
int globalFrameCounter = 0;

BYTE diKeys[256];
DIMOUSESTATE mouseState;

D3DPRESENT_PARAMETERS d3dPP;
IDirect3DDevice9 *d3dDevice;
LPDIRECTINPUT8 dInput;
LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
LPDIRECTINPUTDEVICE8 dInputMouseDevice;

Timer frameTimer = Timer();
SoundManager * soundManager = new SoundManager();
InputManager * inputManager = new InputManager();
#define FPS 120

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
                case 'F':
                    // ToggleFullScreen();  // optional
                    break;
                default:
                    break;
            }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateGameWindow() {
    ZeroMemory(&wndClass, sizeof(wndClass));

    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpfnWndProc = WindowProcedure;
    wndClass.lpszClassName = "My Window";
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

    g_hWnd = CreateWindowEx(0, wndClass.lpszClassName,
                            "Car Sprite Animation Demo",
                            WS_OVERLAPPEDWINDOW,
                            0, 100, windowWidth, windowHeight,
                            NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(g_hWnd, 1);

    ShowCursor(true);
}

bool WindowIsRunning() {
    ZeroMemory(&msg, sizeof(msg));
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void CleanupWindow() {
    UnregisterClass(wndClass.lpszClassName, GetModuleHandle(NULL));
}

void CleanupDirectX() {
    if (d3dDevice) {
        d3dDevice->Release();
        d3dDevice = NULL;
    }
}

bool CreateDirectX() {
    IDirect3D9 *direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

    ZeroMemory(&d3dPP, sizeof(d3dPP));
    d3dPP.Windowed = windowScreen;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferCount = 1;
    d3dPP.BackBufferWidth = windowWidth;
    d3dPP.BackBufferHeight = windowHeight;
    d3dPP.hDeviceWindow = g_hWnd;

    hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);

    if (FAILED(hr)) {
        MessageBox(NULL, "Unable to instantiate DirectX", "ERROR", MB_ICONSTOP | MB_OK);
        return false;
    }
    return true;
}

void InitializeGame() {
    hr = D3DXCreateSprite(d3dDevice, &spriteBrush);
    if (FAILED(hr))
        MessageBox(NULL, "Unable to create sprite brush", "ERROR", MB_ICONSTOP | MB_OK);

    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/car.png", &playerTexture);
    if (FAILED(hr))
        MessageBox(NULL, "Unable to load player texture", "ERROR", MB_ICONSTOP | MB_OK);

    player1Rect.top = 0;
    player1Rect.left = 0;
    player1Rect.right = player1Rect.left + player1SpriteWidth;
    player1Rect.bottom = player1Rect.top + player1SpriteHeight;
}

void CreateFontInterface() {
    hr = D3DXCreateFont(d3dDevice, 25, 0, 0, 1, false,
                        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
                        DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
}

void CleanupFontInterface() {
    if (fontBrush) {
        fontBrush->Release();
        fontBrush = NULL;
    }
}

void CreateDirectInput() {
    hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
    dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
    dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);

    dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    dInputMouseDevice->SetDataFormat(&c_dfDIMouse);

    dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputMouseDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void CleanupDirectInput() {
    if (dInputKeyboardDevice) {
        dInputKeyboardDevice->Unacquire();
        dInputKeyboardDevice->Release();
        dInputKeyboardDevice = NULL;
    }
    if (dInputMouseDevice) {
        dInputMouseDevice->Unacquire();
        dInputMouseDevice->Release();
        dInputMouseDevice = NULL;
    }
    if (dInput) {
        dInput->Release();
        dInput = NULL;
    }
}

void CleanupTexture() {
    if (spriteBrush) { spriteBrush->Release(); spriteBrush = NULL; }
    if (playerTexture) { playerTexture->Release(); playerTexture = NULL; }
}

void CleanAll() {
    CleanupTexture();
    CleanupDirectX();
    CleanupFontInterface();
    CleanupDirectInput();
    CleanupWindow();
}

bool Initialization() {
    CreateGameWindow();
    if (!CreateDirectX())
        return false;

    InitializeGame();
    CreateFontInterface();
    CreateDirectInput();
    frameTimer.Init(FPS);

    dInputKeyboardDevice->Acquire();
    dInputMouseDevice->Acquire();

    return true;
}

void Input() {
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
}

void Update(int framesToUpdate) {
    for (int i = 0; i < framesToUpdate; i++) {
        globalFrameCounter++;

        // --- Controls ---
        bool forward  = (diKeys[DIK_W] | diKeys[DIK_UP]) & 0x80;
        bool backward = (diKeys[DIK_S] | diKeys[DIK_DOWN]) & 0x80;
        bool left     = (diKeys[DIK_A] | diKeys[DIK_LEFT]) & 0x80;
        bool right    = (diKeys[DIK_D] | diKeys[DIK_RIGHT]) & 0x80;

        // --- Apply forward/backward force ---
        if (forward) {
            player1Velocity.x += sin(player1Rotation) * accelerationRate;
            player1Velocity.y += -cos(player1Rotation) * accelerationRate;
        }
        if (backward) {
            player1Velocity.x -= sin(player1Rotation) * brakeRate;
            player1Velocity.y -= -cos(player1Rotation) * brakeRate;
        }

        // --- Steering (only if car is moving) ---
        if (D3DXVec2Length(&player1Velocity) > 0.1f) {
            if (left) {
                player1Rotation -= steeringAngle;
            }
            if (right) {
                player1Rotation += steeringAngle;
            }
        }

        // --- Apply drag (friction) ---
        player1Velocity.x *= (1.0f - drag);
        player1Velocity.y *= (1.0f - drag);

        // --- Clamp max speed ---
        float speed = D3DXVec2Length(&player1Velocity);
        if (speed > maxSpeed) {
            D3DXVec2Normalize(&player1Velocity, &player1Velocity);
            player1Velocity *= maxSpeed;
        }

        // --- Update position ---
        player1Pos += player1Velocity;

        // --- Boundary check ---
        if (player1Pos.y <= 0 || player1Pos.y >= windowHeight - player1SpriteHeight) {
            player1Velocity.y *= -0.5f;
            player1Pos.y = max(0.0f, min((float)(windowHeight - player1SpriteHeight), player1Pos.y));
        }
        if (player1Pos.x <= 0 || player1Pos.x >= windowWidth - player1SpriteWidth) {
            player1Velocity.x *= -0.5f;
            player1Pos.x = max(0.0f, min((float)(windowWidth - player1SpriteWidth), player1Pos.x));
        }

        // --- Animation frames ---
        player1Frame = 0; // default forward
        if (left) {
            int animSpeed = 6;
            int animIndex = (globalFrameCounter / animSpeed) % 3;
            player1Frame = 1 + animIndex;
        }
        else if (right) {
            int animSpeed = 6;
            int animIndex = (globalFrameCounter / animSpeed) % 3;
            player1Frame = 4 + animIndex;
        }

        // --- Update transformation matrix ---
        player1Translation = D3DXVECTOR2(player1Pos.x, player1Pos.y);
        D3DXMatrixTransformation2D(
            &player1Matrix,
            &playerCenter,
            0.0,
            &playerScaling,
            &playerCenter,
            player1Rotation,
            &player1Translation
        );
    }
}

void Render() {
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

    d3dDevice->BeginScene();
    spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

    int col = player1Frame % player1Col;
    int row = player1Frame / player1Col;

    player1Rect.left   = col * player1SpriteWidth;
    player1Rect.top    = row * player1SpriteHeight;
    player1Rect.right  = player1Rect.left + player1SpriteWidth;
    player1Rect.bottom = player1Rect.top + player1SpriteHeight;

    spriteBrush->SetTransform(&player1Matrix);
    spriteBrush->Draw(playerTexture, &player1Rect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

    spriteBrush->End();
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

int main() {
    if (!Initialization())
        return 0;

    cout << "Entering game loop..." << endl;

    while (WindowIsRunning()) {
        Input();
        Update(frameTimer.FramesToUpdate());
        Render();
    }

    CleanAll();
    return 0;
}
