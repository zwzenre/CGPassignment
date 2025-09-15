BMCS 2224 Computer Game Programming

# Initial C++

Initial C++ is a fast-paced 2D arcade racing game built with C++ and DirectX9. Players can control their cars to drive through levels, avoiding obstacles and collecting all collectibles within the time limit. Our goal is to create an easy-to-learn and visually captivating racing experience with simple controls while maintaining challenge, smooth animations and fun sound effects.

---

## Game Objective
Deliver tofu as fast as possible while:
- Avoiding obstacles (boxes/rocks)
- Collecting all coins on the road  
Finishing faster and breaking fewer boxes earns more stars!

---

## Features
- Main Menu with game title and interactive “Play” button.
- Smooth 2D Car Movement with collision detection in level 1.
- Background Music and sound effects integrated with Sound Manager.
- Collectible coins system
- Timer for scoring

---

## Gameplay
- Control: Arrow keys / WASD to steer and accelerate.
- Obstacles: Obstacles will appear on the playing field.
- Levels: The playing field is filled with obstacles and collectibles.
- Scoring: Collect all collectibles within the time limit.
- Main Menu: Players start from the main menu and enter by clicking the “Play” button.

---

## Screenshots

### Main Menu
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/31a4ec41-bd1c-4d8d-8fcf-a8bec5e19b4e" />

### Gameplay Scene
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/71a99c89-ed8a-49e5-adff-17eab2c75de3" />

### End Scene
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/041f2bcc-7679-429b-a498-5f10b048c77c" />

---

## Authors

- **Chea Hong Jun** - cheahj-wm23@student.tarc.edu.my
- **Ng Zhe Wei** - ngzw-wm23@student.tarc.edu.my
- **Teoh Yong Ming** - teohym-wm23@student.tarc.edu.my

---

##  Build Instructions
1. Open the project in **Visual Studio** (with DirectX 9 SDK installed).
2. Ensure the `assets/` folder is copied to the executable directory.
3. Build and run the project.

---

## Project Structure
```
CGPassignment/
├── assets/ #assets (image, sound)
├── cmake-build-debug/ 
└── src/
├── Game/ # Game core class
│ ├── Header/ 
│ │ ├── Collectible.cpp
│ │ ├── Cursor.cpp
│ │ ├── Game.cpp
│ │ ├── Item.cpp
│ │ ├── Map.cpp
│ │ ├── Obstacle.cpp
│ │ └── RaceCar.cpp
│ └── ...
├── Manager/ # Manager (Sound, Input, Scene, Physics)
│ ├── Header/ 
│ │ ├── InputManager.h
│ │ ├── PhysicsManager.h
│ │ ├── SceneManager.h
│ │ └── SoundManager.h
│ ├── InputManager.cpp
│ ├── PhysicsManager.cpp
│ ├── SceneManager.cpp
│ └── SoundManager.cpp
├── scene/ # Game scene(main menu, level, end scene)
│ ├── Header/
│ │ ├── EndScene.h
│ │ ├── Level1.h
│ │ ├── MainMenuScene.h
│ │ ├── Scene.h
│ │ └── SelectStageScene.h
│ ├── EndScene.cpp
│ ├── Level1.cpp
│ ├── MainMenuScene.cpp
│ └── SelectStageScene.cpp
└── Timer/ # Timer
├── Header/Timer.h
└── Timer.cpp
```

---

## Credits
- Programming: Initial C++
- Assets: Custom / free resources
- Engine: DirectX 9 (C++)

---

## Acknowledgments

- Thanks to our course instructor for guidance.
- Thanks to all team members for their collaboration on programming, design, art and sound modules.
- Some of the art and sound assets used in this game were sourced from Kenney, Freesound.org, and Itch.io under open licenses.
- Thanks to the C++ / DirectX9 communities, tutorials and documentation that provided technical support and learning materials.


