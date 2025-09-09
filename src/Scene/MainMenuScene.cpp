#include "../Scene/Header/MainMenuScene.h"
#include <iostream>

MainMenuScene::MainMenuScene() {
    d3dDevice = nullptr;
    inputManager = nullptr;
    soundManager = nullptr;
    menuBackground = nullptr;
    titleFont = nullptr;
    fontBrush = nullptr;
    playButtonTex = nullptr;
    playButtonHoverTex = nullptr;
    playButtonHovered = false;
    playButtonPressed = false;
    playButtonClicked = false;

    windowWidth = 1280;
    windowHeight = 720;
    hWnd = nullptr;
}

MainMenuScene::~MainMenuScene() {
    Quit();
}

void MainMenuScene::Init(IDirect3DDevice9* device, InputManager* inputMgr, SoundManager* soundMgr) {
    d3dDevice = device;
    inputManager = inputMgr;
    soundManager = soundMgr;

    D3DDEVICE_CREATION_PARAMETERS params;
    device->GetCreationParameters(&params);
    hWnd = params.hFocusWindow;

    RECT rect;
    GetClientRect(hWnd, &rect);
    windowWidth = rect.right - rect.left;
    windowHeight = rect.bottom - rect.top;

    D3DXCreateTextureFromFile(d3dDevice, "assets/Mainmenu2.png", &menuBackground);
    D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_line.png", &playButtonTex);
    D3DXCreateTextureFromFile(d3dDevice, "assets/button_round_depth_line.png", &playButtonHoverTex);

    int buttonWidth = 128;
    int buttonHeight = 128;
    playButtonRect.left   = 630;
    playButtonRect.top    = 300;
    playButtonRect.right  = playButtonRect.left + buttonWidth;
    playButtonRect.bottom = playButtonRect.top + buttonHeight;

    CreateFontInterface();
    CreateTitleFont();
}

void MainMenuScene::Update(float deltaTime) {
    if (!inputManager) return;

    POINT mousePos = inputManager->GetMousePosition();
    bool isMousePressed = inputManager->IsMouseButtonPressed(0);

    playButtonHovered = (mousePos.x >= playButtonRect.left &&
                         mousePos.x <= playButtonRect.right &&
                         mousePos.y >= playButtonRect.top &&
                         mousePos.y <= playButtonRect.bottom);

    if (playButtonHovered && isMousePressed) {
        playButtonPressed = true;
    } else {
        if (playButtonPressed && !isMousePressed) {
            playButtonClicked = true;
            std::cout << "Play button clicked!" << std::endl;
        }
        playButtonPressed = false;
    }
}

void MainMenuScene::Render(LPD3DXSPRITE spriteBrush) {
    if (!spriteBrush) return;

    if (menuBackground) {
        D3DSURFACE_DESC bgDesc;
        menuBackground->GetLevelDesc(0, &bgDesc);

        float scaleX = (float)windowWidth / bgDesc.Width;
        float scaleY = (float)windowHeight / bgDesc.Height;

        D3DXVECTOR2 bgScale(scaleX, scaleY);
        D3DXVECTOR2 bgPosition(0.0f, 0.0f);

        D3DXMATRIX matBg;
        D3DXMatrixTransformation2D(&matBg, NULL, 0.0f, &bgScale, NULL, 0.0f, &bgPosition);

        spriteBrush->SetTransform(&matBg);
        D3DXVECTOR3 bgPos(0, 0, 0);
        spriteBrush->Draw(menuBackground, NULL, NULL, &bgPos, D3DCOLOR_XRGB(255, 255, 255));
    }

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);

    DrawButton(spriteBrush);
    DrawTitle(spriteBrush);
}

void MainMenuScene::CreateFontInterface() {
    D3DXCreateFont(d3dDevice, 20, 0, FW_BOLD, 1, FALSE,
                   DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontBrush);
}

void MainMenuScene::CreateTitleFont() {
    D3DXCreateFont(d3dDevice, 120, 0, FW_BOLD, 1, TRUE,
                   DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DONTCARE, "Arial", &titleFont);
}

void MainMenuScene::DrawTitle(LPD3DXSPRITE spriteBrush) {
    if (!titleFont) return;

    RECT titleRect;
    SetRect(&titleRect, 0, 10, windowWidth, 150);

    const char* fullText = "Initial";
    const char* lastChar = " D";

    RECT shadowRect;
    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            if (dx == 0 && dy == 0) continue;
            shadowRect = titleRect;
            OffsetRect(&shadowRect, dx, dy);
            titleFont->DrawTextA(spriteBrush, fullText, -1, &shadowRect,
                                 DT_CENTER | DT_VCENTER,
                                 D3DCOLOR_XRGB(0, 0, 0));
        }
    }

    titleFont->DrawTextA(spriteBrush, fullText, -1, &titleRect,
                         DT_CENTER | DT_VCENTER,
                         D3DCOLOR_XRGB(255, 255, 255));

    RECT dRect = { windowWidth/2 + 50, 10, windowWidth, 150 };
    titleFont->DrawTextA(spriteBrush, lastChar, -1, &dRect,
                         DT_LEFT | DT_VCENTER,
                         D3DCOLOR_XRGB(255, 0, 0));
}

void MainMenuScene::DrawButton(LPD3DXSPRITE spriteBrush) {
    LPDIRECT3DTEXTURE9 texToDraw = playButtonHovered ? playButtonHoverTex : playButtonTex;
    if (!texToDraw) return;

    D3DSURFACE_DESC desc;
    texToDraw->GetLevelDesc(0, &desc);

    float scaleX = (float)(playButtonRect.right - playButtonRect.left) / desc.Width;
    float scaleY = (float)(playButtonRect.bottom - playButtonRect.top) / desc.Height;

    D3DXVECTOR2 scaling(scaleX, scaleY);
    D3DXVECTOR2 position((float)playButtonRect.left, (float)playButtonRect.top);

    D3DXMATRIX mat;
    D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaling, NULL, 0.0f, &position);
    spriteBrush->SetTransform(&mat);

    D3DXVECTOR3 pos(0, 0, 0);
    spriteBrush->Draw(texToDraw, NULL, NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);

    if (fontBrush) {
        fontBrush->DrawTextA(spriteBrush, "Play", -1, &playButtonRect,
                             DT_CENTER | DT_VCENTER,
                             D3DCOLOR_XRGB(255, 255, 255));
    }
}

void MainMenuScene::Quit() {
    CleanupTextures();
    CleanupFonts();
}

void MainMenuScene::CleanupTextures() {
    if (menuBackground) { menuBackground->Release(); menuBackground = nullptr; }
    if (playButtonTex) { playButtonTex->Release(); playButtonTex = nullptr; }
    if (playButtonHoverTex) { playButtonHoverTex->Release(); playButtonHoverTex = nullptr; }
}

void MainMenuScene::CleanupFonts() {
    if (titleFont) { titleFont->Release(); titleFont = nullptr; }
    if (fontBrush) { fontBrush->Release(); fontBrush = nullptr; }
}