#include "../include/Entity.h"

bool Entity::rectangleVisible = false;

void Entity::Render()
{
    if(texture != NULL && isVisible == true)
        SDL_RenderCopyF(GUIManager::getInstance()->GetRenderer(), texture, NULL, &dstRect);
    
    if(rectangleVisible == true)
    {
        SDL_SetRenderDrawColor(GUIManager::getInstance()->GetRenderer(), 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRectF(GUIManager::getInstance()->GetRenderer(), &dstRect);
    }
}

void Entity::ToggleRectangleVisible()
{
    rectangleVisible = !rectangleVisible;
}

void Entity::SetTexture(SDL_Texture *texture)
{
    this->texture = texture;
}

SDL_Texture* Entity::GetTexture(void)
{
    return texture;
}

SDL_FRect Entity::GetDestRect(void)
{
    return dstRect;
}

bool Entity::IsVisible(void) 
{ 
    return isVisible; 
}

void Entity::SetVisible(bool visible) 
{ 
    isVisible = visible; 
}

void Entity::SetDestRect(SDL_FRect rect)
{
    dstRect = rect;
}
