#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <d3d9.h>
#include <d3dx9math.h>
#include <dinput.h>
#include <Windows.h>
#include "../../Game/Header/Cursor.h"

class InputManager {
public:
    InputManager();
    ~InputManager();

    bool Initialize(HWND hWnd, LPDIRECT3DDEVICE9 d3dDevice, int screenW, int screenH);
    void Update();
    void Render(LPD3DXSPRITE spriteBrush);
    void Quit();

    bool IsKeyDown(int dik) const;
    bool IsMouseButtonDown(int button) const;
    bool IsMouseButtonPressed(int button);

    Cursor* GetCursor() const { return m_cursor; }
    DIMOUSESTATE GetMouseState() const { return mouseState; }

    bool CreateCursor(const char* texturePath = "assets/cursor.png");
    void SetCursorVisible(bool visible);
    bool IsCursorVisible() const;

    bool IsInitialized() const { return m_initialized; }

private:
    BYTE diKeys[256];
    BYTE prevDiKeys[256];
    DIMOUSESTATE mouseState;
    DIMOUSESTATE prevMouseState;

    HWND hWnd;
    LPDIRECT3DDEVICE9 m_d3dDevice;
    LPDIRECTINPUT8 dInput;
    LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
    LPDIRECTINPUTDEVICE8 dInputMouseDevice;

    Cursor* m_cursor;
    HRESULT hr;
    bool m_initialized;

    int screenWidth;
    int screenHeight;
};