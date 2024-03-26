#include "../include/GUIManager.h"

GUIManager* GUIManager::instance = nullptr;

GUIManager::GUIManager()
{
    window = NULL;
    renderer = NULL;
}

GUIManager* GUIManager::getInstance()
{
    if(instance == nullptr)
        instance = new GUIManager;
    return instance;
}

SDL_Renderer* GUIManager::GetRenderer(void)          
{ 
    return renderer;
}

void GUIManager::SetRenderer(SDL_Renderer *renderer) 
{ 
    this->renderer = renderer; 
}

void GUIManager::SetWindow(SDL_Window *window)       
{ 
    this->window = window; 
}

GUIManager::~GUIManager()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    // delete instance;
    instance = nullptr;
}