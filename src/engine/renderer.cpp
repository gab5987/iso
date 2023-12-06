#include <SDL.h>
#include <SDL_image.h>

#include "engine.h"
#include "log.h"

void Renderer::init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Logger::error("Could not init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
    {
        Logger::warning("SDL Linear texture filtering not enabled");
    }

    this->window = SDL_CreateWindow(
        Global::WNAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        global.wx, global.wy,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (this->window == nullptr)
    {
        Logger::error("Could not initialize: window was a nullptr after "
                      "its creation");
        exit(1);
    }

    this->renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE |
            SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr)
    {
        Logger::error("SDL Renderer creation failed:%s", SDL_GetError());
        exit(1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        Logger::error("SDL Img module not loaded!");
        exit(1);
    }

    global.runnig = true;
    global.wind   = this->window;
    global.rend   = this->renderer;

    Logger::info("OpenGL Loaded");
    // Logger::info("Vendor:   %s", glGetString(GL_VENDOR));
    // Logger::info("Renderer: %s", glGetString(GL_RENDER));
    // Logger::info("Version:  %s", glGetString(GL_VERSION))
}

void Renderer::draw(void)
{
    Isometric *iso = global.iso;
    SDL_SetRenderDrawColor(this->renderer, 0x3b, 0x3b, 0x3b, 0x00);
    SDL_RenderClear(this->renderer);

    iso->draw();

    // drawIsoMouse();

    // if (game.lastTileClicked != -1)
    // {
    //     tiles_tex.renderXYClip(0, 0, &tiles_rects[game.lastTileClicked]);
    // }

    SDL_RenderPresent(this->renderer);
    SDL_Delay(10);
}

