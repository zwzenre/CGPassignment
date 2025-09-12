#pragma once
#include "Header/Item.h"

class Collectible : public Item {
public:
    Collectible(D3DXVECTOR2 position, const char* texturePath);
    ~Collectible() override;

    bool Initialize(IDirect3DDevice9* device) override;
    void Update(float deltaTime) override;
    void Render(LPD3DXSPRITE spriteBrush) override;

    bool IsCollected() const { return collected; }
    void Collect() { collected = true; }

    RECT GetBoundingBox() { return Item::GetBoundingBox(); }

private:
    bool collected;
};