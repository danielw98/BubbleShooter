#include "../include/TextureManager.h"
#include <SDL2/SDL_image.h>

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager()
{
    background = CreateTextureFromImage("assets/images/background.png");
    char bubbleColors[NUM_BUBBLES][16] = {"blue", "yellow", "green", "red", "transparent"};
    for(int i = 0; i < NUM_BUBBLES; i++)
    {
        char path[64];
        sprintf(path, "assets/images/bubble_%s.png", bubbleColors[i]);
        bubbles[i] = CreateTextureFromImage(path);
    }
}

TextureManager* TextureManager::getInstance()
{
    if(!instance)
        instance = new TextureManager;
    return instance;
}

SDL_Texture* TextureManager::GetBackground(void) 
{ 
    return background; 
}

SDL_Texture* TextureManager::GetBubble(BubbleColor bubbleColor) 
{ 
    if(bubbleColor == BUBBLE_NONE) 
        return NULL; 
    return bubbles[bubbleColor]; 
}

SDL_Texture* TextureManager::CreateTextureFromImage(const char *path)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = IMG_Load(path);
    if(surface == NULL)
    {
        fprintf(stderr, "File %s not found\n", path);
        exit(EXIT_FAILURE);
    }
    texture = SDL_CreateTextureFromSurface(GUIManager::getInstance()->GetRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

TextureManager::~TextureManager()
{
    if(background)
        SDL_DestroyTexture(background);
    
    for(int i = 0; i < NUM_BUBBLES; i++)
    {
        if(bubbles[i])
            SDL_DestroyTexture(bubbles[i]);
    }

    // delete instance;
    instance = nullptr;
}
