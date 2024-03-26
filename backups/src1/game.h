#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "singletons.h"
#include "structs.h"
#include "bubble.h"
#include "levelmanager.h"
using namespace std;

class Game
{
public:
    Game(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    ~Game();
    void Play(void);
private:
    void InitMap(void);
    void RotateBubbles(RotateDirection direction);
    void HandleEvents(void);
    void HandleMouseMotion(SDL_Event &event);
    void HandleMouseDown(SDL_Event &event);
    void HandleMouseUp(SDL_Event &event);
    void Render(void);
    void Update(void);
    void DisplayScore(void);
    void DisplayLevel(void);
    void DisplayBubbles(void);
    void DisplayBackground(void) {
        SDL_RenderCopy(GUIManager::getInstance()->GetRenderer(), TextureManager::getInstance()->GetBackground(), NULL, NULL);
    }
    bool InsideRect(int pointX, int pointY, SDL_Rect &rect);

// data section
private:
    // gamestate
    int mouseX, mouseY;
    bool running;
    Uint32 level;
    Uint32 score;
    Uint64 currentFrame;
    Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y];
    Bubble *cannonBubble, *spareBubble;
    
    bool mouseDown;
};

#endif // _GAME_H_