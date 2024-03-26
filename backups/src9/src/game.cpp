#include "game.h"

void Game::InitStats(void)
{
    const char *path = "../assets/startup/config.txt";
    FILE *fp = fopen(path, "r");
    if(fp == NULL)
    {
        perror(path);
        exit(EXIT_FAILURE);
    }

    currentFrame = 0;
    frozenFrame = 0;
    strikesAfter5Consecutive = 0;
    consecutiveStrikes = 0;
    consecutiveStrikesRect.x = STRIKES_RECT_X;
    consecutiveStrikesRect.y = STRIKES_RECT_Y;
    consecutiveStrikesRect.w = STRIKES_RECT_W;
    consecutiveStrikesRect.h = 0;

    levelOffset = 0;
    t = 0;
    velocity = 0;
    acceleration = 0.005;

    running = true;
    paused = false;
    frozen = false;
    rotateLeft = false;
    rotateRight = false;
    shouldShootBubble = false;
    shootingBubble = false;
    // should the return value of fscanf() and the return value of the variables
    fscanf(fp, "level: %d\n", &level);
    fscanf(fp, "score: %d\n", &score);
    fscanf(fp, "score multiplier: %fx\n", &scoreMultiplier);
    fscanf(fp, "timer: %d seconds\n", &timer);
    fscanf(fp, "bubble speed: %f\n", &bubbleSpeed);
    fclose(fp);
    if(level != 1){
        printf("cheater, you want to start at level %d\n", level);
    }
    if(score != 0){
        printf("cheater, your score is %dx\n", score);
    }
    if(scoreMultiplier != 1){
        printf("cheater, your multipler is %fx\n", scoreMultiplier);
    }
    if(timer != 150){
        printf("cheater, you want to start with %d seconds\n", timer);
    }
    if(bubbleSpeed != 2.5){
        printf("okay, i can understand adjusting bubble speed to %d\n", (int)bubbleSpeed);
    }
    bubbleSpeed *= 0.01;
}

Game::Game(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    printf("Game()\n");
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
    currentTime = time(NULL);
    srand(currentTime);

    InitStats();
    InitMap();
}

void Game::InitMap(void)
{
    if(level <= 2)
    {
        numBubblesLeft = LevelManager::getInstance()->GetNumBubbles(level);
        LevelManager::getInstance()->SetLevel(level, bubbles);
    }
    else
    {
        // levelOffset is the offset in the levelarray because levels > 2 are random from levels (3,7) implemented
        levelOffset = 2 + rand() % (NUM_LEVELS_IMPLEMENTED-1);
        numBubblesLeft = LevelManager::getInstance()->GetNumBubbles(levelOffset);
        LevelManager::getInstance()->SetLevel(levelOffset, bubbles);
    }
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


Game::~Game()
{
    delete FontRenderer::getInstance();
    delete TextureManager::getInstance();
    delete GUIManager::getInstance();
    delete Matchfinder::getInstance();
    delete SoundManager::getInstance();
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
        HandleEvents();
        if(paused == false)
        {
            if(frozen == false)
                currentFrame++;
            else
                frozenFrame++;
            Update();
            Render();
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
        else
            ;
            // cout << "it takes too much time " << (int)frameDelay - (int)frameTime << "\n";
        
    }
    printf("Game over, your score is %d\n", score);
}

void Game::Render(void)
{
    SDL_RenderClear(GUIManager::getInstance()->GetRenderer());
    DisplayBackground();
    DisplayScore();
    DisplayLevel();
    DisplayTimer();
    DisplayConsecutiveStrikesRect();
    DisplayBubbles();
    DisplayCannonToBubbleVector();
    SDL_RenderDrawLineF(GUIManager::getInstance()->GetRenderer(), startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    SDL_RenderPresent(GUIManager::getInstance()->GetRenderer());
}

void Game::Normalize(SDL_FPoint &vec, int length)
{
    float norm = sqrt(vec.x*vec.x + vec.y*vec.y);
    vec.x = (vec.x * length) / norm;
    vec.y = (vec.y * length) / norm;
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
    Normalize(shotBubbleDirectionVector);
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

void Game::HandleCollision(float &x, float &y, Bubble *bubble)
{
    SDL_FRect bubbleDestRect = bubble->GetDestRect();
    SDL_FRect shotBubbleRect = shotBubble->GetDestRect();
    
    // now we're inside a bubble, go bck on the path until we find the rect for the bubble
    while(InsideRect(x, y, bubbleDestRect) == true)
    {
        t -= bubbleSpeed;
        x = startPoint.x + shotBubbleDirectionVector.x * t;
        y = startPoint.y + shotBubbleDirectionVector.y * t;
    }

    shotBubbleRect.x = x;
    shotBubbleRect.y = y;
    shotBubble->SetDestRect(shotBubbleRect);
    
    
    for(int new_i = 0; new_i < BOARD_SIZE_X; new_i++)
        for(int new_j = 0; new_j < BOARD_SIZE_Y; new_j++)
        {
            // check whether we are in this empty rect or not
            bubbleDestRect = bubbles[new_i][new_j]->GetDestRect();
            if(bubbles[new_i][new_j]->GetTexture() != NULL || InsideRect(x, y, bubbleDestRect) == false)
                continue;
            
            // we found the rect - now place the bubble in it
            bubbles[new_i][new_j]->SetColor(shotBubble->GetColor());
            bubbles[new_i][new_j]->SetTexture(shotBubble->GetTexture());
            bubbles[new_i][new_j]->SetVisible(true);

            // handle rotation of the wheel - setup the velocity and let the update function know
            // FIXME: handle rotations properly, find a function that takes [0,1] to [0,1] nicely
            double angle = atan2(shotBubbleDirectionVector.y, shotBubbleDirectionVector.x);
            angle += M_PI;
            double angle_in_degrees = angle*180/M_PI;
            if(angle >= 0 && angle < M_PI/4)
            {
                double currentVelocity = angle / (M_PI/4);
                velocity = currentVelocity;
                rotateLeft = true;
                // printf("2. should rotate left %lf with velocity %lf\n", angle_in_degrees, velocity);
            }
            else if(angle >= M_PI/4 && angle < M_PI/2)
            {
                double currentVelocity = (angle-M_PI/4) / (M_PI/4);
                velocity = 1 - currentVelocity;
                rotateRight = true;
                // printf("1. should rotate right %lf with velocity %lf\n", angle_in_degrees, velocity);
            }
            else if(angle >= M_PI/2 && angle < M_PI/2 + M_PI/4)
            {
                double currentVelocity = (angle-M_PI/2) / (M_PI/4);
                velocity = currentVelocity;
                rotateLeft = true;
                // printf("2. should rotate left %lf with velocity %lf\n", angle_in_degrees, velocity);
            }
            else if(angle >= M_PI/2 + M_PI/4 && angle < M_PI/2)
            {
                double currentVelocity = (angle-(M_PI/2 + M_PI/4)) / (M_PI/4);
                velocity = currentVelocity;
                rotateRight = true;
                // printf("2. should rotate right %lf with velocity %lf\n", angle_in_degrees, velocity);
            }
            else if(angle >= M_PI && angle < 3*M_PI/2)
            {
                double currentVelocity = (angle-M_PI) / (M_PI/2);
                velocity = 1 - currentVelocity;
                rotateRight = true;
                // printf("3. should rotate right %lf with velocity %lf\n", angle_in_degrees, velocity);
            }
            else if(angle >= 3*M_PI/2 && angle < 2*M_PI)
            {
                double currentVelocity = (angle-3*M_PI/2) / (M_PI/2);
                velocity = currentVelocity;
                rotateLeft = true;
                // printf("4. should rotate left %lf with velocity %lf\n", angle_in_degrees, velocity);
            }
            if(velocity > 0.5)
                velocity = 0.5;
            if(velocity > 0.1 && velocity < 0.3)
                velocity += velocity/2;
            
            // reset stuff
            t = 0;
            shootingBubble = false;
            shotBubble->SetVisible(false);

            // set the matchfinder table at the given position with the given color 
            Matchfinder::getInstance()->SetMatrix(new_i, new_j, bubbles[new_i][new_j]->GetColor());
            if(Matchfinder::getInstance()->CheckForMatch(new_i, new_j) == true)
            {
                if(consecutiveStrikes == 5)
                {
                    if(strikesAfter5Consecutive % 4 == 0 || strikesAfter5Consecutive % 4 == 1)
                        SoundManager::getInstance()->PlaySound(SOUND_COMBO_BONUS1);
                    if(strikesAfter5Consecutive % 4 == 2 || strikesAfter5Consecutive % 4 == 3)
                        SoundManager::getInstance()->PlaySound(SOUND_COMBO_BONUS2);
                    strikesAfter5Consecutive++;
                }
                else
                    SoundManager::getInstance()->PlaySound(SOUND_COMBO);
                // compute score and get the number of bubbles on the board
                int currentMoveScore;
                Matchfinder::getInstance()->ClearPath(bubbles, &currentMoveScore, &numBubblesLeft);
                score += scoreMultiplier * currentMoveScore;
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
                // increase the height of consecutiveStrikesRect and cosnecutiveStrikes and multiplier
                if(consecutiveStrikes < 5)
                {
                    consecutiveStrikes++;
                    if(consecutiveStrikes == 5)
                        SoundManager::getInstance()->PlaySound(SOUND_FULLY_LOADED);
                    consecutiveStrikesRect.h -= STRIKES_RECT_H;
                    scoreMultiplier = 1.5;
                }
                
            }
            else
            {
                if(consecutiveStrikes == 5)
                    SoundManager::getInstance()->PlaySound(SOUND_RESET_BONUS);
                else
                    SoundManager::getInstance()->PlaySound(SOUND_FAILED_SHOT);
                // erase the strikes accumulated if no match was made
                if(consecutiveStrikes > 0)
                {
                    scoreMultiplier = 1;
                    consecutiveStrikes = 0;
                    strikesAfter5Consecutive = 0;
                    consecutiveStrikesRect.h = 0;
                }
            }
            return;
        }
    t = 0;
    shootingBubble = false;
    shotBubble->SetVisible(false);
}
void Game::ShootBubble(void)
{
    static bool playSound = false;
    static bool playedSound = false;
    float x = startPoint.x + shotBubbleDirectionVector.x * t;
    float y = startPoint.y + shotBubbleDirectionVector.y * t;
    SDL_FRect shotBubbleRect = shotBubble->GetDestRect();
    shotBubbleRect.x = x - shotBubbleRect.w/2;
    shotBubbleRect.y = y - shotBubbleRect.h/2;
    shotBubble->SetDestRect(shotBubbleRect);
    if(x >= WALL_RIGHT_X)
    {
        playSound = true;
        if(playSound == true && playedSound == false){
            SoundManager::getInstance()->PlaySound(SOUND_HIT_WALL);
            playedSound = true;
        }
        // it's going to the right, make it go to the left
        double theta;
        theta = atan2(shotBubbleDirectionVector.y, shotBubbleDirectionVector.x);
        theta = M_PI - theta;
        startPoint.x = x;
        startPoint.y = y;
        // just need a point on the line at any distance
        endPoint.x = startPoint.x + 250 * cos(theta);
        endPoint.y = startPoint.y + 250 * sin(theta);
        shotBubbleDirectionVector.x = endPoint.x - startPoint.x;
        shotBubbleDirectionVector.y = endPoint.y - startPoint.y;
        Normalize(shotBubbleDirectionVector);
        t = 0;
    }
    else if(x <= WALL_LEFT_X)
    {
        playSound = true;
        if(playSound == true && playedSound == false){
            SoundManager::getInstance()->PlaySound(SOUND_HIT_WALL);
            playedSound = true;
        }
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
        Normalize(shotBubbleDirectionVector);
        t = 0;
    }
    else if(y <= WALL_TOP_Y)
    {
        playSound = true;
        if(playSound == true && playedSound == false){
            SoundManager::getInstance()->PlaySound(SOUND_HIT_WALL);
            playedSound = true;
        }
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
        Normalize(shotBubbleDirectionVector);
        t = 0;
    }
    else if(y >= WALL_BOTTOM_Y)
    {
        shotBubble->SetVisible(false);
        shootingBubble = false;
        t = 0;
        return;
    }
    else
    {
        if(playSound == true)
            playSound = false;
        if(playedSound == true)
            playedSound = false;
    }
    // iterate through the board to see if the point (x,y) is within any existing bubble
    for(int i = 0; i < BOARD_SIZE_X; i++)
        for(int j = 0; j < BOARD_SIZE_Y; j++)
        {
            SDL_FRect bubbleDestRect = bubbles[i][j]->GetDestRect();
            if(InsideRect(x, y, bubbleDestRect) == false)
                continue;
            if(bubbles[i][j]->GetColor() == BUBBLE_NONE)
                continue;
            // when the ball encounters an existing ball, place it in the rect right behind it on the trajectory
            HandleCollision(x, y, bubbles[i][j]);
            return;
        }
    t += bubbleSpeed;
}
void Game::ChangeLevel(int offset)
{
    frozen = true;
    if(level+offset < 1)
    {
        frozen = false;
        return;
    }
    if(level+offset > NUM_LEVELS)
    {
        printf("you have reached the final level\n");
        running = false;
        return;
    }
    level += offset;
    currentTime += offset;
    srand(currentTime);
    
    if(level <= 2)
        numBubblesLeft = LevelManager::getInstance()->GetNumBubbles(level);
    else
    {
        // levelOffset is the offset in the levelarray because levels > 2 are random from levels (3,7) implemented
        levelOffset = 2 + rand() % (NUM_LEVELS_IMPLEMENTED-1);
        numBubblesLeft = LevelManager::getInstance()->GetNumBubbles(levelOffset);
    }
}

void Game::Update(void)
{
    // refactor this later - rotation handling
    if(frozen == true && rotateLeft == true){
        rotateLeft = false;
    }
    if(frozen == true && rotateRight == true){
        rotateRight = false;
    }
    if(rotateLeft == true && frozen == false){
        RotateBubbles(ROTATE_LEFT);
    }
    if(rotateRight == true && frozen == false){
        RotateBubbles(ROTATE_RIGHT);
    }
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
    if(numBubblesLeft == 0){
        SoundManager::getInstance()->PlaySound(SOUND_LEVEL_UP);
        ChangeLevel(1);
    }
    if(frozen == true){
        // do some kind of transition between levels here
        if(frozenFrame == 30){
            frozen = false;
            frozenFrame = 0;
            if(level <= 2)
                LevelManager::getInstance()->SetLevel(level, bubbles);
            else
                LevelManager::getInstance()->SetLevel(levelOffset, bubbles);
        }
    }
    if(currentFrame % 60 == 0 && frozen == false){
        timer--;
    }
    if(timer == 0){
        running = false;
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
            switch (event.key.keysym.sym){
            case SDLK_SPACE:{
                // toggle bubbles
                SDL_FRect rect = cannonBubble->GetDestRect();
                cannonBubble->SetDestRect(spareBubble->GetDestRect());
                spareBubble->SetDestRect(rect);
                Bubble *tmp = cannonBubble;
                cannonBubble = spareBubble;
                spareBubble = tmp;
                break;
            }
            case SDLK_a:{
                // toggle left rotation
                rotateLeft = !rotateLeft;
                rotateRight = false;
                break;
            }
            case SDLK_d:{
                // toggle right rotation
                rotateRight = !rotateRight;
                rotateLeft = false;
                break;
            }
            case SDLK_p:{
                // toggle pause state
                paused = !paused;
                break;
            }
            case SDLK_r:{
                Entity::ToggleRectangleVisible();
                break;
            }
            case SDLK_LEFT:{
                ChangeLevel(-1);
                break;
            }
            case SDLK_RIGHT:{
                ChangeLevel(1);
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
    dtheta = sign * 0.10 * velocity;
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
    
    velocity -= acceleration;
    if(velocity <= 0)
    {
        if(rotateLeft == true)
            rotateLeft = false;
        if(rotateRight == true)
            rotateRight = false;
    }
}

void Game::HandleMouseDown(SDL_Event &event)
{
    SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
    if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED)
    {
        // handle left mouse button press
        //printf("%d %d\n", mousePoint.x, mousePoint.y);
        if(shootingBubble == false)
        {
            SoundManager::getInstance()->PlaySound(SOUND_SHOOT_BUBBLE);
            shouldShootBubble = true;
        }
    }
}

void Game::HandleMouseUp(SDL_Event &event)
{
    SDL_atomic_t t;
    SDL_GetMouseState(&mousePoint.x, &mousePoint.y);
    if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED)
    {
    }
}


// display functions - i don't think i have to update them
void Game::DisplayBackground(void) 
{
    SDL_RenderCopy(GUIManager::getInstance()->GetRenderer(), TextureManager::getInstance()->GetBackground(), NULL, NULL);
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
void Game::DisplayConsecutiveStrikesRect(void)
{
    SDL_SetRenderDrawColor(GUIManager::getInstance()->GetRenderer(), 210, 78, 170, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(GUIManager::getInstance()->GetRenderer(), &consecutiveStrikesRect);
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
    if(shotBubble->IsVisible() == true)
        shotBubble->Render();
}
void Game::DisplayCannonToBubbleVector(void)
{
    SDL_RenderDrawLine(GUIManager::getInstance()->GetRenderer(), cannonPoint.x, cannonPoint.y, mousePoint.x, mousePoint.y);
}