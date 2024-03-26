#include "../include/LevelManager.h"

LevelManager* LevelManager::instance = nullptr;

LevelManager::LevelManager()
{
    FILE *fp = fopen("assets/levels/levels.txt", "r");
    if(fp == NULL){
        perror("assets/levels/levels.txt");
        exit(EXIT_FAILURE);
    }
    memset(numBubbles, 0, NUM_LEVELS_IMPLEMENTED*sizeof(numBubbles[0]));
    for(int l = 0; l < NUM_LEVELS_IMPLEMENTED; l++)
    {
        fscanf(fp,"\n");
        for(int i = 0; i < BOARD_SIZE_X; i++)
        {
            for(int j = 0; j < BOARD_SIZE_Y; j++)
            {
                if(j != BOARD_SIZE_Y-1)
                    fscanf(fp, "%hhd ", &level[l][i][j]);
                else
                    fscanf(fp, "%hhd\n", &level[l][i][j]);
                if(level[l][i][j] == 1)
                    numBubbles[l]++;
            }
        }
    }
    fclose(fp);
}

LevelManager* LevelManager::getInstance()
{
    if(!instance)
        instance = new LevelManager;
    return instance;
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
            MatchFinder::getInstance()->SetMatrix(i, j, bubbles[i][j]->GetColor());
            dstRect.x += BUBBLE_WIDTH;
        }
        dstRect.x = BUBBLE_OFFSET_X;
        dstRect.y += BUBBLE_HEIGHT - 5;
    }
}

int LevelManager::GetNumBubbles(int level)
{
    if(level > NUM_LEVELS_IMPLEMENTED)
    {
        fprintf(stderr, "Level %d has not been implemented yet - if you know a new board, mail me at wagner.daniel98@yahoo.com\n", level);
        exit(EXIT_FAILURE);
    }
    return numBubbles[level-1];
}

LevelManager::~LevelManager()
{
    // delete instance;
    instance = nullptr;
}
