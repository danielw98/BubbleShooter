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
    //srand(time(NULL));

    // init stats
    currentFrame = 0;
    frozenFrame = 0;
    frozen = false;
    running = true;
    level = 1;
    score = 0;
    timer = 150;
    mouseDown = false;
    shouldShootBubble = false;
    shootingBubble = false;
    num_bubbles_left = LevelManager::getInstance()->GetNumBubbles(level);
    // init other stuff
    InitMap();  
}

void Game::InitMap(void)
{
    LevelManager::getInstance()->SetLevel(level, bubbles);

    SDL_FRect cannonBubbleDstRect;
    cannonBubbleDstRect.x = CANNON_BUBBLE_OFFSET_X;
    cannonBubbleDstRect.y = CANNON_BUBBLE_OFFSET_Y;
    cannonBubbleDstRect.w = CANNON_BUBBLE_WIDTH;
    cannonBubbleDstRect.h = CANNON_BUBBLE_HEIGHT;
    cannonBubble = new Bubble(RandomBubble(), cannonBubbleDstRect, true);
    SDL_FRect spareBubbleDstRect;
    spareBubbleDstRect.x = SPARE_BUBBLE_OFFSET_X;
    spareBubbleDstRect.y = SPARE_BUFFLE_OFFSET_Y;
    spareBubbleDstRect.w = SPARE_BUBBLE_W;
    spareBubbleDstRect.h = SPARE_BUBBLE_H;
    spareBubble = new Bubble(RandomBubble(), spareBubbleDstRect, true);

    cannonPoint.x = cannonBubbleDstRect.x + cannonBubbleDstRect.w/2;
    cannonPoint.y = cannonBubbleDstRect.y + cannonBubbleDstRect.h/2;
    startPoint = cannonPoint;
    shotBubble = new Bubble(BUBBLE_NONE, cannonBubbleDstRect, false);
}


BubbleColor Game::RandomBubble()
{
    int freq[NUM_BUBBLES];
    memset(freq, 0, NUM_BUBBLES*sizeof(int));
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            if(bubbles[i][j]->GetTexture() == NULL)
                continue;
            
            freq[bubbles[i][j]->GetColor()]++;
        }
    int randomBubble;
    while(1)
    {
        randomBubble = rand() % NUM_BUBBLES - 1;
        if(freq[randomBubble] > 0)
            break;
    }
    return (BubbleColor) randomBubble;
}
bool Game::InsideRect(int pointX, int pointY, SDL_Rect &rect)
{
    if (pointX >= rect.x && pointX <= (rect.x + rect.w))
        if (pointY >= rect.y && pointY <= (rect.y + rect.h))
            return true;
    return false;
}
bool Game::InsideRect(float pointX, float pointY, SDL_FRect &rect)
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

void Game::DisplayTimer(void)
{
    char text[16];
    sprintf(text, "%02d:%02d", timer / 60, timer % 60);
    FontRenderer::getInstance()->DisplayText(text, TIMER_OFFSET_X, TIMER_OFFSET_Y);
}
void Game::DisplayBubbles(void)
{
    
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
            bubbles[i][j]->Render();
    cannonBubble->Render();
    spareBubble->Render();
}

void Game::DisplayCannonToBubbleVector(void)
{
    SDL_RenderDrawLine(GUIManager::getInstance()->GetRenderer(), cannonPoint.x, cannonPoint.y, mousePoint.x, mousePoint.y);
}

void Game::DisplayShotBubble(void)
{
    // if there's nothing to display, return
    if(shotBubble->IsVisible())
        shotBubble->Render();
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
    delete shotBubble;
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
        if(frozen == false)
            currentFrame++;
        else
            frozenFrame++;
        HandleEvents();
        Update();
        Render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
        else
            ;
            // cout << "it takes too much time " << (int)frameDelay - (int)frameTime << "\n";
        
    }
}

void Game::Render(void)
{
    SDL_RenderClear(GUIManager::getInstance()->GetRenderer());
    DisplayBackground();
    DisplayScore();
    DisplayLevel();
    DisplayTimer();
    DisplayBubbles();
    DisplayCannonToBubbleVector();
    DisplayShotBubble();
    SDL_RenderDrawLineF(GUIManager::getInstance()->GetRenderer(), startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    SDL_RenderPresent(GUIManager::getInstance()->GetRenderer());
}

void Game::ShootBubbleInit(void)
{
    // create the bubble that we shoot and set its direction
    SDL_FRect cannonBubbleDestRect = cannonBubble->GetDestRect();
    cannonBubbleDestRect.w = BUBBLE_WIDTH;
    cannonBubbleDestRect.h = BUBBLE_HEIGHT;
    shotBubble->SetColor(cannonBubble->GetColor());
    shotBubble->SetDestRect(cannonBubbleDestRect);
    shotBubble->SetTexture(cannonBubble->GetTexture());
    shotBubble->SetVisible(true);
    shotBubbleDirectionVector.x = mousePoint.x - cannonPoint.x;
    shotBubbleDirectionVector.y = mousePoint.y - cannonPoint.y;
    // move the bubble from spare position to cannon
    cannonBubble->SetColor(spareBubble->GetColor());
    cannonBubble->SetTexture(spareBubble->GetTexture());
    // create a new spare bubble
    BubbleColor bubbleColor = RandomBubble();
    spareBubble->SetColor(bubbleColor);
    spareBubble->SetTexture(TextureManager::getInstance()->GetBubble(bubbleColor));
    // letting the program know that we started shooting a bubble
    shootingBubble = true;
}

void Game::ShootBubble(void)
{
    static float t = 0;
    float x = startPoint.x + shotBubbleDirectionVector.x * t;
    float y = startPoint.y + shotBubbleDirectionVector.y * t;
    SDL_FRect shotBubbleRect = shotBubble->GetDestRect();
    shotBubbleRect.x = x - shotBubbleRect.w/2;
    shotBubbleRect.y = y - shotBubbleRect.h/2;
    shotBubble->SetDestRect(shotBubbleRect);
 
    if(x >= WALL_RIGHT_X)
    {
        //printf("%f >= WALL_RIGHT_X\n", x);
        // it's going to the right, make it go to the left
        double theta;
        theta = atan2(shotBubbleDirectionVector.y, shotBubbleDirectionVector.x);
        theta = M_PI - theta;
        startPoint.x = x;
        startPoint.y = y;
        // FIXME: it's a ridiculous number, use y = mx + n to compute the point instead
        endPoint.x = startPoint.x + 250 * cos(theta);
        endPoint.y = startPoint.y + 250 * sin(theta);
        shotBubbleDirectionVector.x = endPoint.x - startPoint.x;
        shotBubbleDirectionVector.y = endPoint.y - startPoint.y;
        t = 0;
    }
    else if(x <= WALL_LEFT_X)
    {
        // it's going to the left, make it go to the right
        double theta;
        theta = atan2(shotBubbleDirectionVector.y, shotBubbleDirectionVector.x);
        theta = M_PI - theta;
        startPoint.x = x + 3;
        startPoint.y = y;
        endPoint.x = startPoint.x + 250 * cos(theta);
        endPoint.y = startPoint.y + 250 * sin(theta);
        shotBubbleDirectionVector.x = endPoint.x - startPoint.x;
        shotBubbleDirectionVector.y = endPoint.y - startPoint.y;
        t = 0;
    }
    else if(y <= WALL_TOP_Y)
    {
        // it's going to the top, make it go to the bottom
        double theta;
        theta = atan2(shotBubbleDirectionVector.y, shotBubbleDirectionVector.x);
        theta = 2*M_PI - theta;
        startPoint.x = x;
        startPoint.y = y;
        endPoint.x = startPoint.x + 250 * cos(theta);
        endPoint.y = startPoint.y + 250 * sin(theta);
        shotBubbleDirectionVector.x = endPoint.x - startPoint.x;
        shotBubbleDirectionVector.y = endPoint.y - startPoint.y;
        t = 0;
    }
    else if(y >= WALL_BOTTOM_Y)
    {
        shotBubble->SetVisible(false);
        shootingBubble = false;
        t = 0;
        return;
    }
    t += 0.025; // change the speed here

    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            SDL_FRect bubbleDestRect = bubbles[i][j]->GetDestRect();
            // when the ball encounters an existing ball, place it in the rect right behind it on the trajectory
            if(InsideRect(x, y, bubbleDestRect) == false)
                continue;
            if(bubbles[i][j]->GetColor() == BUBBLE_NONE)
                continue;
            // now we're inside a bubble, go back on the path until we find the rect for the bubble
            // going back 0.05 seems decent as it's 2x more than what we traveled last step
            t -= 0.05;
            x = startPoint.x + shotBubbleDirectionVector.x * t;
            y = startPoint.y + shotBubbleDirectionVector.y * t;
        
            shotBubbleRect.x = x; // - shotBubbleRect.w/2;
            shotBubbleRect.y = y; // - shotBubbleRect.h/2;
            shotBubble->SetDestRect(shotBubbleRect);
            
            // check in which rect we are after going back 
            for(int new_i = 0; new_i < BOARD_SIZE_X; new_i++)
                for(int new_j = 0; new_j < BOARD_SIZE_Y; new_j++)
                {
                    bubbleDestRect = bubbles[new_i][new_j]->GetDestRect();
                    SDL_Rect bubbleDestRect_i = {.x = (int) bubbleDestRect.x, .y = (int) bubbleDestRect.y, .w = (int) bubbleDestRect.w, .h = (int) bubbleDestRect.h};
                    SDL_Rect shotBubbleRect_i = {.x = (int) shotBubbleRect.x, .y = (int) shotBubbleRect.y, .w = (int) shotBubbleRect.w, .h = (int) shotBubbleRect.h};
                    SDL_Rect pointRect;
                    pointRect.w = 1;
                    pointRect.h = 1;
                    pointRect.x = x - pointRect.w/2;
                    pointRect.y = y - pointRect.h/2;
                    
                    if(bubbles[new_i][new_j]->GetTexture() == NULL && SDL_HasIntersection(&pointRect, &bubbleDestRect_i))
                    {
                        bubbles[new_i][new_j]->SetColor(shotBubble->GetColor());
                        bubbles[new_i][new_j]->SetTexture(shotBubble->GetTexture());
                        bubbles[new_i][new_j]->SetVisible(true);

                        t = 0;
                        shootingBubble = false;
                        shotBubble->SetVisible(false);

                        Matchfinder::getInstance()->SetMatrix(new_i, new_j, bubbles[new_i][new_j]->GetColor());
                        if(Matchfinder::getInstance()->CheckForMatch(new_i, new_j) == true)
                        {
                            num_bubbles_left = Matchfinder::getInstance()->ClearPath(bubbles);
                            // create a new spare bubble if the current one isnt on the board anymore
                            if(Matchfinder::getInstance()->BoardContains(spareBubble->GetColor()) == false && Matchfinder::getInstance()->BoardIsEmpty() == false){
                                BubbleColor bubbleColor = RandomBubble();
                                spareBubble->SetColor(bubbleColor);
                                spareBubble->SetTexture(TextureManager::getInstance()->GetBubble(bubbleColor));
                            }
                            // create a new cannon bubble if the current one isnt on the board anymore
                            if(Matchfinder::getInstance()->BoardContains(cannonBubble->GetColor()) == false && Matchfinder::getInstance()->BoardIsEmpty() == false){
                                BubbleColor bubbleColor = RandomBubble();
                                cannonBubble->SetColor(bubbleColor);
                                cannonBubble->SetTexture(TextureManager::getInstance()->GetBubble(bubbleColor));
                            }
                        }
                        return;
                    }
                }
            t = 0;
            shootingBubble = false;
            shotBubble->SetVisible(false);
            return;
        }
}


void Game::Update(void)
{
    // update func
    if(mouseDown == true)
        RotateBubbles(ROTATE_LEFT);
    if(shouldShootBubble == true && shootingBubble == false)
    {
        ShootBubbleInit();
        shouldShootBubble = false;
        shootingBubble = true;
        startPoint = cannonPoint;
        endPoint = mousePoint;
    }
    if(shootingBubble == true){
        ShootBubble();
    }
    
    if(num_bubbles_left == 0){
        frozen = true;
        printf("level %d over\n", level);
        level++;
        num_bubbles_left = LevelManager::getInstance()->GetNumBubbles(level);
    }
    if(frozen == true){
        // do some kind of transition between levels here
        if(frozenFrame == 120){
            frozen = false;
            frozenFrame = 0;
            LevelManager::getInstance()->SetLevel(level, bubbles);
        }
    }
    if(currentFrame % 60 == 0 && frozen == false){
        timer--;
    }
    
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
            case SDLK_r:
            {
                // toggle rotation
                mouseDown = !mouseDown;
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
    SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
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
    centerOfRotation.x = bubbles[LVL1_CENTER_X][LVL1_CENTER_Y]->GetDestRect().x;
    centerOfRotation.y = bubbles[LVL1_CENTER_X][LVL1_CENTER_Y]->GetDestRect().y;
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
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
    SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
    if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED)
    {
        // handle left mouse button press
        // printf("%d %d\n", mousePoint.x, mousePoint.y);
    }
}

void Game::HandleMouseUp(SDL_Event &event)
{
    SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
    if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED)
    {
        if(shootingBubble == false)
            shouldShootBubble = true;
    }
}