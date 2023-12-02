#include <SDL.h>
#include <SDL_hints.h>
#include <SDL_render.h>

#include "defs.h"
#include "engine/iso.h"
#include "engine/renderer.h"
#include "engine/texture.h"
#include "global.h"
#include "log.h"

#define NUM_ISOMETRIC_TILES 5
#define MAP_HEIGHT          16
#define MAP_WIDTH           16

int world_test[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1},
    {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 4, 2, 2, 1},
    {2, 2, 2, 4, 4, 4, 2, 1, 2, 3, 3, 3, 4, 2, 2, 1},
    {1, 1, 2, 2, 2, 2, 2, 3, 4, 3, 3, 3, 4, 2, 2, 2},
    {1, 1, 1, 1, 2, 1, 1, 2, 1, 3, 3, 3, 2, 2, 2, 3},
    {2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 2, 2, 1, 1, 2, 1, 1, 3, 2, 2, 2, 4, 4},
    {2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 3, 2, 2, 2, 2, 4},
    {2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 3, 3, 3, 3, 3, 4},
    {2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 4},
    {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
};

Global &global = Global::getInstance();

struct Game
{
    SDL_Event event;
    i32       loopDone;
    SDL_Rect  mouseRect;
    point2d_t mousePoint;
    point2d_t mapScroll2Dpos;
    i32       mapScrolllSpeed;
    Isometric isoEngine = Isometric(32);
    i32       lastTileClicked;
};

struct Game game;
Texture     tiles_tex;
SDL_Rect    tiles_rects[NUM_ISOMETRIC_TILES];

void setupRect(SDL_Rect *rect, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void initTileClip(void)
{
    i32 x = 0, y = 0;

    tiles_tex.init(0, 0, 0, nullptr, nullptr, SDL_FLIP_NONE);
    for (i32 i = 0; i < NUM_ISOMETRIC_TILES; ++i)
    {
        setupRect(&tiles_rects[i], x, y, 64, 80);
        x += 64;
    }
}

void init(void)
{
    game.loopDone = 0;
    initTileClip();
    game.isoEngine.setMapSize(16, 16);

    game.isoEngine.setX(77);
    game.isoEngine.setY(-77);
    game.mapScroll2Dpos.x = -77;
    game.mapScroll2Dpos.y = 0;
    game.mapScrolllSpeed  = 3;
    game.lastTileClicked  = -1;

    if (tiles_tex.load((char *)"data/isotiles.png") == 0)
    {
        Logger::error("Error, could not load texture: data/isotiles.png\n");
        exit(1);
    }
}

void drawIsoMouse(void)
{
    int correctX = (game.mapScroll2Dpos.x % 32) * 2;
    int correctY = game.mapScroll2Dpos.y % 32;

    game.mousePoint.x =
        (game.mouseRect.x / game.isoEngine.TILESIZE) * game.isoEngine.TILESIZE;
    game.mousePoint.y =
        (game.mouseRect.y / game.isoEngine.TILESIZE) * game.isoEngine.TILESIZE;

    // For every other x position on the map
    if ((game.mousePoint.x / game.isoEngine.TILESIZE) % 2)
    {
        // Move the mouse down by half a tile so we can
        // pick isometric tiles on that row as well.
        game.mousePoint.y += game.isoEngine.TILESIZE * 0.5;
    }
    tiles_tex.renderXYClip(
        game.mousePoint.x - correctX, game.mousePoint.y + correctY,
        &tiles_rects[0]);
}

void drawIsoMap(Isometric *isoEngine)
{
    i32       tile = 4;
    point2d_t point;

    for (u32 i = 0; i < isoEngine->map_height; ++i)
    {
        for (u32 j = 0; j < isoEngine->map_width; ++j)
        {
            point.x = (j * game.isoEngine.TILESIZE) + isoEngine->scroll_x;
            point.y = (i * game.isoEngine.TILESIZE) + isoEngine->scroll_y;

            tile = world_test[i][j];
            Isometric::conv2dToIso(&point);

            tiles_tex.renderXYClip(point.x, point.y, &tiles_rects[tile]);
        }
    }
}

void getMouseTileClick(Isometric *isoEngine)
{
    point2d_t point;
    point2d_t tileShift, mouse2IsoPOint;

    int correctX = (game.mapScroll2Dpos.x % 32) * 2;
    int correctY = game.mapScroll2Dpos.y % 32;

    // copy mouse point
    mouse2IsoPOint = game.mousePoint;
    Isometric::convIsoTo2D(&mouse2IsoPOint);

    // get tile coordinates
    isoEngine->getTileCoordinates(&mouse2IsoPOint, &point);

    tileShift.x = correctX;
    tileShift.y = correctY;
    Isometric::conv2dToIso(&tileShift);

    point.x -= ((float)isoEngine->scroll_x + (float)tileShift.x) /
               (float)isoEngine->TILESIZE;
    point.y -= ((float)isoEngine->scroll_y - (float)tileShift.y) /
               (float)isoEngine->TILESIZE;

    if (point.x >= 0 && point.y >= 0 && point.x < MAP_WIDTH &&
        point.y < MAP_HEIGHT)
    {
        game.lastTileClicked = world_test[point.y][point.x];
    }
}

void draw(void)
{
    Renderer *renderer = global.renderer;
    SDL_SetRenderDrawColor(renderer->getRenderer(), 0x3b, 0x3b, 0x3b, 0x00);
    SDL_RenderClear(renderer->getRenderer());

    drawIsoMap(&game.isoEngine);
    drawIsoMouse();

    if (game.lastTileClicked != -1)
    {
        tiles_tex.renderXYClip(0, 0, &tiles_rects[game.lastTileClicked]);
    }

    SDL_RenderPresent(renderer->getRenderer());
    SDL_Delay(10);
}

void update(void)
{
    SDL_GetMouseState(&game.mouseRect.x, &game.mouseRect.y);
}

void updateInput(void)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&game.event) != 0)
    {
        switch (game.event.type)
        {
            case SDL_QUIT:
                game.loopDone = 1;
                break;

            case SDL_KEYUP:
                switch (game.event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        game.loopDone = 1;
                        break;

                    default:
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (game.event.button.button == SDL_BUTTON_LEFT)
                {
                    getMouseTileClick(&game.isoEngine);
                }
                break;

            default:
                break;
        }
    }

    if (keystate[SDL_SCANCODE_W])
    {
        game.isoEngine.scroll_x += game.mapScrolllSpeed;
        game.isoEngine.scroll_y += game.mapScrolllSpeed;
        game.mapScroll2Dpos.y += game.mapScrolllSpeed;

        if (game.mapScroll2Dpos.y > 0)
        {
            game.mapScroll2Dpos.y = 0;
            game.isoEngine.scroll_x -= game.mapScrolllSpeed;
            game.isoEngine.scroll_y -= game.mapScrolllSpeed;
        }
    }
    if (keystate[SDL_SCANCODE_A])
    {
        game.isoEngine.scroll_x += game.mapScrolllSpeed;
        game.isoEngine.scroll_y -= game.mapScrolllSpeed;
        game.mapScroll2Dpos.x -= game.mapScrolllSpeed;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        game.isoEngine.scroll_x -= game.mapScrolllSpeed;
        game.isoEngine.scroll_y -= game.mapScrolllSpeed;
        game.mapScroll2Dpos.y -= game.mapScrolllSpeed;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        game.isoEngine.scroll_x -= game.mapScrolllSpeed;
        game.isoEngine.scroll_y += game.mapScrolllSpeed;
        game.mapScroll2Dpos.x += game.mapScrolllSpeed;
    }
}

int main(i32 argc, char **argv)
{

    Renderer renderer = Renderer();
    global.renderer   = &renderer;

    renderer.init();

    init();

    while (!game.loopDone)
    {
        update();
        updateInput();
        draw();
    }

    return 0;

    // SDL_GL_SetAttribute(
    //     SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //
    // Global &global = Global::getInstance();
    //
    // static Renderer renderer = Renderer();
    // renderer.init();
    //
    // global.renderer = &renderer;
    //
    // while (global.runnig)
    // {
    //     SDL_Event evt;
    //
    //     while (SDL_PollEvent(&evt))
    //     {
    //         if (evt.type == SDL_QUIT) global.runnig = false;
    //     }
    // }
    //
    // return 0;
}
