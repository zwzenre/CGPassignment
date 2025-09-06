#include "Header/Game.h"
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

void Game::Initialization() {
    hr = D3DXCreateSprite(d3dDevice, &spriteBrush);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to create sprite brush", "ERROR", MB_ICONSTOP || MB_OK);


    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/practical9.png", &texture[0]);
    if(FAILED(hr))
        MessageBox(NULL, "Unable to load Texture", "ERROR", MB_ICONSTOP || MB_OK);
}

void Game::Render() {

}

void Game::Update() {

}

void Game::Quit() {

}


