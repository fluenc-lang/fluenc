#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tmx/src/tmx.h"

SDL_Renderer *s_renderer;

void* SDL_tex_loader(const char *path)
{
	return IMG_LoadTexture(s_renderer, path);
}

void tmxInit(SDL_Renderer *renderer)
{
	s_renderer = renderer;

	tmx_img_load_func = SDL_tex_loader;
	tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
}

int mapWidth(tmx_map *map)
{
	return map->width;
}

int mapHeight(tmx_map *map)
{
	return map->height;
}

bool hasNextLayer(tmx_layer *layer)
{
	return layer->next;
}

tmx_layer *getNextLayer(tmx_layer *layer)
{
	return layer->next;
}

tmx_layer *beginLayerIteration(tmx_map *map)
{
	if (!map)
	{
		puts("no map :(");
	}

	return map->ly_head;
}

tmx_tile *getTileAt(tmx_map *map, tmx_layer *layer, int x, int y)
{
	if (!map)
	{
		puts("no map :(");
	}

	if (!map)
	{
		puts("no layer :(");
	}

	int gid = (layer->content.gids[(y*map->width)+x]) & TMX_FLIP_BITS_REMOVAL;

	return map->tiles[gid];
}

bool hasTileAt(tmx_map *map, tmx_layer *layer, int x, int y)
{
	return getTileAt(map, layer, x, y);
}

void *getTilesetImage(tmx_tileset *ts)
{
	return ts->image->resource_image;
}
