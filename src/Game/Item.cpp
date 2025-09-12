#include "Header/Item.h"
#include <windows.h>

Item::Item(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath)
        : d3dDevice(nullptr), texture(nullptr), position(position), size(size), texturePath(texturePath) {}

Item::~Item() {
    if (texture) {
        texture->Release();
        texture = nullptr;
    }
}

bool Item::Initialize(IDirect3DDevice9* device) {
    d3dDevice = device;
    if (texturePath && d3dDevice) {
        if (FAILED(D3DXCreateTextureFromFile(d3dDevice, texturePath, &texture))) {
            MessageBox(nullptr, "Failed to load item texture!", "Error", MB_OK | MB_ICONERROR);
            return false;
        }
    }
    return true;
}

RECT Item::GetBoundingBox() const {
    RECT box;
    box.left = static_cast<LONG>(position.x);
    box.top = static_cast<LONG>(position.y);
    box.right = static_cast<LONG>(position.x + size.x);
    box.bottom = static_cast<LONG>(position.y + size.y);
    return box;
}