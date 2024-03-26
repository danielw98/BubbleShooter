#include "levelmanager.h"

LevelManager* LevelManager::instance = NULL;
LevelManager::LevelManager()
{
    int8_t lvl1[BOARD_SIZE_X][BOARD_SIZE_Y] =
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    numBubbles[0] = 56;
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            level[0][i][j] = lvl1[i][j];

    int8_t lvl2[BOARD_SIZE_X][BOARD_SIZE_Y] =
    {
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 2, 2, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 2, 2, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            level[1][i][j] = lvl2[i][j];
    numBubbles[1] = 80;
    int8_t lvl3[BOARD_SIZE_X][BOARD_SIZE_Y] =
    {
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            level[2][i][j] = lvl3[i][j];
    numBubbles[2] = 122;
}

int8_t LevelManager::GetLevel(int l, int i, int j)
{
    return level[l-1][i][j];
}

void LevelManager::SetLevel(int l, Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y])
{
    SDL_FRect dstRect;
    dstRect.x = BUBBLE_OFFSET_X;
    dstRect.y = BUBBLE_OFFSET_Y;
    dstRect.w = BUBBLE_WIDTH;
    dstRect.h = BUBBLE_HEIGHT;
    BubbleColor bubbleColor[NUM_BUBBLES] = {BUBBLE_BLUE, BUBBLE_YELLOW, BUBBLE_GREEN, BUBBLE_RED, BUBBLE_TRANSPARENT};
    for(int i = 0; i < BOARD_SIZE_X; i++)
    {
        if(i % 2 == 1)
            dstRect.x += BUBBLE_WIDTH/2;
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            uint8_t bubbleType = level[l-1][i][j];
            if(bubbleType == 1)
                bubbles[i][j] = new Bubble(bubbleColor[rand() % 3], dstRect, true);
            else if(bubbleType == 2)
                bubbles[i][j] = new Bubble(BUBBLE_TRANSPARENT, dstRect, true);
            else if (bubbleType == 0)
                bubbles[i][j] = new Bubble(BUBBLE_NONE, dstRect, false);
            Matchfinder::getInstance()->SetMatrix(i, j, bubbles[i][j]->GetColor());
            dstRect.x += BUBBLE_WIDTH;
        }
        dstRect.x = BUBBLE_OFFSET_X;
        dstRect.y += BUBBLE_HEIGHT - 5;
    }
}
LevelManager* LevelManager::getInstance()
{
    if(!instance)
        instance = new LevelManager;
    return instance;
}

LevelManager::~LevelManager()
{
    instance = NULL;
}