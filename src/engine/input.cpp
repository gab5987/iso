#include "defs.h"
#include "engine.h"
#include "global.h"

void Input::getMouseTileClick(void)
{
    Global    &global = Global::getInstance();
    point2d_t  point;
    point2d_t  tile_shift;
    point2d_t  mouse2isopoint;
    Isometric *iso = global.iso;

    int correct_x = (global.map_pos_x % 32) * 2;
    int correct_y = global.map_pos_y % 32;

    mouse2isopoint.x = this->mouse.x;
    mouse2isopoint.y = this->mouse.y;

    Isometric::convIsoTo2D(&mouse2isopoint);

    iso->getTileCoordinates(&mouse2isopoint, &point);

    tile_shift.x = correct_x;
    tile_shift.y = correct_y;
    Isometric::conv2dToIso(&tile_shift);

    point.x -=
        ((float)iso->scroll_x + (float)tile_shift.x) / (float)iso->TILESIZE;
    point.y -=
        ((float)iso->scroll_y - (float)tile_shift.y) / (float)iso->TILESIZE;

    std::pair<i32, i32> mapsize = iso->getMapSize();

    if (point.x >= 0 && point.y >= 0 && point.x < mapsize.first &&
        point.y < mapsize.second)
    {
        global.tile_click = iso->getMap()[point.y][point.x];
    }
}

void Input::update(void)
{
    const u8 *keystate = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&this->mouse.x, &this->mouse.y);

    while (SDL_PollEvent(&this->event) != 0)
    {
        switch (this->event.type)
        {
            case SDL_QUIT:
                global.runnig = false;
                break;

            case SDL_KEYUP:
                switch (this->event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        global.runnig = false;
                        break;

                    default:
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (this->event.button.button == SDL_BUTTON_LEFT)
                {
                    this->getMouseTileClick();
                }
                break;

            default:
                break;
        }
    }

    if (keystate[SDL_SCANCODE_W])
    {
        global.iso->scroll_x += global.map_scroll_speed;
        global.iso->scroll_y += global.map_scroll_speed;
        global.map_pos_y += global.map_scroll_speed;

        if (global.map_pos_y > 0)
        {
            global.map_pos_y = 0;
            global.iso->scroll_x += global.map_scroll_speed;
            global.iso->scroll_y += global.map_scroll_speed;
        }
    }
    if (keystate[SDL_SCANCODE_A])
    {
        global.iso->scroll_x += global.map_scroll_speed;
        global.iso->scroll_y -= global.map_scroll_speed;
        global.map_pos_y -= global.map_scroll_speed;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        global.iso->scroll_x -= global.map_scroll_speed;
        global.iso->scroll_y -= global.map_scroll_speed;
        global.map_pos_y -= global.map_scroll_speed;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        global.iso->scroll_x -= global.map_scroll_speed;
        global.iso->scroll_y += global.map_scroll_speed;
        global.map_pos_y += global.map_scroll_speed;
    }
}
