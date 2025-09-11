#include "Header/Cursor.h"
#include <windows.h>

Cursor::Cursor()
    : d3dDevice(nullptr), texture(nullptr),
      position(400.0f, 300.0f),  // Start at center-ish
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

    // Set initial position to center of screen (will be updated by InputManager)
    RECT rect;
    if (GetClientRect(GetActiveWindow(), &rect)) {
        position.x = (rect.right - rect.left) / 2.0f;
        position.y = (rect.bottom - rect.top) / 2.0f;
    }

    return true;
}

void Cursor::Shutdown() {
    if (texture) {
        texture->Release();
        texture = nullptr;
    }
}

void Cursor::UpdateDelta(LONG dx, LONG dy, int screenW, int screenH) {
    // Apply movement delta
    position.x += static_cast<float>(dx);
    position.y += static_cast<float>(dy);

    // Calculate actual cursor size after scaling
    float cursorWidth = texWidth * scale.x;
    float cursorHeight = texHeight * scale.y;

    // Clamp to screen boundaries
    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > screenW - cursorWidth) position.x = screenW - cursorWidth;
    if (position.y > screenH - cursorHeight) position.y = screenH - cursorHeight;
}

void Cursor::Render(LPD3DXSPRITE sprite) {
    if (!visible || !texture || !sprite) return;

    // Create transformation matrix with both scaling and position
    D3DXMATRIX transform;
    D3DXMatrixTransformation2D(
        &transform,             // Output matrix
        nullptr,                // Scaling center (null = origin)
        0.0f,                  // Scaling rotation
        &scale,                // Scaling factors
        nullptr,                // Rotation center (null = origin)
        0.0f,                  // Rotation angle
        &position              // Translation (position)
    );

    sprite->SetTransform(&transform);

    // Draw at origin since position is handled by transform matrix
    D3DXVECTOR3 drawPos(0.0f, 0.0f, 0.0f);
    sprite->Draw(texture, nullptr, nullptr, &drawPos, D3DCOLOR_ARGB(255, 255, 255, 255));

    // Reset transform to identity for other sprites
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    sprite->SetTransform(&identity);
}