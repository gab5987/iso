#include "engine.h"
#include "log.h"

#define NUM_ISOMETRIC_TILES 5

void setupRect(SDL_Rect *rect, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

Isometric::Isometric(i32 tilesize, u16 **mapptr, size_t mapsize)
{
    if (tilesize <= 0)
        this->TILESIZE = 32;
    else
        this->TILESIZE = tilesize;

    memcpy(this->map, mapptr, mapsize);

    this->map_height = 0;
    this->map_width  = 0;
    this->scroll_x   = 0;
    this->scroll_y   = 0;

    i32 x = 0;
    i32 y = 0;

    this->texture.init(0, 0, 0, nullptr, nullptr, SDL_FLIP_NONE);
    for (i32 i = 0; i < NUM_ISOMETRIC_TILES; ++i)
    {
        setupRect(&this->tileset[i], x, y, 64, 80);
        x += 64;
    }

    if (this->texture.load((char *)"data/isotiles.png") == 0)
    {
        Logger::error("Error, could not load texture: data/isotiles.png\n");
        exit(1);
    }
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
    i32       tile = 4;
    point2d_t point;

    for (i32 i = 0; i < this->map_height; ++i)
    {
        for (i32 j = 0; j < this->map_width; ++j)
        {
            point.x = (j * this->TILESIZE) + this->scroll_x;
            point.y = (i * this->TILESIZE) + this->scroll_y;

            tile = this->map[i][j];
            Isometric::conv2dToIso(&point);

            this->texture.renderXYClip(point.x, point.y, &this->tileset[tile]);
        }
    }
}
