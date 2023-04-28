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

void sdl(map_t *map, range *bound)
{
    // 初始化 SDL
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口和渲染器
    SDL_Window *window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 960, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 计算绘图缩放比例和平移量
    double latRange = bound->maxLat - bound->minLat;
    double lonRange = bound->maxLon - bound->minLon;
    double xRatio = 800.0 / lonRange;
    double yRatio = 711.0 / latRange;
    double xOffset = 160.0;
    double yOffset = 120.0;
    int NODE_SIZE = 5;
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
            (int)((node->lon - bound->minLon) * xRatio + xOffset),
            (int)((bound->maxLat - node->lat) * yRatio + yOffset),
            NODE_SIZE, NODE_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &node_rect);

        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            node_t *node1 = get_node_by_id(map, edge->node1);
            node_t *node2 = get_node_by_id(map, edge->node2);

            // 计算绘制的起始和终止坐标
            int x1 = (int)((node1->lon - bound->minLon) * xRatio + xOffset);
            int y1 = (int)((bound->maxLat - node1->lat) * yRatio + yOffset);
            int x2 = (int)((node2->lon - bound->minLon) * xRatio + xOffset);
            int y2 = (int)((bound->maxLat - node2->lat) * yRatio + yOffset);
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            edge = edge->next;
        }
        node = node->next;
    }

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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, xOffset, 960 - yOffset, 1080 - xOffset + 60, 960 - yOffset);
    SDL_RenderDrawLine(renderer, 1080 - xOffset + 60, 960 - yOffset, 1080 - xOffset + 60, yOffset - 50);
    SDL_RenderDrawLine(renderer, xOffset, yOffset - 50, 1080 - xOffset + 60, yOffset - 50);
    SDL_RenderDrawLine(renderer, xOffset, 960 - yOffset, xOffset, yOffset - 50);

    // 绘制X轴标尺和文本
    for (int i = 0; i <= 5; i++)
    {
        int x = (int)(i * 142.2) + xOffset;
        int y = 711 + yOffset;
        SDL_RenderDrawLine(renderer, x, y - 10, x, y);
        char text[16];
        sprintf(text, "%.3f", bound->minLon + i * (lonRange / 6.0));
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){255, 255, 255});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x - 20, y + 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // 绘制Y轴标尺和文本
    for (int i = 1; i <= 12; i++)
    {
        int x = xOffset;
        int y = 800- (int)(i * 66.67) + yOffset;
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        char text[16];
        sprintf(text, "%.3f", bound->minLat + i * (latRange / 13.0));
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){255, 255, 255});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x - 30 - surface->w, y - surface->h / 2, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // 显示地图
    SDL_RenderPresent(renderer);

    // 等待用户关闭窗口
    SDL_Event event;
    while (true)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
    }

    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
