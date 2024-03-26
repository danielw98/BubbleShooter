#ifndef _LEVELMANAGER_H_
#define _LEVELMANAGER_H_

#include "defines.h"
#include "bubble.h"
#include "matchfinding.h"
#include <iostream>
using namespace std;

#define NUM_LEVELS_IMPLEMENTED 7
class LevelManager
{
public:
    ~LevelManager();
    static LevelManager* getInstance();
    int GetNumBubbles(int level)
    {
        if(level > NUM_LEVELS_IMPLEMENTED)
        {
            fprintf(stderr, "Level %d has not been implemented yet - if you know a new board, mail me at wagner.daniel98@yahoo.com\n", level);
            exit(EXIT_FAILURE);
        }
        return numBubbles[level-1];
    }
    void SetLevel(int l, Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y]);
private:
    LevelManager();
    static LevelManager *instance;
    int8_t level[NUM_LEVELS_IMPLEMENTED][BOARD_SIZE_X][BOARD_SIZE_Y];
    int numBubbles[NUM_LEVELS_IMPLEMENTED];
};
#endif /* _LEVELMANAGER_H_ */