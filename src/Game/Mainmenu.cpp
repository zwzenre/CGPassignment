#include "Header/Mainmenu.h"
#include "MainMenu.h"
#include <windows.h>

// 默认构造函数
MainMenu::MainMenu()
    : selectedIndex(0),
      currentState(MENU_ACTIVE),
      keyPressed(false),
      lastKeyTime(0),
      menuPosition(300.0f, 200.0f),
      selectedColor(D3DCOLOR_XRGB(255, 255, 0)), // 黄色
      normalColor(D3DCOLOR_XRGB(255, 255, 255)),  // 白色
      itemSpacing(40.0f)
{
    // 初始化默认菜单项
    items.push_back("Start Game");
    items.push_back("Options");
    items.push_back("Exit");
}

// 带参数的构造函数
MainMenu::MainMenu(D3DXVECTOR2 position, float spacing)
    : selectedIndex(0),
      currentState(MENU_ACTIVE),
      keyPressed(false),
      lastKeyTime(0),
      menuPosition(position),
      selectedColor(D3DCOLOR_XRGB(255, 255, 0)),
      normalColor(D3DCOLOR_XRGB(255, 255, 255)),
      itemSpacing(spacing)
{
    items.push_back("Start Game");
    items.push_back("Options");
    items.push_back("Exit");
}

// 析构函数
MainMenu::~MainMenu()
{
    items.clear();
}

// 更新菜单逻辑
void MainMenu::update()
{
    if (currentState != MENU_ACTIVE) return;

    handleInput();
}

// 处理输入的私有方法
void MainMenu::handleInput()
{
    // 向上移动
    if (isKeyPressedWithDelay(VK_UP))
    {
        moveUp();
    }

    // 向下移动
    if (isKeyPressedWithDelay(VK_DOWN))
    {
        moveDown();
    }

    // Enter键确认选择（可以在Game类中处理）
    if (isKeyPressedWithDelay(VK_RETURN))
    {
        // 可以添加选择确认的逻辑
        // 或者让Game类来处理这个
    }
}

// 检查按键是否按下且有延迟
bool MainMenu::isKeyPressedWithDelay(int virtualKey)
{
    DWORD currentTime = GetTickCount();

    if (GetAsyncKeyState(virtualKey) & 0x8000)
    {
        if (!keyPressed || (currentTime - lastKeyTime) > KEY_DELAY)
        {
            keyPressed = true;
            lastKeyTime = currentTime;
            return true;
        }
    }
    else
    {
        keyPressed = false;
    }

    return false;
}

// 重置按键状态
void MainMenu::resetKeyState()
{
    keyPressed = false;
    lastKeyTime = 0;
}

// 渲染菜单
void MainMenu::render(LPD3DXSPRITE spriteHandler, LPD3DXFONT font)
{
    if (currentState == MENU_INACTIVE) return;

    RECT rect;
    float currentY = menuPosition.y;

    for (size_t i = 0; i < items.size(); i++)
    {
        SetRect(&rect,
                static_cast<int>(menuPosition.x),
                static_cast<int>(currentY),
                static_cast<int>(menuPosition.x + 400),
                static_cast<int>(currentY + itemSpacing));

        D3DCOLOR color = (i == selectedIndex) ? selectedColor : normalColor;

        font->DrawTextA(spriteHandler,
                       items[i].c_str(),
                       -1,
                       &rect,
                       DT_LEFT | DT_TOP,
                       color);

        currentY += itemSpacing;
    }
}

// 移动选择向上
void MainMenu::moveUp()
{
    selectedIndex--;
    if (selectedIndex < 0)
    {
        selectedIndex = static_cast<int>(items.size()) - 1;
    }
}

// 移动选择向下
void MainMenu::moveDown()
{
    selectedIndex++;
    if (selectedIndex >= static_cast<int>(items.size()))
    {
        selectedIndex = 0;
    }
}

// Getter方法
int MainMenu::getSelectedIndex() const
{
    return selectedIndex;
}

MenuOption MainMenu::getSelectedOption() const
{
    return static_cast<MenuOption>(selectedIndex);
}

std::string MainMenu::getSelectedText() const
{
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size()))
    {
        return items[selectedIndex];
    }
    return "";
}

// 设置选中项
void MainMenu::setSelected(int index)
{
    if (index >= 0 && index < static_cast<int>(items.size()))
    {
        selectedIndex = index;
    }
}

// 状态管理
MenuState MainMenu::getState() const
{
    return currentState;
}

void MainMenu::setState(MenuState state)
{
    currentState = state;
}

bool MainMenu::isActive() const
{
    return currentState == MENU_ACTIVE;
}

// 菜单项管理
void MainMenu::addMenuItem(const std::string& item)
{
    items.push_back(item);
}

void MainMenu::removeMenuItem(int index)
{
    if (index >= 0 && index < static_cast<int>(items.size()))
    {
        items.erase(items.begin() + index);

        // 调整选中索引
        if (selectedIndex >= static_cast<int>(items.size()))
        {
            selectedIndex = static_cast<int>(items.size()) - 1;
        }
        if (selectedIndex < 0 && !items.empty())
        {
            selectedIndex = 0;
        }
    }
}

void MainMenu::clearMenu()
{
    items.clear();
    selectedIndex = 0;
}

// 外观设置
void MainMenu::setColors(D3DCOLOR selected, D3DCOLOR normal)
{
    selectedColor = selected;
    normalColor = normal;
}

void MainMenu::setPosition(const D3DXVECTOR2& position)
{
    menuPosition = position;
}

void MainMenu::setSpacing(float spacing)
{
    itemSpacing = spacing;
}

// 工具方法
int MainMenu::getItemCount() const
{
    return static_cast<int>(items.size());
}

bool MainMenu::isEmpty() const
{
    return items.empty();
}