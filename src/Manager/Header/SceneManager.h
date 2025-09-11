#pragma once
#include "../../Scene/Header/Scene.h"

class SceneManager {
private:
    Scene* currentScene;

public:
    SceneManager();
    ~SceneManager();

    // Updated ChangeScene signature
    void ChangeScene(Scene* newScene, IDirect3DDevice9* device, InputManager* inputMgr,
                    SoundManager* soundMgr, HWND hWnd = nullptr,
                    int screenWidth = 1280, int screenHeight = 720);

    void Update(float deltaTime);
    void Render(LPD3DXSPRITE spriteBrush);
    void Quit();

    Scene* GetCurrentScene() const { return currentScene; }
};