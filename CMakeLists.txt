cmake_minimum_required(VERSION 3.31)
project(Game)

set(CMAKE_CXX_STANDARD 20)

set(FMOD_SDK_PATH "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows")
set(FMOD_API_PATH "${FMOD_SDK_PATH}/api/core")

add_executable(Game main.cpp
        src/Game/Game.cpp
        src/Game/Header/Game.h
        src/Manager/InputManager.cpp
        src/Manager/Header/InputManager.h
        src/Timer/Timer.cpp
        src/Timer/Header/Timer.h
        src/Manager/SceneManager.cpp
        src/Manager/Header/SceneManager.h
        src/Manager/SoundManager.cpp
        src/Manager/Header/SoundManager.h
        src/Manager/PhysicsManager.cpp
        src/Manager/Header/PhysicsManager.h
        src/Scene/Level1.cpp
        src/Scene/Header/Level1.h
        src/Game/Map.cpp
        src/Game/Header/Map.h
        src/Game/Obstacle.cpp
        src/Game/Header/Obstacle.h
        src/Game/RaceCar.cpp
        src/Game/Header/RaceCar.h
        src/Scene/MainMenuScene.cpp
        src/Scene/Header/MainMenuScene.h
        src/Scene/SelectStageScene.cpp
        src/Scene/Header/SelectStageScene.h
        src/Scene/Header/Scene.h
        src/Scene/MainMenuScene.cpp
        src/Game/Cursor.cpp
        src/Game/Header/Cursor.h
        src/Game/Collectible.cpp
        src/Game/Header/Collectible.h
        src/Game/Item.cpp
        src/Game/Header/Item.h
#        src/Scene/EndScene.cpp
#        src/Scene/Header/EndScene.h
)

# USE THIS ONLY FOR int WINAPI WinMain() - WITHOUT CONSOLE OUTPUT
# set_property(TARGET Game PROPERTY WIN32_EXECUTABLE TRUE)

# USE THIS FOR int main() TO PRINT CONSOLE OUTPUT
target_link_options(Game PRIVATE "/SUBSYSTEM:CONSOLE")

include_directories($ENV{DXSDK_DIR}/Include)
include_directories("${FMOD_API_PATH}/inc")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FMOD_LIB_NAME "fmodL_vc")
    set(FMOD_DLL_NAME "fmodL.dll")
else()
    set(FMOD_LIB_NAME "fmod_vc")
    set(FMOD_DLL_NAME "fmod.dll")
endif()

target_link_options(Game PRIVATE
        "/LIBPATH:$ENV{DXSDK_DIR}/Lib/x86"
        "/LIBPATH:${FMOD_API_PATH}/lib/x86"
)

target_link_libraries(Game
        d3d9
        d3dx9
        dinput8
        dxguid
        ${FMOD_LIB_NAME}
)

set(GAME_ASSETS
        assets/Mainmenu.png
        assets/Mainmenu2.png
        assets/button_round_depth_line.png
        assets/button_square_line.png
        assets/resultScreen.png
        assets/EndBg.png
        assets/button_round_line.png
        assets/car.png
        assets/cursor.png
        assets/coin.png
        assets/box.jpg
        assets/btn_pop.wav
        assets/car_drive.wav
        assets/gameplay_bgm.wav
        assets/goal_sound.wav
        assets/main_menu_bgm.wav
        assets/start_countdown.wav
        assets/exitBtn.png
        assets/restartBtn.png
)

add_custom_command(
        TARGET Game
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FMOD_API_PATH}/lib/x86/${FMOD_DLL_NAME}"
        "$<TARGET_FILE_DIR:Game>"
        COMMENT "Copying FMOD DLL: ${FMOD_DLL_NAME}"
)

foreach (asset_file IN LISTS GAME_ASSETS)
    add_custom_command(
            TARGET Game
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${asset_file}"
            "$<TARGET_FILE_DIR:Game>/${asset_file}"
            COMMENT "Copying asset: ${asset_file}"
    )
endforeach ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Game PRIVATE _DEBUG)
endif ()