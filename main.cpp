#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <string>
#include <dinput.h>
#include "src/Timer/Timer.h"
#include "src/Manager/InputManager.h"
#include "src/Manager/SoundManager.h"
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
D3DXVECTOR2 playerScaling = D3DXVECTOR2(1.0f, 1.0f);
int player1Frame = 0;
int player1Width = 32;
int player1Height = 64;
int player1Col = 1;
int player1Row = 2;
int player1MaxFrame = 2;
int player1SpriteWidth = player1Width / player1Col;
int player1SpriteHeight = player1Height / player1Row;
float player1Rotation;
float playerRotationSpeed = 0.1f;
D3DXVECTOR2 playerCenter = D3DXVECTOR2(player1SpriteWidth / 2.0f, player1SpriteHeight / 2.0f);

D3DXVECTOR2 player1EngineForce = D3DXVECTOR2(0, 0);
float player1EnginePower = 1.0f;
float player1Mass = 5.0f;
D3DXVECTOR2 player1Acceleration = D3DXVECTOR2(0, 0);
D3DXVECTOR2 player1Velocity = D3DXVECTOR2(0, 0);
D3DXVECTOR2 gravityAcceleration = D3DXVECTOR2(0, 0.2);

RECT player2Rect;
D3DXVECTOR2 player2Pos = D3DXVECTOR2(300, 150);
int player2Frame = 0;
int player2Width = 32;
int player2Height = 64;
int player2Col = 1;
int player2Row = 2;
int player2MaxFrame = 2;
int player2SpriteWidth = player2Width / player2Col;
int player2SpriteHeight = player2Height / player2Row;
float player2Rotation;
D3DXVECTOR2 player1Translation;
D3DXVECTOR2 player2Translation;

D3DXMATRIX player1Matrix;
D3DXMATRIX player2Matrix;

void ToggleFullScreen();
bool CreateDirectX();
void CircleCollisionDetection();
bool RectangleCollisionDetection(RECT a, RECT b);

bool windowScreen = true;

int globalFrameCounter = 0;

BYTE  diKeys[256];
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
        case WM_MOUSEMOVE:
            break;
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
                case 'F':
                    ToggleFullScreen();
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

    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpfnWndProc = WindowProcedure;
    wndClass.lpszClassName = "My Window";
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

    g_hWnd = CreateWindowEx(0, wndClass.lpszClassName, "Project Sekai: Colorful Stage feat. Hatsune Miku",
                            WS_OVERLAPPEDWINDOW,
                            0, 100, 1280, 720,
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
    d3dDevice->Release();
    d3dDevice = NULL;
}

bool CreateDirectX() {
    IDirect3D9 *direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

    ZeroMemory(&d3dPP, sizeof(d3dPP));

    d3dPP.Windowed = windowScreen;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferCount = 1;
    d3dPP.BackBufferWidth = windowScreen ? 1280 : 1920;
    d3dPP.BackBufferHeight = windowScreen ? 720 : 1080;
    d3dPP.hDeviceWindow = g_hWnd;

    hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);

    if (FAILED(hr)) {
        MessageBox(NULL, "Unable to instantiate DirectX", "ERROR", MB_ICONSTOP || MB_OK);
        return false;
    }
    return true;
}

void InitializeGame() {
    hr = D3DXCreateSprite(d3dDevice, &spriteBrush);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create sprite brush", "ERROR", MB_ICONSTOP || MB_OK);

    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/practical9.png", &playerTexture);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to load player texture", "ERROR", MB_ICONSTOP || MB_OK);

    player1Rect.top = 0;
    player1Rect.left = 0;
    player1Rect.right = player1Rect.left + player1SpriteWidth;
    player1Rect.bottom = player1Rect.top + player1SpriteHeight;

    player2Rect.top = 0;
    player2Rect.left = player1Rect.left + player2SpriteWidth;
    player2Rect.right = player2Rect.left + player2SpriteWidth;
    player2Rect.bottom = player2Rect.top + player2SpriteHeight;
}

void ToggleFullScreen() {
    windowScreen = !windowScreen;

    if (!windowScreen) {
        SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP);
        windowWidth = 1920;
        windowHeight = 1080;
    } else {
        SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        windowWidth = 1280;
        windowHeight = 720;
    }
    SetWindowPos(g_hWnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_SHOWWINDOW);

    d3dDevice->Reset(&d3dPP);
    CreateDirectX();
    InitializeGame();
}

void CreateFontInterface() {
    hr = D3DXCreateFont(d3dDevice, 25, 0, 0, 1, false,
                        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
                        DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create font brush", "ERROR", MB_ICONSTOP || MB_OK);
}

void CleanupFontInterface() {
    fontBrush->Release();
    fontBrush = NULL;
}

void CreateDirectInput() {
    hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create Direct Input", "ERROR", MB_ICONSTOP || MB_OK);

    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create Keyboard Input", "ERROR", MB_ICONSTOP || MB_OK);

    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create Mouse Input", "ERROR", MB_ICONSTOP || MB_OK);

    dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    dInputMouseDevice->SetDataFormat(&c_dfDIMouse);

    dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputMouseDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void CleanupDirectInput() {
    dInputKeyboardDevice->Unacquire();
    dInputKeyboardDevice->Release();
    dInputKeyboardDevice = NULL;

    dInputMouseDevice->Unacquire();
    dInputMouseDevice->Release();
    dInputMouseDevice = NULL;

    dInput->Release();
    dInput = NULL;
}

void CreateLineInterface() {
    HRESULT hr = D3DXCreateLine(d3dDevice, &lineBrush);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create line brush", "ERROR", MB_ICONSTOP || MB_OK);
}

void CleanupLineInterface() {
    lineBrush->Release();
    lineBrush = NULL;
}

void CleanupTexture() {
    spriteBrush->Release();
    spriteBrush = NULL;

    playerTexture->Release();
    playerTexture = NULL;
}

void CleanAll() {
    CleanupTexture();
    CleanupDirectX();
    CleanupFontInterface();
    CleanupDirectInput();
    CleanupLineInterface();
    CleanupWindow();
}

bool Initialization() {
    CreateGameWindow();

    if (!CreateDirectX())
        return false;

    InitializeGame();
    CreateFontInterface();
    CreateDirectInput();
    CreateLineInterface();
    frameTimer.Init(FPS);

    dInputKeyboardDevice->Acquire();
    dInputMouseDevice->Acquire();
}

void Input() {
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
}

void Update(int framesToUpdate) {
    for (int i = 0; i < framesToUpdate; i++) {
        globalFrameCounter++;

        player1Frame = globalFrameCounter % player1MaxFrame;
        player2Frame = globalFrameCounter % player2MaxFrame;

        player1EngineForce = D3DXVECTOR2(0, 0);

        if ((diKeys[DIK_W] | diKeys[DIK_UP]) & 0x80) {
            std::cout << "UP" << std::endl;
            player1EngineForce.x = sin(player1Rotation) * player1EnginePower;
            player1EngineForce.y = - cos(player1Rotation) * player1EnginePower;
        }

        if ((diKeys[DIK_S] | diKeys[DIK_DOWN]) & 0x80) {
            std::cout << "DOWN" << std::endl;
        }

        if ((diKeys[DIK_A] | diKeys[DIK_LEFT]) & 0x80) {
            std::cout << "LEFT" << std::endl;
            player1Rotation -= playerRotationSpeed;
        }

        if ((diKeys[DIK_D] | diKeys[DIK_RIGHT]) & 0x80) {
            std::cout << "RIGHT" << std::endl;
            player1Rotation += playerRotationSpeed;
        }

        player1Acceleration = player1EngineForce / player1Mass;
        player1Velocity = player1Velocity + player1Acceleration; // + gravityAcceleration
        player1Pos = player1Pos + player1Velocity;

        if (player1Pos.y <= 0 | player1Pos.y >= windowHeight - player1SpriteHeight) {
            player1Velocity.y *= -1;
        }

        if (player1Pos.x <= 0 | player1Pos.x >= windowWidth - player1SpriteWidth) {
            player1Velocity.x *= -1;
        }

        CircleCollisionDetection();

        player1Translation = D3DXVECTOR2(player1Pos.x, player1Pos.y);
        player2Translation = D3DXVECTOR2(player2Pos.x, player2Pos.y);

        D3DXMatrixTransformation2D(&player1Matrix, &playerCenter, 0.0, &playerScaling, &playerCenter, player1Rotation, &player1Translation);
        D3DXMatrixTransformation2D(&player2Matrix, &playerCenter, 0.0, &playerScaling, &playerCenter, player2Rotation, &player2Translation);
    }
}

void Render() {
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3dDevice->BeginScene();

    spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

    player1Rect.left = 0;
    player1Rect.top = player1Frame / (float)player1Row * player1Height;
    player1Rect.right = player1Rect.left + player1SpriteWidth;
    player1Rect.bottom = player1Rect.top + player1SpriteHeight;

    spriteBrush->SetTransform(&player1Matrix);
    spriteBrush->Draw(playerTexture, &player1Rect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

    player2Rect.left = player2Width;
    player2Rect.top = player2Frame / (float)player2Row * player2Height;
    player2Rect.right = player2Rect.left + player2SpriteWidth;
    player2Rect.bottom = player2Rect.top + player2SpriteHeight;

    spriteBrush->SetTransform(&player2Matrix);
    spriteBrush->Draw(playerTexture, &player2Rect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

    spriteBrush->End();
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

bool RectangleCollisionDetection(RECT a, RECT b) {


    if (a.top > b.bottom) return false;
    if (a.bottom < b.top) return false;
    if (a.left > b.right) return false;
    if (a.right < b.bottom) return false;

    return true;
}

void CircleCollisionDetection() {
    int radius1 = player1SpriteWidth / 2 * playerScaling.x;
    int radius2 = player2SpriteWidth / 2 * playerScaling.x;
    D3DXVECTOR2 distance = player1Pos - player2Pos;
    float dist = sqrt((distance.x * distance.x) + (distance.y * distance.y));

    if (radius1 + radius2 > dist) {
        std::cout << "Collision Detected" << std::endl;

        D3DXVECTOR2 normalisedDistance;

        // Normalize the perpendicular vector
        D3DXVec2Normalize(&normalisedDistance, &distance);
        D3DXVECTOR2 reflectionDirection = D3DXVec2Dot(&player1Velocity, &normalisedDistance) * normalisedDistance;
        player1Velocity = player1Velocity - reflectionDirection * 2;
        // Change vector to BounceForce
        //player2Velocity = player1Velocity * -1;

        soundManager->PlaySound1(1, player1Pos.x / (float)windowWidth * 2 - 1);
    }
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    if(Initialization())
        return 0;

    soundManager->InitializeAudio();
    soundManager->LoadSounds();
    soundManager->PlaySoundTrack();

    while (WindowIsRunning()) {
        Input();
        Update(frameTimer.FramesToUpdate());
        Render();
    }

    CleanAll();
    return 0;
}