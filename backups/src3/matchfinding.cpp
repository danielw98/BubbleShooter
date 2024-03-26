
#include "matchfinding.h"

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
    memset(graph, 0, NUM_NODES*NUM_NODES*sizeof(int8_t));
    memset(matrix,0, BOARD_SIZE_X*BOARD_SIZE_Y*sizeof(int8_t));
}
Matchfinder::~Matchfinder()
{
    cout << "~Matchfinder()\n";
    instance = NULL;
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
}

int8_t Matchfinder::GetMatrix(int i, int j)
{
    return matrix[i][j];
}

void Matchfinder::Path_to_Directions(void)
{
    // int path_length = enemy->GetPathLength();
    // enemy->SetDirection(0, TD::EAST); // start going right
    // for (int i = 0; i < path_length-1; i++)
    // {
    //     int i1 = enemy->GetPath(i)   / NUM_NODES;
    //     int i2 = enemy->GetPath(i+1) / NUM_NODES;
    //     int j1 = enemy->GetPath(i)   % NUM_NODES;
    //     int j2 = enemy->GetPath(i+1) % NUM_NODES;

    //     if (j1 > j2)
    //         enemy->SetDirection(i+1, TD::WEST);
    //     else if (j1 < j2)
    //         enemy->SetDirection(i+1, TD::EAST);
    //     else if (i1 > i2)
    //         enemy->SetDirection(i+1, TD::NORTH);
    //     else if (i1 < i2)
    //         enemy->SetDirection(i+1, TD::SOUTH);
    // }
    // enemy->SetDirection(enemy->GetPathLength(), TD::EAST); // after last vertex turn right
}

bool Matchfinder::PathContains(int x, int y)
{
    for(int i = 0; i < path.size(); i++)
        if(path[i]->x == x && path[i]->y == y)
            return true;
    return false;
}

void Matchfinder::ClearPath(void)
{
    for(int i = 0; i < path.size(); i++)
        delete path[i];
    path.clear();
}
void Matchfinder::CheckForMatch(int new_i, int new_j)
{
    // transform the bubbles placed matrix in an adjacency matrix - should do this only once, and just update the graph accordingly
    //Matrix_to_Graph(); // sth like AddToGraph(new_i, new_j);
    
    
    printf("new_i = %d\tnew_j = %d\n", new_i, new_j);
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
           
            if(new_i % 2 == 0)
            {
                // (i-1, j)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i-1, new_j);
                if (new_i-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j] && PathContains(new_i-1, new_j) == false)
                        path.push_back(new Path(new_i-1, new_j, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i, j+1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i, new_j+1);
                if (new_j+1 < BOARD_SIZE_Y)
                    if(matrix[new_i][new_j] == matrix[new_i][new_j+1] && PathContains(new_i, new_j+1) == false)
                        path.push_back(new Path(new_i, new_j+1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i, j-1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i, new_j-1);
                if (new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i][new_j-1] && PathContains(new_i, new_j-1) == false)
                        path.push_back(new Path(new_i, new_j-1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i-1, j-1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i-1, new_j-1);
                if(new_i-1 >= 0 && new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j-1] && PathContains(new_i-1, new_j-1) == false)
                        path.push_back(new Path(new_i-1, new_j-1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i+1, j-1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i+1, new_j-1);
                if(new_i+1 < BOARD_SIZE_X && new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i+1][new_j-1] && PathContains(new_i+1, new_j-1) == false)
                        path.push_back(new Path(new_i+1, new_j-1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
            }
            else
            {
                // (i-1, j)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i-1, new_j);
                if (new_i-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j] && PathContains(new_i-1, new_j) == false)
                        path.push_back(new Path(new_i-1, new_j, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i, j+1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i, new_j+1);
                if (new_j+1 < BOARD_SIZE_Y)
                    if(matrix[new_i][new_j] == matrix[new_i][new_j+1] && PathContains(new_i, new_j+1) == false)
                        path.push_back(new Path(new_i, new_j+1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i, j-1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i, new_j-1);
                if (new_j-1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i][new_j-1] && PathContains(new_i, new_j-1) == false)
                        path.push_back(new Path(new_i, new_j-1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i-1, j+1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i-1, new_j+1);
                if(new_i-1 >= 0 && new_j+1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i-1][new_j+1] && PathContains(new_i-1, new_j+1) == false)
                        path.push_back(new Path(new_i-1, new_j+1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
                // (i+1, j+1)
                printf("checking if (%d, %d) has same color as (%d, %d): ", new_i, new_j, new_i+1, new_j+1);
                if(new_i+1 < BOARD_SIZE_X && new_j+1 >= 0)
                    if(matrix[new_i][new_j] == matrix[new_i+1][new_j+1] && PathContains(new_i+1, new_j+1) == false)
                        path.push_back(new Path(new_i+1, new_j+1, false)),
                        printf("yes\n");
                    else
                        printf("no\n");
            }
        }
        printf("path_size = %zu\tnum_visited = %d\n", path.size(), num_visited);
    }
    for(int i = 0; i < path.size(); i++)
        printf("(%d, %d) ", path[i]->x, path[i]->y);
    printf("\n");
}

void Matchfinder::Matrix_to_Graph()
{
    // will see if correct but it seems so
    for (int i = 0; i < BOARD_SIZE_X; i++)
    {
        for (int j = 0; j < BOARD_SIZE_Y; j++)
        {
            int elem_i = NUM_NODES * i + j;
            int elem_j;

            if (matrix[i][j] == -1)
                continue;
            // (i+1, j)
            if (i+1 < BOARD_SIZE_X){
                elem_j = (i+1) * BOARD_SIZE_X + j;
                if (matrix[i+1][j] != -1)
                    graph[elem_i][elem_j] = 1;
            }
            // (i-1, j)
            if (i-1 >= 0){
                elem_j = (i-1) * BOARD_SIZE_X + j;
                if (matrix[i-1][j] != -1)
                    graph[elem_i][elem_j] = 1;
            }
            // (i, j+1)
            if (j+1 < BOARD_SIZE_Y){
                elem_j = i * BOARD_SIZE_X + j+1;
                if (matrix[i][j+1] != -1)
                    graph[elem_i][elem_j] = 1;
            }
            // (i, j-1)
            if (j-1 >= 0){
                elem_j = i * BOARD_SIZE_X + j-1;
                if (matrix[i][j-1] != -1)
                    graph[elem_i][elem_j] = 1;
            }
            // (i-1, j-1)
            if(i-1 >= 0 && j-1 >= 0){
                elem_j = (i-1) * BOARD_SIZE_X + j-1;
                if (matrix[i-1][j-1] != -1)
                    graph[elem_i][elem_j] = 1;
            }
            // (i+1, j-1)
            if(i+1 < BOARD_SIZE_X && j-1 >= 0){
                elem_j = (i+1) * BOARD_SIZE_X + j-1;
                if (matrix[i+1][j-1] != -1)
                    graph[elem_i][elem_j] = 1;
            }
        }
    }
}