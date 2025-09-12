#pragma once

#include <d3dx9.h>

class Item {
public:
    Item(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath = nullptr);
    virtual ~Item();

    virtual bool Initialize(IDirect3DDevice9* device);
    virtual void Update(float deltaTime) = 0;
    virtual void Render(LPD3DXSPRITE spriteBrush) = 0;

    // Getters
    D3DXVECTOR2 GetPosition() const { return position; }
    D3DXVECTOR2 GetSize() const { return size; }
    LPDIRECT3DTEXTURE9 GetTexture() const { return texture; }
    RECT GetBoundingBox() const;

    // Setters
    void SetPosition(D3DXVECTOR2 newPos) { position = newPos; }
    void SetSize(D3DXVECTOR2 newSize) { size = newSize; }

    virtual void OnCollision(class RaceCar* car);

protected:
    IDirect3DDevice9* d3dDevice;
    LPDIRECT3DTEXTURE9 texture;
    D3DXVECTOR2 position; // Top-left corner
    D3DXVECTOR2 size;     // Width, Height
    const char* texturePath; // Path to the texture file
};