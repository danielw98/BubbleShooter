#ifndef FONT_RENDERER_H_
#define FONT_RENDERER_H_

#include <SDL2/SDL_ttf.h>

class FontRenderer {
private:
    TTF_Font *font;

public:
    static FontRenderer* getInstance();
private:
    static FontRenderer *instance;
    FontRenderer();

public:
    void DisplayText(const char *msg, int x, int y);
    ~FontRenderer();
};

#endif // FONT_RENDERER_H_