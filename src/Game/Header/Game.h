#pragma once

// DirectX includes
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

// 包含MainMenu.h，这样就可以使用它里面定义的枚举
#include "MainMenu.h"

// 前向声明其他类
class Timer;
class SoundManager;
class PhysicsManager;
class InputManager;
class SceneManager;
class Level;

// 只定义GameState，不要重复定义MenuOption和MenuState
enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_EXIT
};

class Game {
private:
    // 所有Game.cpp中使用的成员变量
    Timer* timer;
    SoundManager* soundManager;
    PhysicsManager* physicsManager;
    InputManager* inputManager;
    SceneManager* sceneManager;
    Level* level;
    MainMenu* mainMenu;

    // DirectX 对象
    LPDIRECT3D9 d3d;
    LPDIRECT3DDEVICE9 d3ddev;
    LPD3DXSPRITE spriteHandler;
    LPD3DXFONT fontBrush;

    // 游戏状态
    GameState currentState;

    // 窗口句柄
    HWND hWnd;

    // 私有辅助方法
    bool initDirectX();
    void handleMenuInput();
    void handleGameInput();
    void updateMenu(float deltaTime);
    void updateGame(float deltaTime);
    void renderMenu();
    void renderGame();

public:
    // 构造函数和析构函数
    Game();
    ~Game();

    // 主要方法
    bool initialization(HWND hWnd);
    void update(float deltaTime);
    void render();
    void quit();

    // 额外的管理方法
    void run();
    void cleanup();
    void setState(GameState newState);
    GameState getState() const;
};