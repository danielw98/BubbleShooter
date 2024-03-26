#ifndef _MATCHFINDER_H_
#define _MATCHFINDER_H_

#include "structs/Path.h"
#include "enums/BubbleColor.h"
#include "Defines.h"
#include "Bubble.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <SDL2/SDL.h>
#include <unistd.h>

class MatchFinder {
private:
    int8_t graph[NUM_NODES][NUM_NODES];
    int8_t matrix[BOARD_SIZE_X][BOARD_SIZE_Y];
    int numBubbles;
    std::vector<Path*> path;

public:
    static MatchFinder* getInstance();
private:
    static MatchFinder *instance;
    MatchFinder();

public:
    bool CheckForMatch(int new_i, int new_j);
    bool BoardContains(BubbleColor bubbleColor);
    bool BoardIsEmpty(void);
    void ClearPath(Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y], int *pscore, int *pbubbles);
    void SetMatrix(int i, int j, BubbleColor bubbleColor);
    int8_t GetMatrix(int i, int j);
    void ZeroMatrix(void);
    void PrintMatrix(void);
    ~MatchFinder();
private:
    bool PathContains(std::vector<Path*> &p, int x, int y);
    bool IsIsolated(int x, int y);
};


#endif // _MATCHFINDER_H_