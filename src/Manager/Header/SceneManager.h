#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Timer/Header/Timer.h"
#include "../../Scene/Header/Scene.h"
#include "InputManager.h"
#include "SoundManager.h"

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    bool Init();
    bool WindowIsRunning();
    float GetDeltaTime();
    void ChangeScene(Scene* newScene);
    void Update();
    void Render();
    void Quit();

    IDirect3DDevice9* GetDevice() const { return d3dDevice; }
    HWND GetHWND() const { return hWnd; }
    InputManager* GetInput() { return inputManager; }
    SoundManager* GetSound() { return soundManager; }

private:
    HWND hWnd;
    WNDCLASS wndClass{};
    MSG msg{};
    const int windowWidth = 1280;
    const int windowHeight = 720;

    IDirect3DDevice9* d3dDevice;
    D3DPRESENT_PARAMETERS d3dPP{};

    Scene* currentScene;
    Timer frameTimer;
    InputManager* inputManager;
    SoundManager* soundManager;

    static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void CreateGameWindow();
    bool CreateDirectX();
    void CleanupWindow();
    void CleanupDirectX();
};