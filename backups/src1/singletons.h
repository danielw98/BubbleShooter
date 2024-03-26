#ifndef _SINGLETONS_H_
#define _SINGLETONS_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "defines.h"
#include "structs.h"
class FontRenderer
{
public:
    ~FontRenderer();
    static FontRenderer* getInstance();
    void DisplayText(const char *msg, int x, int y);
private:
    FontRenderer();
    static FontRenderer *instance;
    TTF_Font *font;
};

class TextureManager
{
public:
    ~TextureManager();
    static TextureManager* getInstance();
    SDL_Texture* GetBackground(void) { return background; }
    SDL_Texture* GetBubble(BubbleColor bubbleColor) 
    { 
        if(bubbleColor == BUBBLE_NONE) 
            return NULL; 
        return bubbles[bubbleColor]; 
    }
private:
    TextureManager();
    SDL_Texture* CreateTextureFromImage(const char *path);
private:
    static TextureManager *instance;
    SDL_Texture *background;
    SDL_Texture *bubbles[NUM_BUBBLES];
};

class GUIManager
{
public:
    ~GUIManager();
    static GUIManager* getInstance();
    SDL_Renderer* GetRenderer(void)          { return renderer; }
    void SetRenderer(SDL_Renderer *renderer) { this->renderer = renderer; }
    void SetWindow(SDL_Window *window)       { this->window = window; }
private:
    GUIManager();
    static GUIManager *instance;
    SDL_Window *window;
    SDL_Renderer *renderer;
};
#endif // _SINGLETONS_H_