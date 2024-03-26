#ifndef _BUBBLE_H_
#define _BUBBLE_H_
#include "entity.h"
class Bubble : public Entity
{
public:
    Bubble(BubbleColor bubbleColor, SDL_FRect dstRect, bool isVisible = false);
    ~Bubble();
    BubbleColor GetColor(void) { return bubbleColor; }
    
    void SetColor(BubbleColor bubbleColor) { this->bubbleColor = bubbleColor; }
private:
    BubbleColor bubbleColor;
};
#endif /* _BUBBLE_H_ */