#include "../../Timer/Header/Timer.h"
#include "../../Manager/Header/SoundManager.h"
#include "../../Manager/Header/PhysicsManager.h"
#include "../../Manager/Header/InputManager.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifndef GAME_GAME_H
#define GAME_GAME_H

class Game {
private:
    Timer timer;
    SoundManager soundManager;
    PhysicsManager physicsManager;
    InputManager inputManager;
    int level;
    HRESULT hr;
    LPD3DXSPRITE spriteBrush;
    LPD3DXFONT fontBrush;
    LPD3DXLINE lineBrush;
    D3DPRESENT_PARAMETERS d3dPP;
    IDirect3DDevice9 *d3dDevice;
    LPDIRECT3DTEXTURE9 texture[];
    


public:
    void Initialization();
    void Render();
    void Update();
    void Quit();
};

#endif //GAME_GAME_H
