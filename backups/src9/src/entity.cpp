#include "entity.h"
#include "singletons.h"
#include <iostream>
using namespace std;

Entity::Entity()
{
    
}

Entity::~Entity()
{
}
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