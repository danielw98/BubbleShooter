#ifndef _STRUCTS_H
#define _STRUCTS_H

enum BubbleColor
{
    BUBBLE_NONE = -1,
    BUBBLE_BLUE,
    BUBBLE_YELLOW,
    BUBBLE_GREEN,
    BUBBLE_RED,
    BUBBLE_TRANSPARENT
};

enum RotateDirection
{
    ROTATE_LEFT,
    ROTATE_RIGHT
};

struct Path
{
    Path(int x, int y, bool visited){ 
        this->x = x; 
        this->y = y; 
        this->visited = visited;
    }
    int x;
    int y;
    bool visited;
};

struct Level
{
    int numBubbles;
    int startX;
    int startY;
    int endX;
    int endY;
    int centerX;
    int centerY;
};
#endif // _STRUCTS_H