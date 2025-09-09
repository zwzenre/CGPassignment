// #pragma once
// #include <d3d9.h>
// #include <d3dx9.h>
// #include <dinput.h>
// #include <string>
//
// class MainMenu {
// public:
//     MainMenu();
//     ~MainMenu();
//
//     void Initialization(LPDIRECT3DDEVICE9 device, int width, int height, HWND hwnd);
//     void Update(const DIMOUSESTATE& mouseState, const BYTE* diKeys);
//     void Render(LPD3DXSPRITE spriteBrush);
//
//     bool IsPlayButtonClicked() const { return playButtonClicked; }
//     void ResetPlayButton() { playButtonClicked = false; }
//
// private:
//     LPDIRECT3DDEVICE9 d3dDevice;
//     int windowWidth;
//     int windowHeight;
//     HWND hWnd;
//
//     LPDIRECT3DTEXTURE9 menuBackground;
//     D3DXVECTOR3 menuBackgroundPos;
//     RECT mainMenuRect;
//
//     LPD3DXFONT titleFont;
//     LPD3DXFONT fontBrush;
//
//     RECT playButtonRect;
//     bool playButtonHovered;
//     bool playButtonPressed;
//     bool playButtonClicked;
//     LPDIRECT3DTEXTURE9 playButtonTex;
//     LPDIRECT3DTEXTURE9 playButtonHoverTex;
//
//     LPDIRECTINPUT8 dInput;
//     LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
//     LPDIRECTINPUTDEVICE8 dInputMouseDevice;
//
//     void CreateDirectInput();
//     void CreateFontInterface();
//     void CreateTitleFont();
//
//     void DrawTitle(LPD3DXSPRITE spriteBrush);
//     void DrawButton(LPD3DXSPRITE spriteBrush);
//
//     void CleanupDirectInput();
//     void CleanupTextures();
//     void CleanupFonts();
// };