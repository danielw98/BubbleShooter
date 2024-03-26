#include "bubble.h"
Bubble::Bubble(BubbleColor bubbleColor, SDL_FRect dstRect, bool isVisible)
{
    this->bubbleColor = bubbleColor;
    this->texture     = TextureManager::getInstance()->GetBubble(this->bubbleColor);
    this->dstRect     = dstRect;
    this->isVisible   = isVisible;
}

Bubble::~Bubble()
{
}