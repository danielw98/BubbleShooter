#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "../include/GUIManager.h"

class Entity {
public:
    void Render();
    static void ToggleRectangleVisible();
    void SetTexture(SDL_Texture *texture);
    SDL_Texture* GetTexture(void);
    SDL_FRect GetDestRect(void);
    bool IsVisible(void);
    void SetVisible(bool visible);
    void SetDestRect(SDL_FRect rect);
protected:
    SDL_Texture *texture;
    SDL_FRect srcRect, dstRect;
    bool isVisible;
    static bool rectangleVisible;
};

#endif // _ENTITY_H_