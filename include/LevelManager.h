#ifndef _LEVELMANAGER_H_
#define _LEVELMANAGER_H_

#include "Defines.h"
#include "enums/BubbleColor.h"
#include "MatchFinder.h"
#include <iostream>

#define NUM_LEVELS_IMPLEMENTED 7
class LevelManager {
private:
    int8_t level[NUM_LEVELS_IMPLEMENTED][BOARD_SIZE_X][BOARD_SIZE_Y];
    int numBubbles[NUM_LEVELS_IMPLEMENTED];

public:
    static LevelManager* getInstance();
private:
    static LevelManager *instance;
    LevelManager();

public:
    int GetNumBubbles(int level);
    void SetLevel(int l, Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y]);
    ~LevelManager();
};

#endif /* _LEVELMANAGER_H_ */
