#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "global.h"

class Renderer
{
    private:
    inline static Global &global = Global::getInstance();

    void initquad(void);

    protected:
    SDL_Window   *window = nullptr;
    SDL_Renderer *renderer;

    public:
    void init(void);

    inline SDL_Renderer *getRenderer(void)
    {
        return this->renderer;
    };

    inline void closeRender(void) const
    {
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
    };
};

#endif
