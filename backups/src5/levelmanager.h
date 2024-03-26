#ifndef _LEVELMANAGER_H_
#define _LEVELMANAGER_H_

#include "defines.h"
#include "bubble.h"
#include "matchfinding.h"
#include <iostream>
using namespace std;
class LevelManager
{
public:
    ~LevelManager();
    static LevelManager* getInstance();
    int8_t GetLevel(int l, int i, int j);
    int8_t GetNumBubbles(int level){
        return numBubbles[level-1];
    }
    void SetLevel(int l, Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y]);
private:
    LevelManager();
    static LevelManager *instance;
    int8_t level[NUM_LEVELS][BOARD_SIZE_X][BOARD_SIZE_Y];
    int8_t numBubbles[NUM_LEVELS];
};
#endif /* _LEVELMANAGER_H_ */