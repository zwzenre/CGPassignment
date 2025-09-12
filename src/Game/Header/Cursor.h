#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class Cursor {
public:
    Cursor();
    ~Cursor();

    bool Initialize(LPDIRECT3DDEVICE9 device, const char* texturePath);
    void Shutdown();

    void UpdateDelta(LONG dx, LONG dy, int screenW, int screenH);
    void Render(LPD3DXSPRITE sprite);

    void SetPosition(float x, float y);
    D3DXVECTOR2 GetPosition() const { return position; }

    // Add these missing methods:
    void SetVisible(bool visible) { this->visible = visible; }
    bool IsVisible() const { return visible; }

private:
    LPDIRECT3DDEVICE9 d3dDevice;
    LPDIRECT3DTEXTURE9 texture;
    D3DXVECTOR2 position;
    D3DXVECTOR2 scale;
    bool visible;

    int textureWidth;
    int textureHeight;
};