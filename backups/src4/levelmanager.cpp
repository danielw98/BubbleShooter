#include "levelmanager.h"

LevelManager* LevelManager::instance = NULL;
LevelManager::LevelManager()
{
    int8_t lvl1[BOARD_SIZE_X][BOARD_SIZE_Y] =
    {
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 2, 2, 1, 1, 1},
        {0, 1, 1, 1, 2, 2, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            level[0][i][j] = lvl1[i][j];
}

int8_t LevelManager::GetLevel(int l, int i, int j)
{
    return level[l-1][i][j];
}
void LevelManager::SetLevel(int l, int i, int j, int8_t val)
{
    level[l-1][i][j] = val;
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