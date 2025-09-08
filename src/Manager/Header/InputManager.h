#pragma once
#include <dinput.h>
#include <Windows.h>

class InputManager {
public:
    InputManager();
    ~InputManager();

    bool Initialize(HWND hWnd);
    void Update();
    void Quit();

    bool IsKeyDown(int dik) const;
    bool IsMouseButtonDown(int button) const;
    LONG GetMouseX() const;
    LONG GetMouseY() const;
    LONG GetMouseZ() const;

private:
    BYTE diKeys[256];
    DIMOUSESTATE mouseState;

    LPDIRECTINPUT8 dInput;
    LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
    LPDIRECTINPUTDEVICE8 dInputMouseDevice;

    HRESULT hr;
};