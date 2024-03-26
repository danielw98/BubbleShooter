#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_
#include <SDL2/SDL_mixer.h>
#include "enums/SoundType.h"

#define NUM_SOUNDS 9

class SoundManager {
private:
    SDL_AudioSpec wavSpec[NUM_SOUNDS];
    SDL_AudioDeviceID deviceId[NUM_SOUNDS];
    Uint32 wavLength[NUM_SOUNDS];
    Uint8 *wavBuffer[NUM_SOUNDS];

public:
    static SoundManager* getInstance();
private:
    static SoundManager *instance;
    SoundManager();

public:
    void PlaySound(SoundType soundType);
    ~SoundManager();
private:
    void Load(const char *filename, SoundType soundType);
};

#endif /* _SOUNDMANAGER_H_ */