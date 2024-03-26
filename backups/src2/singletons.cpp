#include "singletons.h"
#include <iostream>
using namespace std;

FontRenderer* FontRenderer::instance = NULL;
FontRenderer::FontRenderer()
{
    if(TTF_Init() == -1){
        fprintf(stderr,"Could not initialize TTF library: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    font = TTF_OpenFont("../Sans.ttf", 36);
    if(font == NULL){
        fprintf(stderr, "Loading font failed: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    } 
}
FontRenderer::~FontRenderer()
{
    TTF_CloseFont(font);
    TTF_Quit();
    instance = NULL;
}

FontRenderer* FontRenderer::getInstance()
{
    if(!instance)
        instance = new FontRenderer;
    return instance;
}

void FontRenderer::DisplayText(const char *msg, int x, int y)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color color = {.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE};
    SDL_Rect rect;

    surface = TTF_RenderText_Solid(font, msg, color);
    texture = SDL_CreateTextureFromSurface(GUIManager::getInstance()->GetRenderer(), surface);

    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;
    
    SDL_RenderCopy(GUIManager::getInstance()->GetRenderer(), texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


TextureManager* TextureManager::instance = NULL;
TextureManager::TextureManager()
{
    background = CreateTextureFromImage("../assets/background.png");
    char bubbleColors[NUM_BUBBLES][16] = {"blue", "yellow", "green", "red", "transparent"};
    for(int i = 0; i < NUM_BUBBLES; i++)
    {
        char path[64];
        sprintf(path, "../assets/bubble_%s.png", bubbleColors[i]);
        bubbles[i] = CreateTextureFromImage(path);
    }
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
    instance = NULL;
}
TextureManager* TextureManager::getInstance()
{
    if(!instance)
        instance = new TextureManager;
    return instance;
}

SDL_Texture* TextureManager::CreateTextureFromImage(const char *path)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = IMG_Load(path);
    if(surface == NULL){
        fprintf(stderr, "File %s not found\n", path);
        exit(EXIT_FAILURE);
    }
    texture = SDL_CreateTextureFromSurface(GUIManager::getInstance()->GetRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

GUIManager* GUIManager::instance = NULL;
GUIManager* GUIManager::getInstance()
{
    if(instance == NULL)
        instance = new GUIManager;
    return instance;
}

GUIManager::GUIManager()
{
    window = NULL;
    renderer = NULL;
}

GUIManager::~GUIManager()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    instance = NULL;
}