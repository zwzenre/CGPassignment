#include "Header/Collectible.h"

Collectible::Collectible(D3DXVECTOR2 position, const char* texturePath)
        : Item(position, D3DXVECTOR2(32, 32), texturePath), collected(false) {}

Collectible::~Collectible() = default;

bool Collectible::Initialize(IDirect3DDevice9* device) {
    return Item::Initialize(device);
}

void Collectible::Update(float deltaTime) {
    // Collectibles might animate or rotate
    // For now, they just exist.
}

void Collectible::Render(LPD3DXSPRITE spriteBrush) {
    if (collected || !texture || !spriteBrush) return;

    D3DXMATRIX mat;
    D3DXMatrixTranslation(&mat, position.x, position.y, 0.0f);
    spriteBrush->SetTransform(&mat);

    D3DXVECTOR3 drawPos(0.0f, 0.0f, 0.0f);
    spriteBrush->Draw(texture, NULL, NULL, &drawPos, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}