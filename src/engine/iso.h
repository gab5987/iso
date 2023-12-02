#ifndef _ISO_H_
#define _ISO_H_

#include "defs.h"
typedef struct
{
} isoengine_t;

typedef struct
{
    int x;
    int y;
} point2d_t;

class Isometric
{
    public:
    u32 TILESIZE;
    i32 scroll_x;
    i32 scroll_y;
    i32 map_height;
    i32 map_width;

    explicit Isometric(i32 tilesize);
    inline void setX(i32 x)
    {
        this->scroll_x = x;
    };
    inline void setY(i32 y)
    {
        this->scroll_y = y;
    };
    void        setMapSize(i32 width, i32 height);
    static void conv2dToIso(point2d_t *point);
    static void convIsoTo2D(point2d_t *point);
    void getTileCoordinates(point2d_t *point, point2d_t *point2d_coord) const;
};

#endif

