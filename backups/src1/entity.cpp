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
void Entity::Render()
{
    if(texture != NULL && isVisible == true)
    SDL_RenderCopyF(GUIManager::getInstance()->GetRenderer(), texture, NULL, &dstRect);
}