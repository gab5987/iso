#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL.h>

#include "defs.h"
#include "global.h"

class Texture
{
    private:
    inline static Global &global = Global::getInstance();
    i32                   x;
    i32                   y;
    i32                   width;
    i32                   height;
    f64                   angle;

    SDL_Point       *center;
    SDL_Rect        *cliprect;
    SDL_RendererFlip fliptype;
    SDL_Texture     *texture;

    public:
    i32  load(char *filename);
    void init(
        i32 x, i32 y, f64 angle, SDL_Point *center, SDL_Rect *cliprect,
        SDL_RendererFlip fliptype);
    void renderXYClip(i32 x, i32 y, SDL_Rect *cliprect);
};

#endif
