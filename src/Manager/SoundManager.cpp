#include "Header/SoundManager.h"

SoundManager::SoundManager() {
    system = nullptr;
    sound2 = nullptr;
    mainMenuBgm = nullptr;
    buttonSound = nullptr;
    gameplayBgm = nullptr;
    carSound = nullptr;
    hitSound = nullptr;
    startCountdownSound = nullptr;
    goalSound = nullptr;
    coinSound = nullptr;
    boxSound = nullptr;
    channel = nullptr;
    extradriverdata = nullptr;
    carChannel = nullptr;
}

SoundManager::~SoundManager() {
    cleanup();
}

void SoundManager::cleanup() {
    if (mainMenuBgm) mainMenuBgm->release();
    if (gameplayBgm) gameplayBgm->release();
    if (buttonSound) buttonSound->release();
    if (carSound) carSound->release();
    if (hitSound) hitSound->release();
    if (startCountdownSound) startCountdownSound->release();
    if (goalSound) goalSound->release();
    if (sound2) sound2->release();
    if (coinSound) coinSound->release();
    if (boxSound) boxSound->release();
    if (system) {
        system->close();
        system->release();
    }
}

void SoundManager::InitializeAudio() {
    result = FMOD::System_Create(&system);
    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    LoadSounds();

}

//car drive
void SoundManager::PlayCarSound(float pitch, float pan, float volume) {
    bool isPlaying = false;
    carChannel->setVolume(volume);
    if (carChannel) {
        carChannel->isPlaying(&isPlaying);
    }

    if (!isPlaying) {
        result = system->playSound(carSound, 0, false, &carChannel);
        if (carChannel) {
            carChannel->setPitch(pitch);
            carChannel->setPan(pan);
        }
    }


}

//car hit box
void SoundManager::PlayHitSound(float pitch, float pan) {
    result = system->playSound(hitSound, 0, false, &channel);
    channel->setPitch(pitch);
    channel->setPan(pan);
}

//main menu bgm
void SoundManager::PlayMainMenuBgm() {
    result = system->playSound(mainMenuBgm, 0, false, &channel);
}

//gameplay bgm
void SoundManager::PlayGameplayBgm(float volume) {
    result = system->playSound(gameplayBgm, 0, false, &channel);
    channel->setVolume(volume);
}

//start countdown
void SoundManager::PlayStartCountdown(float pitch, float pan) {
    result = system->playSound(startCountdownSound, 0, false, &channel);
    channel->setPitch(pitch);
    channel->setPan(pan);
}

//goal
void SoundManager::PlayGoalSound(float pitch, float pan) {
    result = system->playSound(goalSound, 0, false, &channel);
    channel->setPitch(pitch);
    channel->setPan(pan);
}

//button
void SoundManager::PlayButtonSound(float pitch, float pan) {
    result = system->playSound(buttonSound, 0, false, &channel);
    channel->setPitch(pitch);
    channel->setPan(pan);
}

void SoundManager::PlayCoinSound(float pan) {
    result = system->playSound(coinSound, 0, false, &channel);
    channel->setPan(pan);
}

void SoundManager::PlayBoxSound(float pan) {
    result = system->playSound(boxSound, 0, false, &channel);
    channel->setPan(pan);
}

void SoundManager::PlaySoundTrack() {
    result = system->playSound(sound2, 0, false, &channel);
}

void SoundManager::LoadSounds() {
    // result = system->createStream("../assets/659278__seth_makes_sounds__lofi-guitar-beat-70bpm.wav", FMOD_DEFAULT, 0, &sound2);
    // result = sound2->setMode(FMOD_LOOP_OFF);

    result = system->createSound("assets/128294__xenonn__layered-gunshot-2.wav", FMOD_DEFAULT, 0, &hitSound);
    result = hitSound->setMode(FMOD_LOOP_OFF);

    result = system->createStream("assets/main_menu_bgm.wav", FMOD_LOOP_NORMAL, 0, &mainMenuBgm);
    result = mainMenuBgm->setMode(FMOD_LOOP_NORMAL);

    result = system->createStream("assets/gameplay_bgm.wav", FMOD_LOOP_NORMAL, 0, &gameplayBgm);
    result = gameplayBgm->setMode(FMOD_LOOP_NORMAL);

    result = system->createSound("assets/start_countdown.wav", FMOD_DEFAULT, 0, &startCountdownSound);
    result = startCountdownSound->setMode(FMOD_LOOP_OFF);

    result = system->createSound("assets/goal_sound.wav", FMOD_DEFAULT, 0, &goalSound);
    result = goalSound->setMode(FMOD_LOOP_OFF);

    result = system->createSound("assets/car_drive.wav", FMOD_DEFAULT, 0, &carSound);
    result = carSound->setMode(FMOD_LOOP_NORMAL);

    result = system->createSound("assets/btn_pop.wav", FMOD_DEFAULT, 0, &buttonSound);
    result = buttonSound->setMode(FMOD_LOOP_OFF);

    result = system->createSound("assets/coin.wav", FMOD_DEFAULT, 0, &coinSound);
    result = buttonSound->setMode(FMOD_LOOP_OFF);

    result = system->createSound("assets/box.mp3", FMOD_DEFAULT, 0, &boxSound);
    result = buttonSound->setMode(FMOD_LOOP_OFF);
}

void SoundManager::UpdateSound() {
    result = system->update();
}

void SoundManager::StopBackgroundMusic() {
    if (channel) {
        channel->stop();
    }
    if (carChannel) {
        carChannel->stop();
    }
}


void SoundManager::StopCarSound() {
    if (carChannel) {
        bool isPlaying = false;
        carChannel->isPlaying(&isPlaying);
        if (isPlaying) {
            carChannel->stop();
        }
        carChannel = nullptr;
    }
}