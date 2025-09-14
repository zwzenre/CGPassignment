#define NOMINMAX
#include "Header/EndScene.h"
#include "../Manager/Header/InputManager.h"
#include "../Manager/Header/SoundManager.h"
#include <d3dx9.h>
#include <windows.h>
#include <algorithm>

using std::max;
using std::min;

void EndScene::Init(IDirect3DDevice9* dev,
                    InputManager* inputMgr,
                    SoundManager* soundMgr,
                    HWND hwnd,
                    int width,
                    int height) {
    device = dev;
    inputManager = inputMgr;
    soundManager = soundMgr;
    hWnd = hwnd;
    screenWidth = width;
    screenHeight = height;

    // Load textures
    if (FAILED(D3DXCreateTextureFromFile(device, "assets/EndBg.png", &bgTexture))) {
        MessageBox(nullptr, "Failed to load EndBg.png", "Error", MB_OK);
        bgTexture = nullptr;
    }
    if (FAILED(D3DXCreateTextureFromFile(device, "assets/resultScreen.png", &resultTexture))) {
        MessageBox(nullptr, "Failed to load resultScreen.png", "Error", MB_OK);
        resultTexture = nullptr;
    }
    if (FAILED(D3DXCreateTextureFromFile(device, "assets/restartBtn.png", &restartBtnTexture))) {
        MessageBox(nullptr, "Failed to load restartBtn.png", "Error", MB_OK);
        restartBtnTexture = nullptr;
    }
    if (FAILED(D3DXCreateTextureFromFile(device, "assets/exitBtn.png", &exitBtnTexture))) {
        MessageBox(nullptr, "Failed to load exitBtn.png", "Error", MB_OK);
        exitBtnTexture = nullptr;
    }

    if (FAILED(D3DXCreateTextureFromFile(device, "assets/complete_star.png", &starCompleteTexture))) {
        MessageBox(nullptr, "Failed to load complete_star.png", "Error", MB_OK);
        starCompleteTexture = nullptr;
    }

    if (FAILED(D3DXCreateTextureFromFile(device, "assets/incomplete_star.png", &starIncompleteTexture))) {
        MessageBox(nullptr, "Failed to load incomplete_star.png", "Error", MB_OK);
        starIncompleteTexture = nullptr;
    }

    // Create font (safe check)
    if (device) {
        HRESULT hr = D3DXCreateFont(device, 28, 0, FW_BOLD, 1, FALSE,
                                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                   DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);
        if (FAILED(hr)) {
            font = nullptr;
        }
    }

    // Initialize button rects / state
    restartBtnRect = {0};
    exitBtnRect = {0};
    restartSelected = false;
    exitRequested = false;
}

void EndScene::Update(float deltaTime) {
    if (!inputManager) return;

    // Keyboard escape -> exit
    if (inputManager->IsKeyDown(DIK_ESCAPE)) {
        exitRequested = true;
    }

    // Cursor position (DirectInput cursor from InputManager)
    D3DXVECTOR2 cursorPos = inputManager->GetCursorPosition();

    // If left mouse clicked (pressed), check if inside button rects
    if (inputManager->IsMouseButtonPressed(0)) {
        if (restartBtnRect.left != 0 || restartBtnRect.top != 0 || restartBtnRect.right != 0 || restartBtnRect.bottom != 0) {
            if (cursorPos.x >= restartBtnRect.left && cursorPos.x <= restartBtnRect.right &&
                cursorPos.y >= restartBtnRect.top  && cursorPos.y <= restartBtnRect.bottom) {
                restartSelected = true;
            }
        }
        if (exitBtnRect.left != 0 || exitBtnRect.top != 0 || exitBtnRect.right != 0 || exitBtnRect.bottom != 0) {
            if (cursorPos.x >= exitBtnRect.left && cursorPos.x <= exitBtnRect.right &&
                cursorPos.y >= exitBtnRect.top  && cursorPos.y <= exitBtnRect.bottom) {
                exitRequested = true;
            }
        }
    }
}

void EndScene::Render(LPD3DXSPRITE spriteBrush) {
    if (!spriteBrush) return;

    // Background
    if (bgTexture) {
        D3DSURFACE_DESC bgDesc;
        bgTexture->GetLevelDesc(0, &bgDesc);

        // Stretch to screen bounds
        float scaleX = static_cast<float>(screenWidth)  / static_cast<float>(bgDesc.Width);
        float scaleY = static_cast<float>(screenHeight) / static_cast<float>(bgDesc.Height);

        D3DXVECTOR2 scaling(scaleX, scaleY);
        D3DXVECTOR2 trans(0.0f, 0.0f);

        D3DXMATRIX matBg;
        D3DXMatrixTransformation2D(&matBg,
                                  nullptr,                          // scaling center
                                  0.0f,
                                  &scaling,                         // scale
                                  nullptr,                          // rotation center
                                  0.0f,                             // rotation
                                  &trans);                  // translation

        spriteBrush->SetTransform(&matBg);
        D3DXVECTOR3 origin(0, 0, 0);
        spriteBrush->Draw(bgTexture, nullptr, nullptr, &origin, D3DCOLOR_XRGB(255,255,255));
    }

    // Reset transform
    {
        D3DXMATRIX id;
        D3DXMatrixIdentity(&id);
        spriteBrush->SetTransform(&id);
    }

    // Result box
    RECT boxRectLocal = {0,0,0,0}; // local box rect in screen coordinates
    if (resultTexture) {
        D3DSURFACE_DESC boxDesc;
        resultTexture->GetLevelDesc(0, &boxDesc);

        // Desired size relative to screen
        float targetWidth  = screenWidth  * 0.4f;
        float targetHeight = screenHeight * 0.25f;

        // Keep aspect ratio
        float scale = min(targetWidth / static_cast<float>(boxDesc.Width),
                          targetHeight / static_cast<float>(boxDesc.Height));

        float scaledWidth  = boxDesc.Width  * scale;
        float scaledHeight = boxDesc.Height * scale;

        float offsetX = (screenWidth  - scaledWidth)  * 0.5f;
        float offsetY = (screenHeight - scaledHeight) * 0.35f; // slightly higher than center

        D3DXVECTOR2 boxScale(scale, scale);
        D3DXVECTOR2 boxPos(offsetX, offsetY);

        D3DXMATRIX matBox;
        D3DXMatrixTransformation2D(&matBox, nullptr, 0.0f, &boxScale, nullptr, 0.0f, &boxPos);
        spriteBrush->SetTransform(&matBox);

        D3DXVECTOR3 origin(0,0,0);
        spriteBrush->Draw(resultTexture, nullptr, nullptr, &origin, D3DCOLOR_XRGB(255,255,255));

        // Save local screen-space rect for text and button placement
        boxRectLocal.left   = static_cast<LONG>(offsetX);
        boxRectLocal.top    = static_cast<LONG>(offsetY);
        boxRectLocal.right  = static_cast<LONG>(offsetX + scaledWidth);
        boxRectLocal.bottom = static_cast<LONG>(offsetY + scaledHeight);

        // Reset transform after drawing box
        D3DXMATRIX id;
        D3DXMatrixIdentity(&id);
        spriteBrush->SetTransform(&id);
    }

//    // Message text
//    if (font && resultTexture) {
//        RECT textRect = boxRectLocal;
//
//        // shrink the text rect a little so it doesn't overlap the buttons
//        int paddingTop = static_cast<int>((textRect.bottom - textRect.top) * 0.08f);
//        int paddingSides = static_cast<int>((textRect.right - textRect.left) * 0.08f);
//        textRect.left  += paddingSides;
//        textRect.right -= paddingSides;
//        textRect.top   += paddingTop;
//        textRect.bottom = textRect.top + 80;
//
//        font->DrawText(spriteBrush, message.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_XRGB(255,255,255));
//    }

    if (starCompleteTexture && starIncompleteTexture) {
        D3DSURFACE_DESC starDesc;
        starCompleteTexture->GetLevelDesc(0, &starDesc);

        float starSize = 64.0f;
        float starScale = starSize / static_cast<float>(starDesc.Width);
        float totalStarsWidth = starSize * 3 + (starSize / 4) * 2; // 3 stars + 2 small gaps

        float startX = (boxRectLocal.left + boxRectLocal.right - totalStarsWidth) * 0.5f;
        float startY = boxRectLocal.top + (boxRectLocal.bottom - boxRectLocal.top) * 0.28f; // Position below title

        for (int i = 0; i < 3; ++i) {
            LPDIRECT3DTEXTURE9 currentStarTexture = (i < totalStars) ? starCompleteTexture : starIncompleteTexture;
            D3DXVECTOR2 scaleVec(starScale, starScale);
            D3DXVECTOR2 transVec(startX + i * (starSize + starSize / 4), startY);

            D3DXMATRIX matStar;
            D3DXMatrixTransformation2D(&matStar, nullptr, 0.0f, &scaleVec, nullptr, 0.0f, &transVec);
            spriteBrush->SetTransform(&matStar);

            D3DXVECTOR3 pos(0,0,0);
            spriteBrush->Draw(currentStarTexture, nullptr, nullptr, &pos, D3DCOLOR_XRGB(255,255,255));
        }

        D3DXMATRIX id;
        D3DXMatrixIdentity(&id);
        spriteBrush->SetTransform(&id);
    }

    // Buttons
    if (restartBtnTexture && exitBtnTexture && (boxRectLocal.right > boxRectLocal.left)) {
        // Button base size in texture pixels
        D3DSURFACE_DESC rDesc, eDesc;
        restartBtnTexture->GetLevelDesc(0, &rDesc);
        exitBtnTexture->GetLevelDesc(0, &eDesc);

        const float baseBtnPx = 64.0f;
        const float btnScaleTo = baseBtnPx / static_cast<float>(rDesc.Width);

        const float hoverScale = 1.15f;
        const float normalScale = btnScaleTo;

        float btnWidth  = rDesc.Width  * normalScale;
        float btnHeight = rDesc.Height * normalScale;
        float spacing = 24.0f;

        // Position
        float centerX = (boxRectLocal.left + boxRectLocal.right) * 0.5f;
        float btnY = static_cast<float>(boxRectLocal.bottom) - btnHeight - 35.0f; // 35 px padding from box bottom

        //
        float totalWidth = btnWidth * 2.0f + spacing;
        float leftX = centerX - totalWidth * 0.5f;
        float rightX = leftX + btnWidth + spacing;

        // Current cursor pos for hover
        D3DXVECTOR2 cursorPos = inputManager ? inputManager->GetCursorPosition() : D3DXVECTOR2(-1,-1);

        // Restart button
        float restartScale = normalScale;
        if (cursorPos.x >= leftX && cursorPos.x <= leftX + btnWidth &&
            cursorPos.y >= btnY   && cursorPos.y <= btnY + btnHeight) {
            restartScale = normalScale * hoverScale;
        }

        // Exit button hover
        float exitScale = normalScale;
        if (cursorPos.x >= rightX && cursorPos.x <= rightX + btnWidth &&
            cursorPos.y >= btnY   && cursorPos.y <= btnY + btnHeight) {
            exitScale = normalScale * hoverScale;
        }

        // Draw restart button
        {
            D3DXVECTOR2 scaleVec(restartScale, restartScale);
            D3DXVECTOR2 transVec(leftX, btnY);

            D3DXMATRIX mat;
            D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scaleVec, nullptr, 0.0f, &transVec);
            spriteBrush->SetTransform(&mat);

            D3DXVECTOR3 pos(0,0,0);
            spriteBrush->Draw(restartBtnTexture, nullptr, nullptr, &pos, D3DCOLOR_XRGB(0,0,0));

            // Save rect for hit-test
            float scaledW = rDesc.Width * restartScale;
            float scaledH = rDesc.Height * restartScale;
            restartBtnRect.left   = static_cast<LONG>(leftX);
            restartBtnRect.top    = static_cast<LONG>(btnY);
            restartBtnRect.right  = static_cast<LONG>(leftX + scaledW);
            restartBtnRect.bottom = static_cast<LONG>(btnY + scaledH);

            // Reset transform
            D3DXMATRIX id;
            D3DXMatrixIdentity(&id);
            spriteBrush->SetTransform(&id);
        }

        // Draw exit button
        {
            D3DXVECTOR2 scaleVec(exitScale, exitScale);
            D3DXVECTOR2 transVec(rightX, btnY);

            D3DXMATRIX mat;
            D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scaleVec, nullptr, 0.0f, &transVec);
            spriteBrush->SetTransform(&mat);

            D3DXVECTOR3 pos(0,0,0);
            spriteBrush->Draw(exitBtnTexture, nullptr, nullptr, &pos, D3DCOLOR_XRGB(0,0,0));

            float scaledW = eDesc.Width * exitScale;
            float scaledH = eDesc.Height * exitScale;
            exitBtnRect.left   = static_cast<LONG>(rightX);
            exitBtnRect.top    = static_cast<LONG>(btnY);
            exitBtnRect.right  = static_cast<LONG>(rightX + scaledW);
            exitBtnRect.bottom = static_cast<LONG>(btnY + scaledH);

            // Reset transform
            D3DXMATRIX id;
            D3DXMatrixIdentity(&id);
            spriteBrush->SetTransform(&id);
        }
    }
}

void EndScene::Quit() {
    if (bgTexture) { bgTexture->Release(); bgTexture = nullptr; }
    if (resultTexture) { resultTexture->Release(); resultTexture = nullptr; }
    if (restartBtnTexture) { restartBtnTexture->Release(); restartBtnTexture = nullptr; }
    if (exitBtnTexture) { exitBtnTexture->Release(); exitBtnTexture = nullptr; }
    if (font) { font->Release(); font = nullptr; }

    if (soundManager) {
        soundManager->StopBackgroundMusic();
    }
}