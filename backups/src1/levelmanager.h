#ifndef _LEVELMANAGER_H_
#define _LEVELMANAGER_H_

#include "defines.h"
#include <iostream>
using namespace std;
class LevelManager
{
public:
    ~LevelManager();
    static LevelManager* getInstance();
    uint8_t GetLevel(int l, int i, int j);
private:
    LevelManager();
    static LevelManager *instance;
    int8_t level[NUM_LEVELS][BOARD_SIZE_X][BOARD_SIZE_Y];
};
#endif /* _LEVELMANAGER_H_ */