#include "Header/Obstacle.h"

Obstacle::Obstacle(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* texturePath)
        : Item(position, size, texturePath) {}

Obstacle::~Obstacle() = default;

bool Obstacle::Initialize(IDirect3DDevice9* device) {
    return Item::Initialize(device);
}

void Obstacle::Update(float deltaTime) {
    // Example: A moving obstacle could update its position here
    // For static obstacles, this can remain empty.
}

void Obstacle::Render(LPD3DXSPRITE spriteBrush) {
    if (!texture || !spriteBrush) return;

    D3DSURFACE_DESC desc;
    texture->GetLevelDesc(0, &desc);
    D3DXVECTOR2 textureSize((float)desc.Width, (float)desc.Height);

    D3DXVECTOR2 scaleVec(size.x / textureSize.x, size.y / textureSize.y);

    D3DXMATRIX mat;
    D3DXMatrixTransformation2D(
            &mat,
            nullptr,
            0.0f,
            &scaleVec,
            nullptr,
            0.0f,
            &position
    );
    spriteBrush->SetTransform(&mat);

    D3DXVECTOR3 drawPos(0.0f, 0.0f, 0.0f);
    spriteBrush->Draw(texture, NULL, NULL, &drawPos, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}