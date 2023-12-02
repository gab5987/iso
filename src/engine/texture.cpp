#include <SDL.h>
#include <SDL_image.h>

#include "global.h"
#include "log.h"
#include "renderer.h"
#include "texture.h"

i32 Texture::load(char *filename)
{
    Renderer    *renderer    = global.renderer;
    SDL_Surface *tmp_surface = IMG_Load(filename);

    if (tmp_surface == nullptr)
    {
        Logger::error(
            "Texture error: Could not load image:%s! SDL_image Error:%s\n",
            filename, IMG_GetError());
        return 0;
    }
    else
    {
        this->texture =
            SDL_CreateTextureFromSurface(renderer->getRenderer(), tmp_surface);

        if (this->texture == nullptr)
        {
            Logger::error(
                "Texture error: Could not load image:%s! SDL_image Error:%s\n",
                filename, IMG_GetError());
            SDL_FreeSurface(tmp_surface);
            return 0;
        }
        else
        {
            this->width  = tmp_surface->w;
            this->height = tmp_surface->h;
        }
        SDL_FreeSurface(tmp_surface);
        return 1;
    }
    return 0;
}

void Texture::init(
    i32 x, i32 y, f64 angle, SDL_Point *center, SDL_Rect *cliprect,
    SDL_RendererFlip fliptype)
{
    this->x        = x;
    this->y        = y;
    this->angle    = angle;
    this->center   = center;
    this->fliptype = fliptype;
    this->cliprect = cliprect;
}

void Texture::renderXYClip(i32 x, i32 y, SDL_Rect *cliprect)
{
    Renderer *renderer = global.renderer;
    this->x            = x;
    this->y            = y;
    this->cliprect     = cliprect;
    SDL_Rect quad      = {this->x, this->y, this->width, this->height};
    if (this->cliprect != nullptr)
    {
        quad.w = this->cliprect->w;
        quad.h = this->cliprect->h;
    }

    SDL_RenderCopyEx(
        renderer->getRenderer(), this->texture, this->cliprect, &quad,
        this->angle, this->center, this->fliptype);
}
