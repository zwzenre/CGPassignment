#pragma once
#include "fmod.hpp"

class SoundManager {
public:
    // SoundManager();
    // ~SoundManager();

    FMOD::System * system; // Virtual Sound Card
    FMOD::Sound * sound1, * sound2; // Store Sound File
    FMOD::Channel * channel = 0; // Buffer
    FMOD_RESULT result; // HRESULT
    void * extradriverdata = 0; // Nothing

    void InitializeAudio();
    void PlaySound1(float pitch, float pan);
    void PlaySoundTrack();
    void LoadSounds();
    void UpdateSound();
    void cleanup();
};
