#ifndef PATH_H_
#define PATH_H_

struct Path
{
    Path(int x, int y, bool visited){ 
        this->x = x; 
        this->y = y; 
        this->visited = visited;
    }
    int x;
    int y;
    bool visited;
};

#endif // PATH_H_
