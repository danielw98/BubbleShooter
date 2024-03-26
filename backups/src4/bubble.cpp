#include "bubble.h"
Bubble::Bubble(BubbleColor bubbleColor, SDL_FRect dstRect, bool isVisible)
{
    this->bubbleColor = bubbleColor;
    this->texture     = TextureManager::getInstance()->GetBubble(this->bubbleColor);
    this->dstRect     = dstRect;
    this->isVisible   = isVisible;
}

void Bubble::Clear(void)
{
    SetVisible(false);
    SetTexture(TextureManager::getInstance()->GetBubble(BUBBLE_NONE));
    SetColor(BUBBLE_NONE);
}
Bubble::~Bubble()
{
}