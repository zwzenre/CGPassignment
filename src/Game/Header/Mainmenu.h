#pragma once
#include <vector>
#include <string>
#include <d3dx9.h>

#ifndef GAME_MAINMENU_H
#define GAME_MAINMENU_H

enum MenuOption {
    MENU_START_GAME = 0,
    MENU_OPTIONS = 1,
    MENU_EXIT = 2,
    MENU_COUNT
};

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

    bool keyPressed;
    DWORD lastKeyTime;
    static const DWORD KEY_DELAY = 150;

    D3DXVECTOR2 menuPosition;
    D3DCOLOR selectedColor;
    D3DCOLOR normalColor;
    float itemSpacing;

    void handleInput();
    bool isKeyPressedWithDelay(int virtualKey);
    void resetKeyState();

public:
    LPDIRECT3DTEXTURE9 menuBackground;

    void Initialization();
    void update();
    void render();

    int getSelectedIndex() const;
    MenuOption getSelectedOption() const;
    std::string getSelectedText() const;

    void moveUp();
    void moveDown();
    void setSelected(int index);

    MenuState getState() const;
    void setState(MenuState state);
    bool isActive() const;

    void addMenuItem(const std::string& item);
    void removeMenuItem(int index);
    void clearMenu();

    void setColors(D3DCOLOR selected, D3DCOLOR normal);
    void setPosition(const D3DXVECTOR2& position);
    void setSpacing(float spacing);

    int getItemCount() const;
    bool isEmpty() const;
};

#endif // GAME_MAINMENU_H