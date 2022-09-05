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
#include "matchfinding.h"
#include "soundmanager.h"
using namespace std;

class Game
{
public:
    Game(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    ~Game();
    void Play(void);
private:
    void InitMap(void);
    void InitStats(void);
    void RotateBubbles(RotateDirection direction);
    void HandleEvents(void);
    void HandleMouseMotion(SDL_Event &event);
    void HandleMouseDown(SDL_Event &event);
    void HandleMouseUp(SDL_Event &event);
    void Render(void);
    void Update(void);
    void ShootBubbleInit(void);
    void ShootBubble(void);
    void ChangeLevel(int offset);
    BubbleColor RandomBubble(void);
    void DisplayCannonToBubbleVector(void);
    void DisplayScore(void);
    void DisplayTimer(void);
    void DisplayLevel(void);
    void DisplayBubbles(void);
    void DisplayConsecutiveStrikesRect(void);
    void DisplayBackground(void);
    bool InsideRect(int pointX, int pointY, SDL_Rect &rect);
    bool InsideRect(float pointX, float pointY, SDL_FRect &rect);
    void Normalize(SDL_FPoint &vec, int length = 600);
    void HandleCollision(float &x, float &y, Bubble *bubble);

// data section
private:
    // gamestate
    bool running;
    bool paused;
    bool frozen;
    time_t currentTime;
    int levelOffset;
    Uint32 level;
    Uint32 score;
    Uint64 currentFrame;
    Uint64 frozenFrame;
    Bubble *bubbles[BOARD_SIZE_X][BOARD_SIZE_Y];
    Bubble *cannonBubble, *spareBubble, *shotBubble;
    SDL_Rect consecutiveStrikesRect;
    SDL_Point cannonPoint, mousePoint;
    SDL_Point startPoint, endPoint;
    SDL_FPoint shotBubbleDirectionVector;
    int numBubblesLeft;
    int timer;
    float bubbleSpeed;
    int consecutiveStrikes;
    int strikesAfter5Consecutive;
    float scoreMultiplier;
    float t;
    bool rotateLeft, rotateRight;
    bool shouldShootBubble;
    bool shootingBubble;
    double velocity;
    double acceleration;
};

#endif // _GAME_H_