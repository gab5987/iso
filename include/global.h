#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <SDL.h>

#include "defs.h"
#include "log.h"

class Renderer;

class Global
{
    private:
    explicit Global(void){};

    public:
    i32                    wx    = 1200;
    i32                    wy    = 720;
    static constexpr char *WNAME = (char *)"Game";

    Renderer *render = nullptr;

    bool runnig = false;

    Renderer *renderer = nullptr;

    static Global &getInstance()
    {
        static Global inst;
        return inst;
    };
};

#endif
