#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <utility>

#include "defs.h"
#include "global.h"

typedef struct
{
    i32 x;
    i32 y;
} point2d_t;

class Input
{
    private:
    Global   &global = Global::getInstance();
    SDL_Event event;
    SDL_Rect  mouse;
    void      getMouseTileClick();

    public:
    void update(void);
};

class Renderer
{
    private:
    inline static Global &global = Global::getInstance();

    void initquad(void);

    protected:
    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;

    public:
    void init(void);
    void draw(void);

    inline void closeRender(void) const
    {
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
    };
};

class Texture
{
    private:
    inline static Global &global = Global::getInstance();
    i32                   x;
    i32                   y;
    i32                   width;
    i32                   height;
    f64                   angle;

    SDL_Point       *center;
    SDL_Rect        *cliprect;
    SDL_RendererFlip fliptype;
    SDL_Texture     *texture;

    public:
    i32  load(char *filename);
    void init(
        i32 x, i32 y, f64 angle, SDL_Point *center, SDL_Rect *cliprect,
        SDL_RendererFlip fliptype);
    void renderXYClip(i32 x, i32 y, SDL_Rect *cliprect);
};

typedef struct
{
    u8 x, y;
} map_descriptor_t;

typedef struct
{
    u16 image_w, image_h;
    u8  cols, rows;
    u8  tile_size;
} texture_descriptor_t;

class Isometric
{
    private:
    static constexpr size_t MAX_MAP_SIZE = 32;
    static constexpr size_t MAX_TILESET  = 64;
    i16                     map[MAX_MAP_SIZE][MAX_MAP_SIZE];
    SDL_Rect                tileset[MAX_TILESET];
    Texture                *texture = new Texture();

    public:
    u32 TILESIZE;
    i32 scroll_x;
    i32 scroll_y;
    i32 map_height;
    i32 map_width;

    inline void setX(i32 x)
    {
        this->scroll_x = x;
    };
    inline void setY(i32 y)
    {
        this->scroll_y = y;
    };
    inline u16 **getMap(void)
    {
        return (u16 **)this->map;
    };

    inline std::pair<i32, i32> getMapSize(void)
    {
        return std::pair<i32, i32>(this->map_width, this->map_height);
    };

    ~Isometric(void)
    {
        delete this->texture;
    };

    Isometric(
        const char *text_loc, i16 (&mapptr)[MAX_MAP_SIZE][MAX_MAP_SIZE],
        texture_descriptor_t *text_desc, map_descriptor_t *map_desc);
    void        draw(void);
    void        center(void);
    void        setMapSize(i32 width, i32 height);
    static void conv2dToIso(point2d_t *point);
    static void convIsoTo2D(point2d_t *point);
    void getTileCoordinates(point2d_t *point, point2d_t *point2d_coord) const;
};

#endif

