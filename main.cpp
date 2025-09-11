#include "src/Game/Header/Game.h"
#include "src/Scene/Header/Level1.h"
#include "src/Scene/Header/MainMenuScene.h"
#include <iostream>

int main() {
    // Get the instance handle
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // Create the main Game object
    Game game(hInstance);

    // Initialize the game (creates window, DirectX, etc.)
    if (!game.Initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return -1;
    }

    // Start with Level1 scene (passing required dependencies)
    game.GetSceneManager()->ChangeScene(
        new Level1(),
        game.GetDevice(),
        game.GetInputManager(),
        game.GetSoundManager(),
        game.GetWindowHandle(),  // Add HWND parameter
        game.GetWindowWidth(),   // Add screen width
        game.GetWindowHeight()   // Add screen height
    );

    // game.GetSceneManager()->ChangeScene(
    //         new MainMenuScene(),
    //         game.GetDevice(),
    //         game.GetInputManager(),
    //         game.GetSoundManager(),
    //         game.GetWindowWidth(),
    //         game.GetWindowHeight());

    // Run the main game loop
    game.Run();

    // Cleanup
    game.Shutdown();

    return 0;
}