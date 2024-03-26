#include "../include/SoundManager.h"

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager()
{
    Load("assets/sounds/shoot_bubble.wav", SOUND_SHOOT_BUBBLE);
    Load("assets/sounds/failed_shot.wav", SOUND_FAILED_SHOT);
    Load("assets/sounds/combo.wav", SOUND_COMBO);
    Load("assets/sounds/combo_bonus1.wav", SOUND_COMBO_BONUS1);
    Load("assets/sounds/combo_bonus2.wav", SOUND_COMBO_BONUS2);
    Load("assets/sounds/fully_loaded.wav", SOUND_FULLY_LOADED);
    Load("assets/sounds/hit_wall.wav", SOUND_HIT_WALL);
    Load("assets/sounds/level_up.wav", SOUND_LEVEL_UP);
    Load("assets/sounds/reset_bonus.wav", SOUND_RESET_BONUS);
}

SoundManager* SoundManager::getInstance()
{
    if(instance == nullptr)
        instance = new SoundManager;
    return instance;
}

void SoundManager::Load(const char *filename, SoundType soundType)
{
    if(SDL_LoadWAV(filename, &wavSpec[soundType], &wavBuffer[soundType], &wavLength[soundType]) == NULL)
    {
        printf("Error loading file %s: %s", filename, SDL_GetError());
        exit(EXIT_FAILURE);
    }        
    deviceId[soundType] = SDL_OpenAudioDevice(NULL, 0, &wavSpec[soundType], NULL, 0);
}

void SoundManager::PlaySound(SoundType soundType)
{
    SDL_QueueAudio(deviceId[soundType], wavBuffer[soundType], wavLength[soundType]);
    SDL_PauseAudioDevice(deviceId[soundType], 0);
}

SoundManager::~SoundManager()
{
    for(int i = 0; i < NUM_SOUNDS; i++)
    {
        SDL_CloseAudioDevice(deviceId[i]);
        SDL_FreeWAV(wavBuffer[i]);
    }
}
