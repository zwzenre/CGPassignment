#include "Header/Collectible.h"

Collectible::Collectible(D3DXVECTOR2 position, const char* texturePath)
        : Item(position, D3DXVECTOR2(32, 32), texturePath), collected(false),
          currentFrame(0), frameWidth(16), frameHeight(16),
          frameTimer(0.0f), frameDelay(0.1f), maxFrames(7) {}

Collectible::~Collectible() = default;

bool Collectible::Initialize(IDirect3DDevice9* device) {
    return Item::Initialize(device);
}

void Collectible::Update(float deltaTime) {
    if (collected) return;

    frameTimer += deltaTime;
    if (frameTimer >= frameDelay) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % maxFrames;
    }
}

void Collectible::Render(LPD3DXSPRITE spriteBrush) {
    if (collected || !texture || !spriteBrush) return;

    RECT srcRect;
    srcRect.left = currentFrame * frameWidth;
    srcRect.top = 0;
    srcRect.right = srcRect.left + frameWidth;
    srcRect.bottom = srcRect.top + frameHeight;

    D3DXMATRIX mat;
    D3DXMatrixTranslation(&mat, position.x, position.y, 0.0f);
    spriteBrush->SetTransform(&mat);

    D3DXVECTOR3 drawPos(0.0f, 0.0f, 0.0f);
    spriteBrush->Draw(texture, NULL, NULL, &drawPos, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}