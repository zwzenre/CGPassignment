#include "Header/Mainmenu.h"
#include "Header/Game.h"
#include "Header/Level.h"
#include "Header/Map.h"
#include "Header/Obstacle.h"
#include "Header/RaceCar.h"
#include "../Timer/Header/Timer.h"
#include "../Manager/Header/InputManager.h"
#include "../Manager/Header/PhysicsManager.h"
#include "../Manager/Header/SceneManager.h"
#include "../Manager/Header/SoundManager.h"


// 构造函数
Game::Game()
    : d3d(nullptr),
      d3ddev(nullptr),
      spriteHandler(nullptr),
      fontBrush(nullptr),
      currentState(STATE_MENU),
      hWnd(nullptr)
{
    // 初始化所有管理器为nullptr
    sceneManager = nullptr;
    timer = nullptr;
    soundManager = nullptr;
    physicsManager = nullptr;
    inputManager = nullptr;
    level = nullptr;
    mainMenu = nullptr;
}

// 析构函数
Game::~Game()
{
    cleanup();
}

// 初始化游戏 - 对应类图的initialization()
bool Game::initialization(HWND hWnd)
{
    this->hWnd = hWnd;

    // 初始化DirectX
    if (!initDirectX())
    {
        return false;
    }

    // 创建管理器对象（按照类图）
    try
    {
        // 创建Timer
        timer = new Timer();
        timer->initialization();

        // 创建InputManager
        inputManager = new InputManager();
        inputManager->initialization();

        // 创建SoundManager
        soundManager = new SoundManager();
        soundManager->initialization();

        // 创建PhysicsManager
        physicsManager = new PhysicsManager();
        physicsManager->initialization();

        // 创建SceneManager
        sceneManager = new SceneManager();
        //sceneManager->initialization();

        // 创建主菜单
        mainMenu = new MainMenu(D3DXVECTOR2(300.0f, 250.0f), 60.0f);

        // 创建Level（游戏关卡）
        level = new Level();

        return true;
    }
    catch (...)
    {
        cleanup();
        return false;
    }
}

// 初始化DirectX
bool Game::initDirectX()
{
    // 创建Direct3D对象
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d)
    {
        return false;
    }

    // 设置Present Parameters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // 创建设备
    HRESULT result = d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev
    );

    if (FAILED(result))
    {
        return false;
    }

    // 创建Sprite
    if (FAILED(D3DXCreateSprite(d3ddev, &spriteHandler)))
    {
        return false;
    }

    // 创建Font
    if (FAILED(D3DXCreateFont(d3ddev, 32, 0, FW_BOLD, 1, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                             "Arial", &fontBrush)))
    {
        return false;
    }

    return true;
}

// 更新游戏 - 对应类图的update()
void Game::update(float deltaTime)
{
    if (!timer) return;

    // 更新计时器
    timer->tick();
    deltaTime = timer->getDeltaTimer();

    // 更新输入
    if (inputManager)
    {
        inputManager->updateInput();
    }

    switch (currentState)
    {
        case STATE_MENU:
            updateMenu(deltaTime);
            break;

        case STATE_PLAYING:
            updateGame(deltaTime);
            break;

        case STATE_PAUSED:
            // 暂停状态处理
            handleGameInput(); // 检查是否取消暂停
            break;

        case STATE_EXIT:
            quit();
            break;
    }
}

// 更新菜单状态
void Game::updateMenu(float deltaTime)
{
    if (mainMenu)
    {
        mainMenu->update();
    }

    handleMenuInput();
}

// 更新游戏状态
void Game::updateGame(float deltaTime)
{
    // 更新物理
    if (physicsManager)
    {
        physicsManager->update(deltaTime);
    }

    // 更新场景
    if (sceneManager)
    {
        sceneManager->update(deltaTime);
    }

    // 更新关卡
    if (level)
    {

    }

    handleGameInput();
}

// 处理菜单输入
void Game::handleMenuInput()
{
    if (!inputManager || !mainMenu) return;

    // 检查Enter键确认选择
    if (inputManager->isKeyPress(VK_RETURN))
    {
        MenuOption selectedOption = mainMenu->getSelectedOption();

        switch (selectedOption)
        {
            case MENU_START_GAME:
                setState(STATE_PLAYING);
                if (soundManager)
                {
                    soundManager->PlaySoundTrack(); // 播放开始游戏音效
                }
                break;

            case MENU_OPTIONS:
                // 打开选项菜单（暂时未实现）
                break;

            case MENU_EXIT:
                setState(STATE_EXIT);
                break;
        }
    }
}

// 处理游戏输入
void Game::handleGameInput()
{
    if (!inputManager) return;

    // ESC键返回菜单或暂停
    if (inputManager->isKeyPress(VK_ESCAPE))
    {
        if (currentState == STATE_PLAYING)
        {
            setState(STATE_PAUSED);
        }
        else if (currentState == STATE_PAUSED)
        {
            setState(STATE_PLAYING);
        }
    }

    // 其他游戏输入处理...
}

// 渲染游戏 - 对应类图的render()
void Game::render()
{
    if (!d3ddev) return;

    // 清屏
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                  D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

    if (SUCCEEDED(d3ddev->BeginScene()))
    {
        spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

        switch (currentState)
        {
            case STATE_MENU:
                renderMenu();
                break;

            case STATE_PLAYING:
            case STATE_PAUSED:
                renderGame();
                break;
        }

        spriteHandler->End();
        d3ddev->EndScene();
    }

    d3ddev->Present(NULL, NULL, NULL, NULL);
}

// 渲染菜单
void Game::renderMenu()
{
    // 渲染游戏标题
    RECT titleRect;
    SetRect(&titleRect, 200, 100, 600, 150);
    fontBrush->DrawTextA(spriteHandler, "RACING GAME", -1, &titleRect,
                   DT_CENTER | DT_TOP, D3DCOLOR_XRGB(255, 255, 255));

    // 渲染菜单
    if (mainMenu)
    {
        mainMenu->render(spriteHandler, fontBrush);
    }
}

// 渲染游戏
void Game::renderGame()
{
    // 渲染场景
    if (sceneManager)
    {
        sceneManager->render();
    }

    // 渲染关卡
    if (level)
    {

    }

    // 如果是暂停状态，显示暂停文本
    if (currentState == STATE_PAUSED)
    {
        RECT pauseRect;
        SetRect(&pauseRect, 250, 300, 550, 350);
        fontBrush->DrawTextA(spriteHandler, "PAUSED - Press ESC to continue", -1, &pauseRect,
                       DT_CENTER | DT_TOP, D3DCOLOR_XRGB(255, 255, 0));
    }
}

// 游戏主循环
void Game::run()
{
    MSG msg;

    while (currentState != STATE_EXIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        float deltaTime = 0.016f; // 默认60FPS
        update(deltaTime);
        render();
    }
}

// 设置游戏状态
void Game::setState(GameState newState)
{
    currentState = newState;

    // 状态改变时的处理
    switch (newState)
    {
        case STATE_MENU:
            if (mainMenu) mainMenu->setState(MENU_ACTIVE);
            break;

        case STATE_PLAYING:
            if (mainMenu) mainMenu->setState(MENU_INACTIVE);
            // 重置游戏计时器
            if (timer) timer->reset();
            break;

        case STATE_EXIT:
            quit();
            break;
    }
}

// 获取当前状态
GameState Game::getState() const
{
    return currentState;
}

// 退出游戏 - 对应类图的quit()
void Game::quit()
{
    cleanup();
    PostQuitMessage(0);
}

// 清理资源
void Game::cleanup()
{
    // 清理管理器
    if (level) { delete level; level = nullptr; }
    if (mainMenu) { delete mainMenu; mainMenu = nullptr; }
    if (sceneManager) { delete sceneManager; sceneManager = nullptr; }
    if (soundManager) { delete soundManager; soundManager = nullptr; }
    if (physicsManager) { delete physicsManager; physicsManager = nullptr; }
    if (inputManager) { delete inputManager; inputManager = nullptr; }
    if (timer) { delete timer; timer = nullptr; }

    // 清理DirectX对象
    if (fontBrush) { fontBrush->Release(); fontBrush = nullptr; }
    if (spriteHandler) { spriteHandler->Release(); spriteHandler = nullptr; }
    if (d3ddev) { d3ddev->Release(); d3ddev = nullptr; }
    if (d3d) { d3d->Release(); d3d = nullptr; }
}