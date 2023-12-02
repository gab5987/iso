#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <SDL.h>

#include "defs.h"

class Renderer;
class Isometric;
class Input;

class Global
{
    private:
    explicit Global(void){};

    public:
    i32                    wx     = 1200;
    i32                    wy     = 720;
    bool                   runnig = false;
    static constexpr char *WNAME  = (char *)"Game";

    Renderer  *renderer = nullptr;
    Isometric *iso      = nullptr;
    Input     *input    = nullptr;

    i32 map_pos_x;
    i32 map_pos_y;
    i32 map_scroll_speed;
    i32 speed;
    i32 tile_click = -1;

    SDL_Renderer *rend = nullptr;
    SDL_Window   *wind = nullptr;
    SDL_Rect      mouse;

    static Global &getInstance()
    {
        static Global inst;
        return inst;
    };
};

#endif
