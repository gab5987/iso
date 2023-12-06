#include <SDL.h>
#include <SDL_hints.h>
#include <SDL_render.h>

#include "defs.h"
#include "engine/engine.h"
#include "global.h"
#include "log.h"

#define NUM_ISOMETRIC_TILES 5
#define MAP_HEIGHT          32
#define MAP_WIDTH           32

// u16 world_test[MAP_HEIGHT][MAP_WIDTH] = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
//     {2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
//     {2, 1, 1, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
//     {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1},
//     {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
//     {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 4, 2, 2, 1},
//     {2, 2, 2, 4, 4, 4, 2, 1, 2, 3, 3, 3, 4, 2, 2, 1},
//     {1, 1, 2, 2, 2, 2, 2, 3, 4, 3, 3, 3, 4, 2, 2, 2},
//     {1, 1, 1, 1, 2, 1, 1, 2, 1, 3, 3, 3, 2, 2, 2, 3},
//     {2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
//     {2, 1, 1, 2, 2, 1, 1, 2, 1, 1, 3, 2, 2, 2, 4, 4},
//     {2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 3, 2, 2, 2, 2, 4},
//     {2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 3, 3, 3, 3, 3, 4},
//     {2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 4},
//     {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
// };

i16 world_test[MAP_HEIGHT][MAP_WIDTH] = {
    {-1, -1, -1, -1, -1, 1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, -2, 2, 2, 1},
    {2, 1, 1, 1, 2, 2, 2, -2, 1, 1, 2, -2, 2, 2, 2, 1},
    {2, 1, 1, 2, 2, 1, 1, -2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1},
    {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 4, 4, 4, 1, 2, 1, 1, 2, 2, 4, 2, 2, 1},
    {2, 2, 2, 4, 4, 4, 2, 1, 2, 3, 3, 3, 4, 2, 2, 1},
    {1, 1, 2, 2, 2, 2, 2, -3, 4, 3, 3, 3, 4, 2, 2, 2},
    {1, 1, 1, 1, 2, 1, 1, 2, 1, 3, 3, 3, 2, 2, 2, 3},
    {2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {2, 1, 1, 2, 2, 1, 1, 2, 1, 1, 3, 2, 2, 2, 4, 4},
    {2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 3, 2, 2, 2, 2, 4},
    {2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 3, 3, 3, 3, 3, 4},
    {2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 4},
    {-1, -1, -1, -1, -1, 1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1},
};

// u16 world_test[MAP_HEIGHT][MAP_WIDTH] = {
//     {10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
// };

Global &global = Global::getInstance();

int main(i32 argc, char **argv)
{
    static Renderer renderer = Renderer();
    renderer.init();
    static Input         input     = Input();
    map_descriptor_t     desc      = {.x = 32, .y = 32};
    texture_descriptor_t text_desc = {
        .image_w   = 512,
        .image_h   = 448,
        .cols      = 8,
        .rows      = 7,
        .tile_size = 64,
    };
    static Isometric iso =
        Isometric("data/blocks.png", world_test, &text_desc, &desc);

    global.renderer = &renderer;
    global.input    = &input;
    global.iso      = &iso;
    global.speed    = 5;

    global.iso->center();

    while (global.runnig)
    {
        global.input->update();
        global.renderer->draw();
    }

    return 0;
}
