#include <stdio.h>
#include <stdlib.h>
#include "map.h"

node_t *get_node_by_id(map_t *map, int id);
void initsize(sizeMap_t *size, range_t *bound);
void origin(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer);
void link(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer);
void geom(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer);
void extra(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer, int option);
void plotpath(map_t *map, path_t *path, path_t *path1, sizeMap_t *s, range_t *bounde, SDL_Window *window, SDL_Renderer *renderer, int option);
int axis(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer, int option);
