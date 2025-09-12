#pragma once
#include "Header/Item.h"

class Obstacle : public Item {
public:
    Obstacle(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath);
    ~Obstacle() override;

    bool Initialize(IDirect3DDevice9* device) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE spriteBrush) override;

    RECT GetBoundingBox() { return Item::GetBoundingBox(); }
};