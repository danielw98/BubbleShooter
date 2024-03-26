#include "game.h"

Game::Game(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    Uint32 flags = 0;
    if(fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    
    // init SDL API
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
        running = false;
        return;
    }
    
    // init window and renderer
    SDL_Window *window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    GUIManager::getInstance()->SetWindow(window);
    GUIManager::getInstance()->SetRenderer(renderer);

    // init RNG
    srand(time(NULL));

    // init stats
    currentFrame = 0;
    running = true;
    level = 1;
    score = 0;
    mouseDown = true;
    // init other classes
    InitMap();
}

void Game::InitMap(void)
{
    SDL_FRect dstRect;
    dstRect.x = BUBBLE_OFFSET_X;
    dstRect.y = BUBBLE_OFFSET_Y;
    dstRect.w = BUBBLE_WIDTH;
    dstRect.h = BUBBLE_HEIGHT;
    BubbleColor bubbleColor[NUM_BUBBLES] = {BUBBLE_BLUE, BUBBLE_YELLOW, BUBBLE_GREEN, BUBBLE_RED, BUBBLE_TRANSPARENT};
    
    for(int i = 0; i < BOARD_SIZE_X; i++)
    {
        if(i % 2 == 1)
            dstRect.x += BUBBLE_WIDTH/2;
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            uint8_t bubbleType = LevelManager::getInstance()->GetLevel(1, i, j);
            if(bubbleType == 1)
                bubbles[i][j] = new Bubble(bubbleColor[rand() % 3], dstRect, true);
            else if(bubbleType == 2)
                bubbles[i][j] = new Bubble(BUBBLE_TRANSPARENT, dstRect, true);
            else
                bubbles[i][j] = new Bubble(BUBBLE_NONE, dstRect, true);
            dstRect.x += BUBBLE_WIDTH;
        }
        dstRect.x = BUBBLE_OFFSET_X;
        dstRect.y += BUBBLE_HEIGHT - 5;
    }

    SDL_FRect cannonBubbleDstRect;
    cannonBubbleDstRect.x = CANNON_BUBBLE_OFFSET_X;
    cannonBubbleDstRect.y = CANNON_BUBBLE_OFFSET_Y;
    cannonBubbleDstRect.w = CANNON_BUBBLE_WIDTH;
    cannonBubbleDstRect.h = CANNON_BUBBLE_HEIGHT;
    cannonBubble = new Bubble(bubbleColor[rand() % 3], cannonBubbleDstRect, true);

    SDL_FRect spareBubbleDstRect;
    spareBubbleDstRect.x = SPARE_BUBBLE_OFFSET_X;
    spareBubbleDstRect.y = SPARE_BUFFLE_OFFSET_Y;
    spareBubbleDstRect.w = SPARE_BUBBLE_W;
    spareBubbleDstRect.h = SPARE_BUBBLE_H;
    spareBubble = new Bubble(bubbleColor[rand() % 3], spareBubbleDstRect, true);
}


bool Game::InsideRect(int pointX, int pointY, SDL_Rect &rect)
{
    if (pointX >= rect.x && pointX <= (rect.x + rect.w))
        if (pointY >= rect.y && pointY <= (rect.y + rect.h))
            return true;
    return false;
}

void Game::DisplayScore(void)
{
    char text[32];
    sprintf(text, "%d", score);
    FontRenderer::getInstance()->DisplayText(text, SCORE_OFFSET_X, SCORE_OFFSET_Y);
}

void Game::DisplayLevel(void)
{
    char text[16];
    sprintf(text, "%d", level);
    FontRenderer::getInstance()->DisplayText(text, LEVEL_OFFSET_X, LEVEL_OFFSET_Y);
}

void Game::DisplayBubbles(void)
{
    
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            bubbles[i][j]->Render();
    cannonBubble->Render();
    spareBubble->Render();
}

Game::~Game()
{
    FontRenderer::getInstance()->~FontRenderer();
    TextureManager::getInstance()->~TextureManager();
    GUIManager::getInstance()->~GUIManager();
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            delete bubbles[i][j];
    delete cannonBubble;
    delete spareBubble;
    SDL_Quit();
}

void Game::Play(void)
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    Uint32 frameTime;
    while (running)
    {
        frameStart = SDL_GetTicks();

        HandleEvents();
        Update();
        Render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
        else
            // cout << "it takes too much time " << (int)frameDelay - (int)frameTime << "\n";
        currentFrame++;
    }
}

void Game::Render(void)
{
    SDL_RenderClear(GUIManager::getInstance()->GetRenderer());
    DisplayBackground();
    DisplayScore();
    DisplayLevel();
    DisplayBubbles();
    SDL_RenderPresent(GUIManager::getInstance()->GetRenderer());
}

void Game::Update(void)
{
    // update func
    if(mouseDown == true)
        RotateBubbles(ROTATE_LEFT);
}

void Game::HandleEvents(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:{
            running = false;
            break;
        }
        case SDL_KEYDOWN:{
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                // toggle bubbles
                SDL_FRect rect = cannonBubble->GetDestRect();
                cannonBubble->SetDestRect(spareBubble->GetDestRect());
                spareBubble->SetDestRect(rect);
                Bubble *tmp = cannonBubble;
                cannonBubble = spareBubble;
                spareBubble = tmp;
                break;
            }
            }
            break;
        }
        case SDL_MOUSEMOTION:{
            HandleMouseMotion(event);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:{
            HandleMouseDown(event);
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            HandleMouseUp(event);
            break;
        }
        default:{
            //printf("Unhandled event %d\n", event.type);
            break;
        }
        }
    }
}

void Game::HandleMouseMotion(SDL_Event &event)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    // handle mouse motion
}

void Game::RotateBubbles(RotateDirection direction)
{
    int sign;
    float dtheta;
    SDL_FPoint centerOfRotation;

    if(direction == ROTATE_LEFT)
        sign = -1;
    else if(direction == ROTATE_RIGHT)
        sign = 1;
    dtheta = sign * 0.01;
    centerOfRotation.x = bubbles[4][5]->GetDestRect().x;
    centerOfRotation.y = bubbles[4][5]->GetDestRect().y;
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            if(bubbles[i][j]->GetTexture() != NULL && bubbles[i][j]->IsVisible() == true)
            {
                SDL_FPoint vectorCenterToBubble;
                vectorCenterToBubble.x = bubbles[i][j]->GetDestRect().x - centerOfRotation.x;
                vectorCenterToBubble.y = bubbles[i][j]->GetDestRect().y - centerOfRotation.y;

                float radius = sqrt(vectorCenterToBubble.x * vectorCenterToBubble.x + vectorCenterToBubble.y * vectorCenterToBubble.y);
                float theta  = atan2(vectorCenterToBubble.y, vectorCenterToBubble.x);
                
                SDL_FRect newRect;
                newRect.x = centerOfRotation.x + radius * cos(theta + dtheta);
                newRect.y = centerOfRotation.y + radius * sin(theta + dtheta);
                newRect.w = bubbles[i][j]->GetDestRect().w;
                newRect.h = bubbles[i][j]->GetDestRect().h;
                
                bubbles[i][j]->SetDestRect(newRect);
            } 
}

void Game::HandleMouseDown(SDL_Event &event)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED)
    {
        // handle left mouse button press
        mouseDown = !mouseDown;
    }
}

void Game::HandleMouseUp(SDL_Event &event)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED)
    {
        // handle left mouse button release
        // mouseDown = false;
    }
}