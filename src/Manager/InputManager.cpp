#include "Header/InputManager.h"

extern HWND g_hWnd;

InputManager::InputManager() {
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

void InputManager::GetDevice() {
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
}
