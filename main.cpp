#include "src/Manager/Header/SceneManager.h"
#include "src/Scene/Header/Level1.h"
#include "src/Scene/Header/MainMenuScene.h"

int main() {
    SceneManager sceneManager;

    if (!sceneManager.Init()) {
        return -1;
    }

    // Start with Level1 scene
    //sceneManager.ChangeScene(new Level1());
    sceneManager.ChangeScene(new MainMenuScene());



    // Main game loop
    while (sceneManager.WindowIsRunning()) {
        sceneManager.Update();
        sceneManager.Render();
    }

    sceneManager.Quit();
    return 0;
}
