#ifndef _BUBBLE_H_
#define _BUBBLE_H_
#include "enums/BubbleColor.h"
#include "Entity.h"
#include "TextureManager.h"

class Bubble : public Entity {
public:
    Bubble(BubbleColor bubbleColor, SDL_FRect dstRect, bool isVisible = false);
    BubbleColor GetColor(void);
    void SetColor(BubbleColor bubbleColor);
    void Clear(void);
private:
    BubbleColor bubbleColor;
};
#endif /* _BUBBLE_H_ */