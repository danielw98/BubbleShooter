#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include "enums/BubbleColor.h"
#include "Defines.h"
#include "GUIManager.h"
#include <SDL2/SDL.h>

class TextureManager {
private:
    SDL_Texture *background;
    SDL_Texture *bubbles[NUM_BUBBLES];

public:
    static TextureManager* getInstance();
private:
    static TextureManager *instance;
    TextureManager();

public:
    SDL_Texture* GetBackground(void);
    SDL_Texture* GetBubble(BubbleColor bubbleColor);
    ~TextureManager();
private:
    SDL_Texture* CreateTextureFromImage(const char *path);
};

#endif // TEXTURE_MANAGER_H_