#include "engine.h"
#include "log.h"

void setupRect(SDL_Rect *rect, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

Isometric::Isometric(
    const char           *text_loc, i16 (&mapptr)[MAX_MAP_SIZE][MAX_MAP_SIZE],
    texture_descriptor_t *text_desc, map_descriptor_t *map_desc)
{
    if (text_loc == nullptr || mapptr == nullptr || text_desc == nullptr ||
        map_desc == nullptr)
    {
        return;
    }
    if (text_desc->tile_size <= 0)
    {
        Logger::error("Loaded a texture with wrong size(< 0) | %s", text_loc);
        return;
    }

    this->TILESIZE = text_desc->tile_size / 2;

    memcpy(this->map, mapptr, sizeof(this->map));

    // for (u16 j = 0; j < MAX_MAP_SIZE; j++)
    // {
    //     for (u16 i = 0; i < MAX_MAP_SIZE; i++) printf("%d ", mapptr[j][i]);
    //     printf("\n");
    // }

    this->map_height = map_desc->y;
    this->map_width  = map_desc->x;
    this->scroll_x   = 0;
    this->scroll_y   = 0;

    this->texture->init(0, 0, 0, nullptr, nullptr, SDL_FLIP_NONE);

    i32 y = 0;
    i32 x = 0;
    for (u8 i = 0; i < text_desc->rows; ++i)
    {
        u8 offset = i * text_desc->cols;
        for (u8 j = 0; j < text_desc->cols; j++)
        {
            setupRect(
                &this->tileset[j + offset], x, y, text_desc->tile_size,
                text_desc->tile_size);
            x += text_desc->tile_size;
        }
        x = 0;
        y += text_desc->tile_size;
    }

    if (this->texture->load((char *)text_loc) == 0)
    {
        Logger::error("Could not load texture: %s", text_loc);
        exit(1);
    }
}

void Isometric::center(void)
{
    static Global &global = Global::getInstance();

    i32 winx;
    i32 winy;
    SDL_GetWindowSize(global.wind, &winx, &winy);

    this->scroll_x = (0.44 * winx) - 160;
    this->scroll_y = (-0.2 * winy) - this->TILESIZE;
}

void Isometric::setMapSize(i32 width, i32 height)
{
    this->map_height = height;
    this->map_width  = width;
}

void Isometric::conv2dToIso(point2d_t *point)
{
    int tmp_x = point->x - point->y;
    int tmp_y = (point->x + point->y) * 0.5;
    point->x  = tmp_x;
    point->y  = tmp_y;
}

void Isometric::convIsoTo2D(point2d_t *point)
{
    int tmp_x = (2 * point->y + point->x) * 0.5;
    int tmp_y = (2 * point->y - point->x) * 0.5;
    point->x  = tmp_x;
    point->y  = tmp_y;
}

void Isometric::getTileCoordinates(
    point2d_t *point, point2d_t *point2d_coord) const
{
    f32 temp_x = (f32)point->x / (f32)this->TILESIZE;
    f32 temp_y = (f32)point->y / (f32)this->TILESIZE;

    point2d_coord->x = (i32)temp_x;
    point2d_coord->y = (i32)temp_y;
}

void Isometric::draw(void)
{
    i32       tile;
    point2d_t point;

    for (i32 i = 0; i < this->map_height; ++i)
    {
        for (i32 j = 0; j < this->map_width; ++j)
        {
            tile = this->map[i][j];
            if (tile < 0) continue;

            point.x = (j * this->TILESIZE) + this->scroll_x;
            point.y = (i * this->TILESIZE) + this->scroll_y;

            // if (!done) printf("(%d) x: %d | y: %d\n", i, point.x, point.y);

            Isometric::conv2dToIso(&point);

            this->texture->renderXYClip(point.x, point.y, &this->tileset[tile]);
        }
    }
}
