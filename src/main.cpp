#include <SDL.h>
#include <SDL_hints.h>
#include <SDL_render.h>

#include "defs.h"
#include "engine/engine.h"
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
    i32       lastTileClicked;
};

struct Game game;
Texture     tiles_tex;
SDL_Rect    tiles_rects[NUM_ISOMETRIC_TILES];

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

int main(i32 argc, char **argv)
{

    static Renderer renderer = Renderer();
    renderer.init();
    static Input     input = Input();
    static Isometric iso =
        Isometric(32, (u16 **)world_test, sizeof(world_test));

    global.renderer = &renderer;
    global.input    = &input;
    global.iso      = &iso;

    iso.setMapSize(16, 16);

    global.iso->setX(77);
    global.iso->setY(-77);
    global.map_pos_x        = -77;
    global.map_pos_y        = 77;
    global.map_scroll_speed = 5;

    while (global.runnig)
    {
        global.input->update();
        global.renderer->draw();
    }

    return 0;
}
