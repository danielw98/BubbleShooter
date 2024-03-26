#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_
#include <SDL2/SDL_mixer.h>
#define NUM_SOUNDS 9
enum SoundType
{
    SOUND_SHOOT_BUBBLE,
    SOUND_FAILED_SHOT,
    SOUND_COMBO,
    SOUND_COMBO_BONUS1,
    SOUND_COMBO_BONUS2,
    SOUND_FULLY_LOADED,
    SOUND_HIT_WALL,
    SOUND_LEVEL_UP,
    SOUND_RESET_BONUS
};
class SoundManager
{
private:
    SoundManager();
    void Load(const char *filename, SoundType soundType);
public:
    ~SoundManager();
    static SoundManager* getInstance();
    void PlaySound(SoundType soundType);
public:
    static SoundManager *instance;
private:
    SDL_AudioSpec wavSpec[NUM_SOUNDS];
    SDL_AudioDeviceID deviceId[NUM_SOUNDS];
    Uint32 wavLength[NUM_SOUNDS];
    Uint8 *wavBuffer[NUM_SOUNDS];
};
#endif /* _SOUNDMANAGER_H_ */