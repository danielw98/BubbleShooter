#ifndef _MATCHFINDER_H_
#define _MATCHFINDER_H_

#include "structs.h"
#include "defines.h"
#include "bubble.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <SDL2/SDL.h>
#include <unistd.h>
using namespace std;


class Matchfinder
{
public:
    bool CheckForMatch(int new_i, int new_j);
    static Matchfinder* getInstance();
    void ClearPath(Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y]);
    void SetMatrix(int i, int j, BubbleColor bubbleColor);
    int8_t GetMatrix(int i, int j);
    void ZeroMatrix(void);
    void PrintMatrix(void);
    ~Matchfinder();
private:
    Matchfinder();
    void Path_to_Directions(void);
    void Matrix_to_Graph(void);
    bool PathContains(int x, int y);
    bool IsIsolated(int x, int y);
    static Matchfinder *instance;
    int8_t graph[NUM_NODES][NUM_NODES];
    int8_t matrix[BOARD_SIZE_X][BOARD_SIZE_Y];
    vector<Path*> path;
};


#endif // _MATCHFINDER_H_