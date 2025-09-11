#include "Header/Cursor.h"
#include <windows.h>

Cursor::Cursor()
    : d3dDevice(nullptr), texture(nullptr),
      position(400.0f, 300.0f),
      scale(0.25f, 0.25f),
      visible(true),
      texWidth(0), texHeight(0) {}

Cursor::~Cursor() {
    Shutdown();
}

bool Cursor::Initialize(LPDIRECT3DDEVICE9 device, const char* texturePath) {
    d3dDevice = device;

    D3DXIMAGE_INFO info;
    if (FAILED(D3DXCreateTextureFromFileEx(
            d3dDevice, texturePath,
            D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
            D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
            D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
            0, &info, nullptr, &texture))) {
        MessageBox(NULL, "Failed to load cursor texture!", "Error", MB_OK | MB_ICONERROR);
        return false;
            }

    texWidth = info.Width;
    texHeight = info.Height;
    return true;
}

void Cursor::Shutdown() {
    if (texture) {
        texture->Release();
        texture = nullptr;
    }
}

void Cursor::UpdateDelta(LONG dx, LONG dy, int screenW, int screenH) {
    position.x += static_cast<float>(dx);
    position.y += static_cast<float>(dy);

    // Clamp inside screen
    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > screenW - texWidth * scale.x) position.x = screenW - texWidth * scale.x;
    if (position.y > screenH - texHeight * scale.y) position.y = screenH - texHeight * scale.y;
}

void Cursor::Render(LPD3DXSPRITE sprite) {
    if (!visible || !texture) return;

    D3DXVECTOR3 pos(position.x, position.y, 0.0f);

    D3DXMATRIX mat;
    D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scale, nullptr, 0.0f, nullptr);
    sprite->SetTransform(&mat);

    sprite->Draw(texture, nullptr, nullptr, &pos, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMatrixIdentity(&mat);
    sprite->SetTransform(&mat);
}

void Cursor::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}