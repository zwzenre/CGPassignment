// #include "../Game/Header/Mainmenu.h"
// #include <iostream>
//
// MainMenu::MainMenu() {
//     d3dDevice = nullptr;
//     menuBackground = nullptr;
//     titleFont = nullptr;
//     fontBrush = nullptr;
//     playButtonTex = nullptr;
//     playButtonHoverTex = nullptr;
//     playButtonHovered = false;
//     playButtonPressed = false;
//     playButtonClicked = false;
//
//     dInput = nullptr;
//     dInputKeyboardDevice = nullptr;
//     dInputMouseDevice = nullptr;
// }
//
// MainMenu::~MainMenu() {
//     CleanupTextures();
//     CleanupFonts();
//     CleanupDirectInput();
// }
//
// void MainMenu::Initialization(LPDIRECT3DDEVICE9 device, int width, int height, HWND hwnd) {
//     d3dDevice = device;
//     windowWidth = width;
//     windowHeight = height;
//     hWnd = hwnd;
//
//     D3DXCreateTextureFromFile(d3dDevice, "assets/Mainmenu2.png", &menuBackground);
//     D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_line.png", &playButtonTex);
//     D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_depth_line.png", &playButtonHoverTex);
//
//     int buttonWidth = 128;
//     int buttonHeight = 128;
//     playButtonRect.left   = 630;
//     playButtonRect.top    = 300;
//     playButtonRect.right  = playButtonRect.left + buttonWidth;
//     playButtonRect.bottom = playButtonRect.top + buttonHeight;
//
//     CreateFontInterface();
//     CreateTitleFont();
//
//     CreateDirectInput();
// }
//
// void MainMenu::CreateDirectInput() {
//     HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
//     if (SUCCEEDED(hr)) {
//         dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
//         dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
//
//         if (dInputKeyboardDevice) {
//             dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
//             dInputKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//         }
//
//         if (dInputMouseDevice) {
//             dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
//             dInputMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//         }
//
//         if (dInputKeyboardDevice) dInputKeyboardDevice->Acquire();
//         if (dInputMouseDevice) dInputMouseDevice->Acquire();
//     }
// }
//
// void MainMenu::CreateFontInterface() {
//     D3DXCreateFont(d3dDevice, 20, 0, FW_BOLD, 1, FALSE,
//                    DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
//                    DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
// }
//
// void MainMenu::CreateTitleFont() {
//     D3DXCreateFont(d3dDevice, 120, 0, FW_BOLD, 1, TRUE,
//                    DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
//                    DEFAULT_PITCH | FF_DONTCARE, "Arial", &titleFont);
// }
//
// void MainMenu::Update(const DIMOUSESTATE& mouseState, const BYTE* diKeys) {
//     POINT mousePos;
//     GetCursorPos(&mousePos);
//     ScreenToClient(hWnd, &mousePos);
//
//     playButtonHovered = (mousePos.x >= playButtonRect.left &&
//                          mousePos.x <= playButtonRect.right &&
//                          mousePos.y >= playButtonRect.top &&
//                          mousePos.y <= playButtonRect.bottom);
//
//     bool isMousePressed = (mouseState.rgbButtons[0] & 0x80);
//
//     if (playButtonHovered && isMousePressed) {
//         playButtonPressed = true;
//     } else {
//         if (playButtonPressed && !isMousePressed) {
//             playButtonClicked = true;
//             std::cout << "Play button clicked!" << std::endl;
//         }
//         playButtonPressed = false;
//     }
// }
//
// void MainMenu::Render(LPD3DXSPRITE spriteBrush) {
//     if (!spriteBrush) return;
//
//     if (menuBackground) {
//         D3DSURFACE_DESC bgDesc;
//         menuBackground->GetLevelDesc(0, &bgDesc);
//
//         float scaleX = (float)windowWidth / bgDesc.Width;
//         float scaleY = (float)windowHeight / bgDesc.Height;
//
//         D3DXVECTOR2 bgScale(scaleX, scaleY);
//         D3DXVECTOR2 bgPosition(0.0f, 0.0f);
//
//         D3DXMATRIX matBg;
//         D3DXMatrixTransformation2D(&matBg, NULL, 0.0f, &bgScale, NULL, 0.0f, &bgPosition);
//
//         spriteBrush->SetTransform(&matBg);
//         D3DXVECTOR3 bgPos(0, 0, 0);
//         spriteBrush->Draw(menuBackground, NULL, NULL, &bgPos, D3DCOLOR_XRGB(255, 255, 255));
//     }
//
//     D3DXMATRIX identity;
//     D3DXMatrixIdentity(&identity);
//     spriteBrush->SetTransform(&identity);
//
//     DrawButton(spriteBrush);
//     DrawTitle(spriteBrush);
// }
//
// void MainMenu::DrawTitle(LPD3DXSPRITE spriteBrush) {
//     if (!titleFont) return;
//
//     RECT titleRect;
//     SetRect(&titleRect, 0, 10, windowWidth, 150);
//
//     const char* fullText = "Initial";
//     const char* lastChar = " D";
//
//     RECT shadowRect;
//     for (int dx = -3; dx <= 3; dx++) {
//         for (int dy = -3; dy <= 3; dy++) {
//             if (dx == 0 && dy == 0) continue;
//             shadowRect = titleRect;
//             OffsetRect(&shadowRect, dx, dy);
//             titleFont->DrawTextA(NULL, fullText, -1, &shadowRect,
//                                  DT_CENTER | DT_VCENTER,
//                                  D3DCOLOR_XRGB(0, 0, 0));
//         }
//     }
//
//     titleFont->DrawTextA(NULL, fullText, -1, &titleRect,
//                          DT_CENTER | DT_VCENTER,
//                          D3DCOLOR_XRGB(255, 255, 255));
//
//     RECT dRect = { windowWidth/2 + 50, 10, windowWidth, 150 };
//     titleFont->DrawTextA(NULL, lastChar, -1, &dRect,
//                          DT_LEFT | DT_VCENTER,
//                          D3DCOLOR_XRGB(255, 0, 0));
// }
//
// void MainMenu::DrawButton(LPD3DXSPRITE spriteBrush) {
//     LPDIRECT3DTEXTURE9 texToDraw = playButtonHovered ? playButtonHoverTex : playButtonTex;
//     if (!texToDraw) return;
//
//     D3DSURFACE_DESC desc;
//     texToDraw->GetLevelDesc(0, &desc);
//
//     float scaleX = (float)(playButtonRect.right - playButtonRect.left) / desc.Width;
//     float scaleY = (float)(playButtonRect.bottom - playButtonRect.top) / desc.Height;
//
//     D3DXVECTOR2 scaling(scaleX, scaleY);
//     D3DXVECTOR2 position((float)playButtonRect.left, (float)playButtonRect.top);
//
//     D3DXMATRIX mat;
//     D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaling, NULL, 0.0f, &position);
//     spriteBrush->SetTransform(&mat);
//
//     D3DXVECTOR3 pos(0, 0, 0);
//     spriteBrush->Draw(texToDraw, NULL, NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));
//
//     D3DXMATRIX identity;
//     D3DXMatrixIdentity(&identity);
//     spriteBrush->SetTransform(&identity);
//
//     if (fontBrush) {
//         fontBrush->DrawTextA(NULL, "Play", -1, &playButtonRect,
//                              DT_CENTER | DT_VCENTER,
//                              D3DCOLOR_XRGB(255, 255, 255));
//     }
// }
//
// void MainMenu::CleanupDirectInput() {
//     if (dInputKeyboardDevice) {
//         dInputKeyboardDevice->Unacquire();
//         dInputKeyboardDevice->Release();
//         dInputKeyboardDevice = nullptr;
//     }
//     if (dInputMouseDevice) {
//         dInputMouseDevice->Unacquire();
//         dInputMouseDevice->Release();
//         dInputMouseDevice = nullptr;
//     }
//     if (dInput) {
//         dInput->Release();
//         dInput = nullptr;
//     }
// }
//
// void MainMenu::CleanupTextures() {
//     if (menuBackground) { menuBackground->Release(); menuBackground = nullptr; }
//     if (playButtonTex) { playButtonTex->Release(); playButtonTex = nullptr; }
//     if (playButtonHoverTex) { playButtonHoverTex->Release(); playButtonHoverTex = nullptr; }
// }
//
// void MainMenu::CleanupFonts() {
//     if (titleFont) { titleFont->Release(); titleFont = nullptr; }
//     if (fontBrush) { fontBrush->Release(); fontBrush = nullptr; }
// }