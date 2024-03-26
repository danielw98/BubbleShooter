#ifndef GUI_MANAGER_H_
#define GUI_MANAGER_H_

#include <SDL2/SDL.h>

class GUIManager {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

public:
    static GUIManager* getInstance();
private:
    static GUIManager *instance;
    GUIManager();

public:
    SDL_Renderer* GetRenderer(void);
    void SetRenderer(SDL_Renderer *renderer);
    void SetWindow(SDL_Window *window);
    ~GUIManager(); 
};

#endif // GUI_MANAGER_H_