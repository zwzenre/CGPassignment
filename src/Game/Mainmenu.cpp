#include "Header/Mainmenu.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


extern LPD3DXSPRITE spriteBrush;
extern LPDIRECT3DDEVICE9 d3dDevice;
extern LPDIRECT3DTEXTURE9 menuBackground;



void MainMenu::Initialization()
{
    HRESULT hr = D3DXCreateTextureFromFileEx(
        d3dDevice,
        "assets/Mainmenu.png",
        D3DX_DEFAULT, D3DX_DEFAULT,
        D3DX_DEFAULT, NULL,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        D3DX_DEFAULT, D3DX_DEFAULT, 0,
        NULL, NULL,
        &menuBackground
    );

    if (FAILED(hr)) {
        MessageBox(NULL, "Failed to load Mainmenu.png!", "ERROR", MB_OK | MB_ICONSTOP);
    }
}

// void CreateFontInterface()
// {
//     HRESULT hr = D3DXCreateFont(d3dDevice, 20, 0, 0, 1, false,
//         DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
//         DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
//
//     textRect.left = 10;
//     textRect.top = 10;
//     textRect.right = 400;
//     textRect.bottom = 200;
// }

void MainMenu::update()
{

}

void MainMenu::handleInput()
{
    if (isKeyPressedWithDelay(VK_UP))
    {
        moveUp();
    }

    if (isKeyPressedWithDelay(VK_DOWN))
    {
        moveDown();
    }

    if (isKeyPressedWithDelay(VK_RETURN))
    {

    }
}

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

void MainMenu::resetKeyState()
{
    keyPressed = false;
    lastKeyTime = 0;
}

void MainMenu::render()
{
    if (spriteBrush && menuBackground) {
        spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

        D3DXVECTOR3 position(0.0f, 0.0f, 0.0f);
        spriteBrush->Draw(menuBackground, NULL, NULL, &position,
                          D3DCOLOR_XRGB(255, 255, 255));

        spriteBrush->End();
    }
}

void MainMenu::moveUp()
{
    selectedIndex--;
    if (selectedIndex < 0)
    {
        selectedIndex = static_cast<int>(items.size()) - 1;
    }
}

void MainMenu::moveDown()
{
    selectedIndex++;
    if (selectedIndex >= static_cast<int>(items.size()))
    {
        selectedIndex = 0;
    }
}

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

void MainMenu::setSelected(int index)
{
    if (index >= 0 && index < static_cast<int>(items.size()))
    {
        selectedIndex = index;
    }
}

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

void MainMenu::addMenuItem(const std::string& item)
{
    items.push_back(item);
}

void MainMenu::removeMenuItem(int index)
{
    if (index >= 0 && index < static_cast<int>(items.size()))
    {
        items.erase(items.begin() + index);

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

int MainMenu::getItemCount() const
{
    return static_cast<int>(items.size());
}

bool MainMenu::isEmpty() const
{
    return items.empty();
}