#include "iso.h"

Isometric::Isometric(i32 tilesize)
{
    if (tilesize <= 0)
        this->TILESIZE = 32;
    else
        this->TILESIZE = tilesize;

    this->map_height = 0;
    this->map_width  = 0;
    this->scroll_x   = 0;
    this->scroll_y   = 0;
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

