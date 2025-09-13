#include "Header/Collectible.h"

Collectible::Collectible(D3DXVECTOR2 position, const char* texturePath)
        : Item(position, D3DXVECTOR2(32, 32), texturePath), collected(false),
          currentFrame(0), frameWidth(17), frameHeight(17),
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

    D3DXVECTOR2 coinTextureSize((float)frameWidth, (float)frameHeight);
    D3DXVECTOR2 scaleVec(size.x / coinTextureSize.x, size.y / coinTextureSize.y);

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

    spriteBrush->Draw(texture, &srcRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    spriteBrush->SetTransform(&identity);
}