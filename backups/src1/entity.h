#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "singletons.h"
class Entity
{
public:
    Entity();
    ~Entity();
    void Render();
    void SetTexture(SDL_Texture *texture)
    {
        this->texture = texture;
    }
    SDL_Texture* GetTexture(void)
    {
        return texture;
    }
    SDL_FRect GetDestRect(void)
    {
        return dstRect;
    }
    bool IsVisible(void) 
    { 
        return isVisible; 
    }
    void SetDestRect(SDL_FRect rect)
    {
        dstRect = rect;
    }
protected:
    SDL_Texture *texture;
    SDL_FRect srcRect, dstRect;
    bool isVisible;
};
#endif // _ENTITY_H_