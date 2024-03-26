
#include "matchfinding.h"
#include "levelmanager.h"
using namespace std;

#define INFINITY_NODE 255

Matchfinder* Matchfinder::instance = NULL;
Matchfinder* Matchfinder::getInstance()
{
    if(!instance)
        instance = new Matchfinder;
    return instance;
}
Matchfinder::Matchfinder()
{
    cout << "Matchfinder()\n";
    numBubbles = 0;
    memset(graph, 0, NUM_NODES*NUM_NODES*sizeof(int8_t));
    memset(matrix,0, BOARD_SIZE_X*BOARD_SIZE_Y*sizeof(int8_t));
}
Matchfinder::~Matchfinder()
{
    cout << "~Matchfinder()\n";
    instance = NULL;
}
bool Matchfinder::BoardContains(BubbleColor bubbleColor)
{
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            if(matrix[i][j] == (int8_t) bubbleColor)
                return true;
    return false;
}
void Matchfinder::ZeroMatrix(void)
{
    memset(matrix, 0, BOARD_SIZE_X*BOARD_SIZE_Y*sizeof(int8_t));
}

void Matchfinder::PrintMatrix(void)
{
    printf("Matrix:\n");
    for(int i = 0; i < BOARD_SIZE_X; i++)
    {
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            printf("%2i ", matrix[i][j]);
        printf("\n");
    }
}
void Matchfinder::SetMatrix(int i, int j, BubbleColor bubbleColor)
{
    matrix[i][j] = (int8_t) bubbleColor;
    if(bubbleColor != BUBBLE_NONE && bubbleColor != BUBBLE_TRANSPARENT)
        numBubbles++;
}

int8_t Matchfinder::GetMatrix(int i, int j)
{
    return matrix[i][j];
}

bool Matchfinder::PathContains(int x, int y)
{
    for(int i = 0; i < path.size(); i++)
        if(path[i]->x == x && path[i]->y == y)
            return true;
    return false;
}
// FIXME: checks if there exists a path between (x, y) and one of the middle bubbles
bool Matchfinder::IsIsolated(int x, int y)
{
    int num_neighbours = 0;
    
    // (i, j+1) - RIGHT
    if (y+1 < BOARD_SIZE_Y)
        if(matrix[x][y+1] != BUBBLE_NONE)
            num_neighbours++;
    // (i, j-1) - LEFT
    if (y-1 >= 0)
        if(matrix[x][y-1] != BUBBLE_NONE)
            num_neighbours++;
    // (i-1, j) - UP
    if (x-1 >= 0)
        if(matrix[x-1][y] != BUBBLE_NONE)
            num_neighbours++;
    // (i+1, j) - DOWN
    if(x+1 < BOARD_SIZE_X)
         if(matrix[x+1][y] != BUBBLE_NONE)
                num_neighbours++;
    
    if(x % 2 == 0){
        // (i-1, j-1) - BOTTOM RIGHT
        if(x-1 >= 0 && y-1 >= 0)
            if(matrix[x-1][y-1] != BUBBLE_NONE)
                num_neighbours++;
        // (i+1, j-1) - BOTTOM LEFT
        if(x+1 < BOARD_SIZE_X && y-1 >= 0)
            if(matrix[x+1][y-1] != BUBBLE_NONE)
                num_neighbours++;
    }
    else{
         // (i-1, j+1) - UP RIGHT
        if(x-1 >= 0 && y+1 >= 0)
            if(matrix[x-1][y+1] != BUBBLE_NONE)
                num_neighbours++;
        // (i+1, j+1) - BOTTOM RIGHT
        if(x+1 >= 0 && y+1 >= 0)
            if(matrix[x+1][y+1] != BUBBLE_NONE)
                num_neighbours++;
    }
    return num_neighbours == 0;
}


int Matchfinder::ClearPath(Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y])
{
    for(int i = 0; i < path.size(); i++)
    {
        int x = path[i]->x;
        int y = path[i]->y;
        // TODO: make the bubbles fall animation
        bubbles[x][y]->Clear();
        matrix[x][y] = (int8_t) BUBBLE_NONE;
        numBubbles--;
        delete path[i];
    }
    path.clear();
    // FIXME: check rather if a cluster of bubbles is isolated... uff
    // isIsolated should return true if no path to the center exists
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            if(bubbles[i][j]->GetTexture() != NULL && IsIsolated(i, j) == true)
            {   
                bubbles[i][j]->Clear();
                matrix[i][j] = (int8_t) BUBBLE_NONE;
                numBubbles--;
            }
        }
    return numBubbles;
}

bool Matchfinder::CheckForMatch(int new_i, int new_j)
{
    // we didn't visit the first node
    path.push_back(new Path(new_i, new_j, false));
    int num_visited = 0;
    while(path.size() > num_visited)
    {
        size_t size = path.size();
        // check all neighbours of new_i, new_j that they are the same color
        for(size_t i = 0; i < size; i++)
        { 
            if(path[i]->visited == true)
                continue;
            new_i = path[i]->x;
            new_j = path[i]->y;
            
            path[i]->visited = true;
            num_visited++;
            // (i, j+1) - RIGHT
            if (new_j+1 < BOARD_SIZE_Y)
                if(matrix[new_i][new_j] == matrix[new_i][new_j+1] && PathContains(new_i, new_j+1) == false)
                    path.push_back(new Path(new_i, new_j+1, false));
            // (i, j-1) - LEFT
            if (new_j-1 >= 0)
                if(matrix[new_i][new_j] == matrix[new_i][new_j-1] && PathContains(new_i, new_j-1) == false)
                    path.push_back(new Path(new_i, new_j-1, false));
            // (i-1, j) - UP
            if (new_i-1 >= 0)
                if(matrix[new_i][new_j] == matrix[new_i-1][new_j] && PathContains(new_i-1, new_j) == false)
                    path.push_back(new Path(new_i-1, new_j, false));
            // (i+1, j) - DOWN
            if(new_i+1 < BOARD_SIZE_X && new_j >= 0)
                if(matrix[new_i][new_j] == matrix[new_i+1][new_j] && PathContains(new_i+1, new_j) == false)
                    path.push_back(new Path(new_i+1, new_j, false));
            
            if(new_i % 2 == 0){
                // (i-1, j-1) - BOTTOM RIGHT
                if(new_i-1 >= 0 && new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j-1] && PathContains(new_i-1, new_j-1) == false)
                        path.push_back(new Path(new_i-1, new_j-1, false));
                // (i+1, j-1) - BOTTOM LEFT
                if(new_i+1 < BOARD_SIZE_X && new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i+1][new_j-1] && PathContains(new_i+1, new_j-1) == false)
                        path.push_back(new Path(new_i+1, new_j-1, false));
            }
            if(new_i % 2 == 1){
                // (i-1, j+1) - UP RIGHT
                if(new_i-1 >= 0 && new_j+1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j+1] && PathContains(new_i-1, new_j+1) == false)
                        path.push_back(new Path(new_i-1, new_j+1, false));
                // (i+1, j+1) - BOTTOM RIGHT
                if(new_i+1 >= 0 && new_j+1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i+1][new_j+1] && PathContains(new_i+1, new_j+1) == false)
                        path.push_back(new Path(new_i+1, new_j+1, false));
            }
        }
    }
    // for(int i = 0; i < path.size(); i++)
    //     printf("(%d, %d) ", path[i]->x, path[i]->y);
    // printf("\n");

    if(path.size() >= 3)
        return true;
    
    for(int i = 0; i < path.size(); i++)
        delete path[i];

    path.clear();
    return false;
}
