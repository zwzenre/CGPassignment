#include "InputManager.h"

void InputManager::GetDevice() {
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
}
