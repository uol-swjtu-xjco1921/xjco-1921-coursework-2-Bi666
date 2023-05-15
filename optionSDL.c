#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"

node_t *get_node_by_id(map_t *map, int id)
{
    node_t *node = map->nodes;
    while (node != NULL)
    {
        if (node->id == id)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
    // If the node is not found, return the null pointer
}

void initsize(sizeMap_t *size, range_t *bound)
{

    // Calculate drawing scaling and panning
    size->latRange = bound->maxLat - bound->minLat;
    size->lonRange = bound->maxLon - bound->minLon;
    size->xRatio = 850.0 / size->lonRange;
    size->yRatio = 860.0 / size->latRange;
    size->xOffset = 150.0;
    size->yOffset = 120.0;
    size->NODE_SIZE = 5;
}

void origin(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer)
{
    // Set the renderer color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Empty the renderer and fill it with white
    SDL_RenderClear(renderer);
    // Drawing Maps
    node_t *node = map->nodes;
    while (node != NULL)
    {
        if (node->num_edges == 0)
        {
            node = node->next;
            continue;
        }
        SDL_Rect node_rect = {
            (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset - 2.5),
            (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 - 2.5),
            size->NODE_SIZE, size->NODE_SIZE};
        SDL_SetRenderDrawColor(renderer, 65, 105, 225, 255);
        SDL_RenderFillRect(renderer, &node_rect);

        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            if (edge->node1 == node->id)
            {
                node_t *node1 = get_node_by_id(map, edge->node1);
                node_t *node2 = get_node_by_id(map, edge->node2);

                // Calculate the start and end coordinates of the drawing
                int x1 = (int)((node1->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y1 = (int)((bound->maxLat - node1->lat - 0.001) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y2 = (int)((bound->maxLat - node2->lat - 0.001) * size->yRatio + size->yOffset - 50);

                SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            edge = edge->next;
        }
        node = node->next;
    }
}

void link(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer)
{
    // Set the renderer color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Empty the renderer and fill it with white
    SDL_RenderClear(renderer);
    node_t *node = map->nodes;
    while (node != NULL)
    {
        if (node->num_edges == 0)
        {
            node = node->next;
            continue;
        }
        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            if (edge->node1 == node->id)
            {
                node_t *node1 = get_node_by_id(map, edge->node1);
                node_t *node2 = get_node_by_id(map, edge->node2);

                // Calculate the start and end coordinates of the drawing
                int x1 = (int)((node1->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y1 = (int)((bound->maxLat - node1->lat - 0.001) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y2 = (int)((bound->maxLat - node2->lat - 0.001) * size->yRatio + size->yOffset - 50);

                SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            edge = edge->next;
        }
        node = node->next;
    }
}

void geom(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer)
{
    // Set the renderer color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Empty the renderer and fill it with white
    SDL_RenderClear(renderer);
    int color = 0;
    geom_t *geom = map->geoms;
    while (geom != NULL)
    {
        for (int i = 0; i < geom->node_count - 1; i++)
        {
            node_t *node1 = map->nodes;
            while (node1 != NULL)
            {
                if (node1->id == geom->node[i])
                    break;
                node1 = node1->next;
            }

            node_t *node2 = map->nodes;
            while (node2 != NULL)
            {
                if (node2->id == geom->node[i + 1])
                    break;
                node2 = node2->next;
            }
            if (node1 != NULL && node2 != NULL)
            {
                int x1 = (int)((node1->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y1 = (int)((bound->maxLat - node1->lat - 0.001) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y2 = (int)((bound->maxLat - node2->lat - 0.001) * size->yRatio + size->yOffset - 50);
                SDL_SetRenderDrawColor(renderer, 255, 69, 0, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }

        geom = geom->next;
    }
}

void extra(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer, int option)
{
    // Clear window
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    node_t *node = map->nodes;
    while (node != NULL)
    {
        if (node->num_edges == 0)
        {
            node = node->next;
            continue;
        }

        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            if (edge->node1 == node->id)
            {
                node_t *node1 = get_node_by_id(map, edge->node1);
                node_t *node2 = get_node_by_id(map, edge->node2);

                // Calculate the start and end coordinates of the drawing
                int x1 = (int)((node1->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y1 = (int)((bound->maxLat - node1->lat - 0.001) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y2 = (int)((bound->maxLat - node2->lat - 0.001) * size->yRatio + size->yOffset - 50);

                if (option == 1)
                {
                    if (edge->veg <= edge->length && edge->veg > 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 250, 154, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else if (edge->veg > edge->length)
                    {
                        SDL_SetRenderDrawColor(renderer, 46, 139, 87, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
                else if (option == 2)
                {
                    if (edge->arch <= edge->length && edge->arch > 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else if (edge->arch > edge->length)
                    {
                        SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
                else if (option == 3)
                {
                    if (edge->land <= edge->length && edge->land > 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else if (edge->land > edge->length)
                    {
                        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
            }
            edge = edge->next;
        }
        node = node->next;
    }
}

void plotpath(map_t *map, path_t *path, path_t *path1, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer, int option, int pr)
{
    double answer = 0.0;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    node_t *node = map->nodes;
    while (node != NULL)
    {
        if (node->num_edges == 0)
        {
            node = node->next;
            continue;
        }
        SDL_Rect node_rect = {
            (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset - 2.5),
            (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 - 2.5),
            size->NODE_SIZE, size->NODE_SIZE};
        int judge = -1;
        for (int i = 0; i < path->count; i++)
        {
            if (path->pathCount[i] == node->count)
            {
                judge = 0;
                break;
            }
        }

        int judge1 = -1;
        for (int i = 0; i < path1->count; i++)
        {
            if (path1->pathCount[i] == node->count)
            {
                judge1 = 0;
                break;
            }
        }

        if (judge == -1 && judge1 == -1)
        {
            SDL_SetRenderDrawColor(renderer, 65, 105, 225, 255);
            SDL_RenderFillRect(renderer, &node_rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255);
            SDL_RenderFillRect(renderer, &node_rect);
        }

        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            if (edge->node1 == node->id)
            {
                node_t *node1 = get_node_by_id(map, edge->node1);
                node_t *node2 = get_node_by_id(map, edge->node2);

                // Calculate the start and end coordinates of the drawing
                int x1 = (int)((node1->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y1 = (int)((bound->maxLat - node1->lat - 0.001) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset);
                int y2 = (int)((bound->maxLat - node2->lat - 0.001) * size->yRatio + size->yOffset - 50);

                int judge = 2;
                for (int i = 0; i < path->count; i++)
                {
                    if (path->pathCount[i] == node1->count || path->pathCount[i] == node2->count)
                        judge--;
                    if (judge == 0)
                        break;
                }
                int judge1 = 2;
                for (int i = 0; i < path1->count; i++)
                {
                    if (path1->pathCount[i] == node1->count || path1->pathCount[i] == node2->count)
                        judge1--;
                    if (judge1 == 0)
                        break;
                }
                if (judge != 0 && judge1 != 0)
                {
                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255);
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    if (option == 1)
                        answer += edge->length;
                    else
                        answer += edge->length / edge->speed;
                }
            }
            edge = edge->next;
        }
        node = node->next;
    }
    if (pr == EXIT_NO_PATH_FOUND)
        return;
    if (option == 1)
        printf("Total length of the shortest route is %.2f\n", answer);
    else
        printf("Total time of the quickest route is %.2f\n", answer);
}

int axis(map_t *map, sizeMap_t *size, range_t *bound, SDL_Window *window, SDL_Renderer *renderer, int option)
{
    // Initialize TTF library
    if (TTF_Init() < 0)
    {
        printf("Error initializing TTF library: %s\n", TTF_GetError());
        return EXIT_SDL_FAILED;
    }

    // Loading Fonts
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf", 24);
    if (font == NULL)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        return EXIT_SDL_FAILED;
    }

    // Plotting the X and Y axes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, size->xOffset, 960 - size->yOffset, 1080 - size->xOffset + 60, 960 - size->yOffset);
    SDL_RenderDrawLine(renderer, 1080 - size->xOffset + 60, 960 - size->yOffset, 1080 - size->xOffset + 60, size->yOffset - 50);
    SDL_RenderDrawLine(renderer, size->xOffset, size->yOffset - 50, 1080 - size->xOffset + 60, size->yOffset - 50);
    SDL_RenderDrawLine(renderer, size->xOffset, 960 - size->yOffset, size->xOffset, size->yOffset - 50);

    // Draw X-axis scale and text
    for (int i = 0; i <= 5; i++)
    {
        int x = (int)(i * 168) + size->xOffset;
        int y = 711 + size->yOffset;
        SDL_RenderDrawLine(renderer, x, y, x, y + 10);
        SDL_RenderDrawLine(renderer, x, y - 761, x, y - 761 + 10);
        char text[16];
        sprintf(text, "%.3f", (bound->minLon - 0.001) + i * (size->lonRange / 5.0));
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){0, 0, 0});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x - 20, y + 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // Draw Y-axis scale and text
    for (int i = 1; i <= 12; i++)
    {
        int x = size->xOffset;
        int y = 790 - (int)(i * 70) + size->yOffset;
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x + 830, y, x + 830 + 10, y);
        char text[16];
        sprintf(text, "%.3f", bound->minLat - 0.001 + i * (size->latRange / 12.0));
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){0, 0, 0});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x - 30 - surface->w, y - surface->h / 2, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, "'map.out'", (SDL_Color){0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {710, 80, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderDrawLine(renderer, 850, 96, 910, 96);
    SDL_Rect rect1 = {880, 94, size->NODE_SIZE, size->NODE_SIZE};
    SDL_RenderFillRect(renderer, &rect1);

    if (option == 1)
    {
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nQ: exit mode\n\nShow the map:\n1: original map\n2: node\n3: link\n4: veg\n5: arch\n6: land\n7: geom\n\nFind the way:\nS: shortest route\nT: quickest route\nP: pass given location\nE: edit map attributes\n", (SDL_Color){0, 0, 0}, 1000);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {1050, 95, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
    else if (option == 2)
    {

        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nM: exit mode\nQ: exit mode\n\nSelect the input mode:\n1: Enter the node id\n2: Click on node in map\n", (SDL_Color){0, 0, 0}, 1000);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {1050, 95, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    // Display Map
    SDL_RenderPresent(renderer);
    return EXIT_NO_ERRORS;
}
