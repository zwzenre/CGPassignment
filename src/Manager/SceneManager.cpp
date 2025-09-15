#include "Header/SceneManager.h"

SceneManager::SceneManager() : currentScene(nullptr) {}

SceneManager::~SceneManager() {
    Quit();
}

void SceneManager::ChangeScene(Scene* newScene, IDirect3DDevice9* device, InputManager* inputMgr,
                              SoundManager* soundMgr, HWND hWnd, int screenWidth, int screenHeight) {

    if (soundMgr) {
        soundMgr->StopBackgroundMusic();
    }

    if (currentScene) {
        currentScene->Quit();
        delete currentScene;
    }

    currentScene = newScene;
    if (currentScene) {
        currentScene->Init(device, inputMgr, soundMgr, hWnd, screenWidth, screenHeight);
    }
}

void SceneManager::Update(float deltaTime) {
    if (currentScene) {
        currentScene->Update(deltaTime);
    }
}

void SceneManager::Render(LPD3DXSPRITE spriteBrush) {
    if (currentScene) {
        currentScene->Render(spriteBrush);
    }
}

void SceneManager::Quit() {
    if (currentScene) {
        currentScene->Quit();
        delete currentScene;
        currentScene = nullptr;
    }
}