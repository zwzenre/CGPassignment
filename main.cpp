#include "src/Manager/Header/SceneManager.h"
#include "src/Scene/Header/Level1.h"
#include "src/Scene/Header/MainMenuScene.h"
#include "src/Timer/Header/Timer.h"
#include <iostream>

int main() {
    SceneManager sceneManager;
    Timer timer;
    int time = 0;

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
        timer.SetTotalTime(time++);
    }

    sceneManager.Quit();
    return 0;
}