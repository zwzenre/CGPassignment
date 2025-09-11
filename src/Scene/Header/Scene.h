#pragma once
#include <d3dx9.h>

class InputManager;
class SoundManager;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr,
                     HWND hWnd = nullptr, int screenWidth = 1280, int screenHeight = 720) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(LPD3DXSPRITE sprite) = 0;
    virtual void Quit() = 0;
};