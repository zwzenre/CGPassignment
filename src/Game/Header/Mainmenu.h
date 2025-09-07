#pragma once
#include <vector>
#include <string>
#include <d3dx9.h>

#ifndef GAME_MAINMENU_H
#define GAME_MAINMENU_H

// 菜单选项枚举，让代码更清晰
enum MenuOption {
    MENU_START_GAME = 0,
    MENU_OPTIONS = 1,
    MENU_EXIT = 2,
    MENU_COUNT // 自动计算选项数量
};

// 菜单状态枚举
enum MenuState {
    MENU_ACTIVE,
    MENU_TRANSITIONING,
    MENU_INACTIVE
};

class MainMenu {
private:
    std::vector<std::string> items;
    int selectedIndex;
    MenuState currentState;

    // 输入处理相关
    bool keyPressed;
    DWORD lastKeyTime;
    static const DWORD KEY_DELAY = 150; // 按键延迟（毫秒）

    // 渲染相关
    D3DXVECTOR2 menuPosition;
    D3DCOLOR selectedColor;
    D3DCOLOR normalColor;
    float itemSpacing;

    // 私有方法
    void handleInput();
    bool isKeyPressedWithDelay(int virtualKey);
    void resetKeyState();

public:
    // 构造函数和析构函数
    MainMenu();
    MainMenu(D3DXVECTOR2 position, float spacing = 40.0f);
    ~MainMenu();

    // 主要功能方法
    void update();
    void render(LPD3DXSPRITE spriteHandler, LPD3DXFONT font);

    // Getter和Setter方法
    int getSelectedIndex() const;
    MenuOption getSelectedOption() const;
    std::string getSelectedText() const;

    // 菜单控制方法
    void moveUp();
    void moveDown();
    void setSelected(int index);

    // 状态管理
    MenuState getState() const;
    void setState(MenuState state);
    bool isActive() const;

    // 自定义方法
    void addMenuItem(const std::string& item);
    void removeMenuItem(int index);
    void clearMenu();

    // 外观设置
    void setColors(D3DCOLOR selected, D3DCOLOR normal);
    void setPosition(const D3DXVECTOR2& position);
    void setSpacing(float spacing);

    // 工具方法
    int getItemCount() const;
    bool isEmpty() const;
};

#endif // GAME_MAINMENU_H