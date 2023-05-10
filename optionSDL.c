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
    return NULL; // 如果找不到节点，返回空指针
}

void initsize(sizeMap_t *size)
{

    // 计算绘图缩放比例和平移量
    size->latRange = 53.812 - 53.800;
    size->lonRange = 1.565 - 1.540;
    size->xRatio = 850.0 / size->lonRange;
    size->yRatio = 840.0 / size->latRange;
    size->xOffset = 150.0;
    size->yOffset = 120.0;
    size->NODE_SIZE = 5;
}

void origin(map_t *map, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 设置渲染器颜色为白色
    SDL_RenderClear(renderer);                            // 清空渲染器，填充白色
    // 绘制地图
    node_t *node = map->nodes;
    while (node != NULL)
    {
        if (node->num_edges == 0)
        {
            node = node->next;
            continue;
        }
        SDL_Rect node_rect = {
            (int)((node->lon + 1.565) * size->xRatio + size->xOffset - 2.5),
            (int)((53.812 - node->lat) * size->yRatio + size->yOffset - 50 - 2.5),
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

                // 计算绘制的起始和终止坐标
                int x1 = (int)((node1->lon + 1.565) * size->xRatio + size->xOffset);
                int y1 = (int)((53.812 - node1->lat) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon + 1.565) * size->xRatio + size->xOffset);
                int y2 = (int)((53.812 - node2->lat) * size->yRatio + size->yOffset - 50);

                SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            edge = edge->next;
        }
        node = node->next;
    }
}

void link(map_t *map, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 设置渲染器颜色为白色
    SDL_RenderClear(renderer);                            // 清空渲染器，填充白色
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

                // 计算绘制的起始和终止坐标
                int x1 = (int)((node1->lon + 1.565) * size->xRatio + size->xOffset);
                int y1 = (int)((53.812 - node1->lat) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon + 1.565) * size->xRatio + size->xOffset);
                int y2 = (int)((53.812 - node2->lat) * size->yRatio + size->yOffset - 50);

                SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            edge = edge->next;
        }
        node = node->next;
    }
}

void plotpath(map_t *map, path_t *path, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer)
{

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
            (int)((node->lon + 1.565) * size->xRatio + size->xOffset - 2.5),
            (int)((53.812 - node->lat) * size->yRatio + size->yOffset - 50 - 2.5),
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
        if (judge == -1)
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

                // 计算绘制的起始和终止坐标
                int x1 = (int)((node1->lon + 1.565) * size->xRatio + size->xOffset);
                int y1 = (int)((53.812 - node1->lat) * size->yRatio + size->yOffset - 50);
                int x2 = (int)((node2->lon + 1.565) * size->xRatio + size->xOffset);
                int y2 = (int)((53.812 - node2->lat) * size->yRatio + size->yOffset - 50);

                int judge = 2;
                for (int i = 0; i < path->count; i++)
                {
                    if (path->pathCount[i] == node1->count || path->pathCount[i] == node2->count)
                        judge--;
                    if (judge == 0)
                        break;
                }
                if (judge != 0)
                {
                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255);
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
            }
            edge = edge->next;
        }
        node = node->next;
    }
}

void axis(map_t *map, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer, int option)
{
    // 初始化TTF库
    if (TTF_Init() < 0)
    {
        printf("Error initializing TTF library: %s\n", TTF_GetError());
        return;
    }

    // 加载字体
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf", 24); // 24为字体大小
    if (font == NULL)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }

    // 绘制X和Y轴
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, size->xOffset, 960 - size->yOffset, 1080 - size->xOffset + 60, 960 - size->yOffset);
    SDL_RenderDrawLine(renderer, 1080 - size->xOffset + 60, 960 - size->yOffset, 1080 - size->xOffset + 60, size->yOffset - 50);
    SDL_RenderDrawLine(renderer, size->xOffset, size->yOffset - 50, 1080 - size->xOffset + 60, size->yOffset - 50);
    SDL_RenderDrawLine(renderer, size->xOffset, 960 - size->yOffset, size->xOffset, size->yOffset - 50);

    // 绘制X轴标尺和文本
    for (int i = 0; i <= 5; i++)
    {
        int x = (int)(i * 168) + size->xOffset;
        int y = 711 + size->yOffset;
        SDL_RenderDrawLine(renderer, x, y, x, y + 10);
        SDL_RenderDrawLine(renderer, x, y - 761, x, y - 761 + 10);
        char text[16];
        sprintf(text, "%.3f", (-1.565) + i * (size->lonRange / 5.0));
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){0, 0, 0});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x - 20, y + 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // 绘制Y轴标尺和文本
    for (int i = 1; i <= 12; i++)
    {
        int x = size->xOffset;
        int y = 790 - (int)(i * 70) + size->yOffset;
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x + 830, y, x + 830 + 10, y);
        char text[16];
        sprintf(text, "%.3f", 53.80 + i * (size->latRange / 12.0));
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
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nM: display menu\n\nShow the map:\n1: original map\n2: node\n3: link\n4: veg\n5: arch\n6: land\n7: way\n8: geom\n\nFind the way:\nS: shortest path", (SDL_Color){0, 0, 0}, 1000);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {1050, 95, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
    else if (option == 2)
    {

        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nM: display menu\nQ: exit mode\n\nSelect the input mode:\nEnter the node id: press(1)\nClick on node in map(2)\n", (SDL_Color){0, 0, 0}, 1000);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {1050, 95, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    // 显示地图
    SDL_RenderPresent(renderer);
}