#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Timer/Header/Timer.h"
#include "../../Manager/Header/InputManager.h"
#include "../../Manager/Header/SoundManager.h"
#include "../../Manager/Header/SceneManager.h"

class Game {
private:
    HWND hWnd;
    IDirect3DDevice9* d3dDevice;
    LPD3DXSPRITE spriteBrush;

    Timer timer;
    InputManager inputManager;
    SoundManager soundManager;
    SceneManager sceneManager;

    bool isRunning;
    const int windowWidth = 1280;
    const int windowHeight = 720;

public:
    Game(HINSTANCE hInstance);
    ~Game();

    bool Initialize();
    bool WindowIsRunning();
    void Run();
    void Shutdown();

    // Getters for subsystems
    HWND GetWindowHandle() const { return hWnd; }
    IDirect3DDevice9* GetDevice() const { return d3dDevice; }
    LPD3DXSPRITE GetSpriteBrush() const { return spriteBrush; }
    InputManager* GetInputManager() { return &inputManager; }
    SoundManager* GetSoundManager() { return &soundManager; }
    SceneManager* GetSceneManager() { return &sceneManager; }
    int GetWindowWidth() const { return windowWidth; }
    int GetWindowHeight() const { return windowHeight; }
    void ShowCursor(bool show);
    bool IsCursorVisible() const;

private:
    bool CreateGameWindow(HINSTANCE hInstance);
    bool InitializeDirectX();
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};