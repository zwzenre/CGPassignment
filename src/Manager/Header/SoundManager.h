#pragma once
#include "fmod.hpp"

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    FMOD::System * system; // Virtual Sound Card

    // Store Sound File
    FMOD::Sound * sound2,
    * mainMenuBgm, * buttonSound, * gameplayBgm, * carSound, * hitSound, * startCountdownSound, * goalSound;
    //main menu bgm, button sound effect, car drive, car hit box, start race, goal

    FMOD::Channel * channel = 0; // Buffer
    FMOD_RESULT result; // HRESULT
    void * extradriverdata = 0; // Nothing

    void InitializeAudio();
    void PlayCarSound(float pitch, float pan);
    void PlayHitSound(float pitch, float pan);
    void PlayMainMenuBgm();
    void PlayGameplayBgm();
    void PlayStartCountdown(float pitch, float pan);
    void PlayGoalSound(float pitch, float pan);
    void PlayButtonSound(float pitch, float pan);
    void PlaySoundTrack();
    void LoadSounds();
    void UpdateSound();
    void cleanup();
    void StopBackgroundMusic();
};
