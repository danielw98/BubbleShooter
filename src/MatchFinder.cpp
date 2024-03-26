#include "../include/MatchFinder.h"
#include "../include/LevelManager.h"

#define INFINITY_NODE 255

MatchFinder* MatchFinder::instance = nullptr;

MatchFinder::MatchFinder()
{
    numBubbles = 0;
    memset(graph, 0, NUM_NODES*NUM_NODES*sizeof(int8_t));
    memset(matrix,0, BOARD_SIZE_X*BOARD_SIZE_Y*sizeof(int8_t));
}

MatchFinder* MatchFinder::getInstance()
{
    if(!instance)
        instance = new MatchFinder;
    return instance;
}

bool MatchFinder::BoardIsEmpty(void) 
{ 
    return numBubbles == 0; 
}

bool MatchFinder::BoardContains(BubbleColor bubbleColor)
{
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            if(matrix[i][j] == (int8_t) bubbleColor)
                return true;
    return false;
}

void MatchFinder::ZeroMatrix(void)
{
    memset(matrix, 0, BOARD_SIZE_X*BOARD_SIZE_Y*sizeof(int8_t));
}

void MatchFinder::PrintMatrix(void)
{
    printf("Matrix:\n");
    for(int i = 0; i < BOARD_SIZE_X; i++)
    {
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            printf("%2i ", matrix[i][j]);
        printf("\n");
    }
}

void MatchFinder::SetMatrix(int i, int j, BubbleColor bubbleColor)
{
    matrix[i][j] = (int8_t) bubbleColor;
    if(bubbleColor != BUBBLE_NONE && bubbleColor != BUBBLE_TRANSPARENT)
        numBubbles++;
}

int8_t MatchFinder::GetMatrix(int i, int j)
{
    return matrix[i][j];
}

bool MatchFinder::PathContains(std::vector<Path*> &p, int x, int y)
{
    for(int i = 0; i < p.size(); i++)
        if(p[i]->x == x && p[i]->y == y)
            return true;
    return false;
}

bool MatchFinder::IsIsolated(int x, int y)
{
    int num_visited = 0;
    bool containsTransparent = false;
    std::vector<Path*> tempPath;
    tempPath.push_back(new Path(x, y, false));
    while(tempPath.size() > num_visited && containsTransparent == false)
    {        
        size_t size = tempPath.size();
        // check all neighbours of x, y so that they dont have a transparent neighbour
        for(size_t i = 0; i < size; i++)
        { 
            if(containsTransparent == true)
                break;
            if(tempPath[i]->visited == true)
                continue;
            x = tempPath[i]->x;
            y = tempPath[i]->y;
            tempPath[i]->visited = true;
            num_visited++;

            // (i, j+1) - RIGHT
            if (y+1 < BOARD_SIZE_Y && matrix[x][y+1] != BUBBLE_NONE)
            {
                if(matrix[x][y+1] == BUBBLE_TRANSPARENT)
                    containsTransparent = true;
                else if(PathContains(tempPath, x, y+1) == false)
                    tempPath.push_back(new Path(x, y+1, false));
            }

            // (i, j-1) - LEFT
            if (y-1 >= 0 && matrix[x][y-1] != BUBBLE_NONE)
            {
                
                if(matrix[x][y-1] == BUBBLE_TRANSPARENT){
                    containsTransparent = true;
                }
                else if(PathContains(tempPath, x, y-1) == false)
                    tempPath.push_back(new Path(x, y-1, false));   
            }

            // (i-1, j) - UP
            if (x-1 >= 0 && matrix[x-1][y] != BUBBLE_NONE)
            {
                if(matrix[x-1][y] == BUBBLE_TRANSPARENT)
                    containsTransparent = true;
                else if(PathContains(tempPath, x-1, y) == false)
                    tempPath.push_back(new Path(x-1, y, false));
            }

            // (i+1, j) - DOWN
            if(x+1 < BOARD_SIZE_X && matrix[x+1][y] != BUBBLE_NONE)
            {
                if(matrix[x+1][y] == BUBBLE_TRANSPARENT)
                    containsTransparent = true;
                else if(PathContains(tempPath, x+1, y) == false)
                    tempPath.push_back(new Path(x+1, y, false));
            }
            
            if(x % 2 == 0)
            {
                // (i-1, j-1) - BOTTOM RIGHT
                if(x-1 >= 0 && y-1 >= 0 && matrix[x-1][y-1] != BUBBLE_NONE){
                    if(matrix[x-1][y-1] == BUBBLE_TRANSPARENT)
                        containsTransparent = true;
                    else if(PathContains(tempPath, x-1, y-1) == false)
                        tempPath.push_back(new Path(x-1, y-1, false));
                }
                // (i+1, j-1) - BOTTOM LEFT
                if(x+1 < BOARD_SIZE_X && y-1 >= 0 && matrix[x+1][y-1] != BUBBLE_NONE){
                    if(matrix[x+1][y-1] == BUBBLE_TRANSPARENT)
                        containsTransparent = true;
                    else  if(PathContains(tempPath, x+1, y-1) == false)
                        tempPath.push_back(new Path(x+1, y-1, false));
                }
            }
            else
            {
                // (i-1, j+1) - UP RIGHT
                if(x-1 >= 0 && y+1 >= 0 && matrix[x-1][y+1] != BUBBLE_NONE){
                    if(matrix[x-1][y+1] == BUBBLE_TRANSPARENT)
                        containsTransparent = true;
                    else if(PathContains(tempPath, x-1, y+1) == false)
                        tempPath.push_back(new Path(x-1, y+1, false));
                }
                // (i+1, j+1) - BOTTOM RIGHT
                if(x+1 >= 0 && y+1 >= 0 && matrix[x+1][y+1] != BUBBLE_NONE){
                    if(matrix[x+1][y+1] == BUBBLE_TRANSPARENT)
                        containsTransparent = true;
                    else if(PathContains(tempPath, x+1, y+1) == false)
                        tempPath.push_back(new Path(x+1, y+1, false));
                }
            }
        }
    }

    for(int i = 0; i < tempPath.size(); i++)
        delete tempPath[i];
    tempPath.clear();

    return !containsTransparent;
}

void MatchFinder::ClearPath(Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y], int *pscore, int *pbubbles)
{
    int startBubbles = numBubbles;
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
    
    for(int i = 0; i < BOARD_SIZE_X; i++)
    {
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            if(bubbles[i][j]->GetTexture() != NULL && IsIsolated(i, j) == true)
            {   
                bubbles[i][j]->Clear();
                matrix[i][j] = (int8_t) BUBBLE_NONE;
                numBubbles--;
            }
        }
    }
    
    // compute score for the move
    int bubblesBroken = startBubbles - numBubbles;
    int score = 100;
    int bubbleScore = 50;
    
    for(int i = 3; i < bubblesBroken && i < 6; i++)
    {
        score += bubbleScore;
        bubbleScore += 50;
    }

    for(int i = 6; i < bubblesBroken && i < 9; i++)
    {
        score += bubbleScore;
        bubbleScore += 100;
    }

    for(int i = 9; i < bubblesBroken && i < 13; i++)
    {
        score += bubbleScore;
        bubbleScore += 150;
    }

    for(int i = 13; i < bubblesBroken && i < 16; i++)
    {
        score += bubbleScore;
        bubbleScore += 200;
    }

    for(int i = 16; i < bubblesBroken && i < 17; i++)
    {
        score += bubbleScore;
        bubbleScore += 300;
    }

    for(int i = 17; i < bubblesBroken; i++)
    {
        score += bubbleScore;
    }

    // printf("score of %d for combo of %d\n", score, bubblesBroken);
    *pbubbles = numBubbles;
    *pscore = score;
}

bool MatchFinder::CheckForMatch(int new_i, int new_j)
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
                if(matrix[new_i][new_j] == matrix[new_i][new_j+1] && PathContains(path, new_i, new_j+1) == false)
                    path.push_back(new Path(new_i, new_j+1, false));
            
            // (i, j-1) - LEFT
            if (new_j-1 >= 0)
                if(matrix[new_i][new_j] == matrix[new_i][new_j-1] && PathContains(path, new_i, new_j-1) == false)
                    path.push_back(new Path(new_i, new_j-1, false));
            
            // (i-1, j) - UP
            if (new_i-1 >= 0)
                if(matrix[new_i][new_j] == matrix[new_i-1][new_j] && PathContains(path, new_i-1, new_j) == false)
                    path.push_back(new Path(new_i-1, new_j, false));
            
            // (i+1, j) - DOWN
            if(new_i+1 < BOARD_SIZE_X && new_j >= 0)
                if(matrix[new_i][new_j] == matrix[new_i+1][new_j] && PathContains(path, new_i+1, new_j) == false)
                    path.push_back(new Path(new_i+1, new_j, false));
            
            if(new_i % 2 == 0)
            {
                // (i-1, j-1) - BOTTOM RIGHT
                if(new_i-1 >= 0 && new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j-1] && PathContains(path, new_i-1, new_j-1) == false)
                        path.push_back(new Path(new_i-1, new_j-1, false));
                // (i+1, j-1) - BOTTOM LEFT
                if(new_i+1 < BOARD_SIZE_X && new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i+1][new_j-1] && PathContains(path, new_i+1, new_j-1) == false)
                        path.push_back(new Path(new_i+1, new_j-1, false));
            }

            if(new_i % 2 == 1)
            {
                // (i-1, j+1) - UP RIGHT
                if(new_i-1 >= 0 && new_j+1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j+1] && PathContains(path, new_i-1, new_j+1) == false)
                        path.push_back(new Path(new_i-1, new_j+1, false));
                // (i+1, j+1) - BOTTOM RIGHT
                if(new_i+1 >= 0 && new_j+1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i+1][new_j+1] && PathContains(path, new_i+1, new_j+1) == false)
                        path.push_back(new Path(new_i+1, new_j+1, false));
            }
        }
    }

    if(path.size() >= 3)
        return true;
    
    // gotta free the path here if we don't need it anymore
    for(int i = 0; i < path.size(); i++)
        delete path[i];
    path.clear();

    return false;
}

MatchFinder::~MatchFinder()
{
    // delete instance;
    instance = nullptr;
}
