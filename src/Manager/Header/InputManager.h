#pragma once
#include <dinput.h>

class InputManager {
public:
    InputManager();
    //~InputManager();
    void GetDevice();

private:
    BYTE  diKeys[256];
    DIMOUSESTATE mouseState;

    LPDIRECTINPUT8 dInput;
    LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
    LPDIRECTINPUTDEVICE8 dInputMouseDevice;

    HRESULT hr;
};

