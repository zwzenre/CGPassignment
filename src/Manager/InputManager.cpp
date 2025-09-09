#include "Header/InputManager.h"
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

InputManager::InputManager()
    : dInput(nullptr), dInputKeyboardDevice(nullptr), dInputMouseDevice(nullptr), hr(S_OK) {
    ZeroMemory(diKeys, sizeof(diKeys));
    ZeroMemory(&mouseState, sizeof(mouseState));
}

InputManager::~InputManager() {
    // Cleanup in Quit() method instead
}

bool InputManager::Initialize(HWND hWnd) {
    // Create DirectInput interface
    hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
                           IID_IDirectInput8, (void**)&dInput, nullptr);
    if (FAILED(hr)) return false;

    // Initialize keyboard
    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, nullptr);
    if (FAILED(hr)) return false;

    hr = dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr)) return false;

    hr = dInputKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) return false;

    hr = dInputKeyboardDevice->Acquire();
    if (FAILED(hr)) return false;

    // Initialize mouse
    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, nullptr);
    if (FAILED(hr)) return false;

    hr = dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr)) return false;

    hr = dInputMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) return false;

    hr = dInputMouseDevice->Acquire();
    if (FAILED(hr)) return false;

    return true;
}

void InputManager::Update() {
    // Update keyboard state
    hr = dInputKeyboardDevice->GetDeviceState(sizeof(diKeys), diKeys);
    if (FAILED(hr)) {
        // Try to reacquire if lost focus
        dInputKeyboardDevice->Acquire();
        dInputKeyboardDevice->GetDeviceState(sizeof(diKeys), diKeys);
    }

    // Update mouse state
    hr = dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
    if (FAILED(hr)) {
        // Try to reacquire if lost focus
        dInputMouseDevice->Acquire();
        dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
    }
}

void InputManager::Quit() {
    if (dInputKeyboardDevice) {
        dInputKeyboardDevice->Unacquire();
        dInputKeyboardDevice->Release();
        dInputKeyboardDevice = nullptr;
    }
    if (dInputMouseDevice) {
        dInputMouseDevice->Unacquire();
        dInputMouseDevice->Release();
        dInputMouseDevice = nullptr;
    }
    if (dInput) {
        dInput->Release();
        dInput = nullptr;
    }
}

POINT InputManager::GetMousePosition() {
    POINT mousePos = {0, 0};
    if (dInputMouseDevice) {
        DIMOUSESTATE mouseState;
        if (SUCCEEDED(dInputMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState))) {
            GetCursorPos(&mousePos);
            ScreenToClient(hWnd, &mousePos);
        }
    }
    return mousePos;
}

bool InputManager::IsMouseButtonPressed(int button) {
    if (dInputMouseDevice && button >= 0 && button < 8) {
        DIMOUSESTATE mouseState;
        if (SUCCEEDED(dInputMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState))) {
            return (mouseState.rgbButtons[button] & 0x80) != 0;
        }
    }
    return false;
}

bool InputManager::IsKeyDown(int dik) const {
    return (diKeys[dik] & 0x80) != 0;
}

bool InputManager::IsMouseButtonDown(int button) const {
    return (mouseState.rgbButtons[button] & 0x80) != 0;
}

LONG InputManager::GetMouseX() const {
    return mouseState.lX;
}

LONG InputManager::GetMouseY() const {
    return mouseState.lY;
}

LONG InputManager::GetMouseZ() const {
    return mouseState.lZ;
}