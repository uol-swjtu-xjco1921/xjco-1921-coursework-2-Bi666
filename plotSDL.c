#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "map.h"

node_t* get_node_by_id(map_t* map, int id)
{
    node_t* node = map->nodes;
    while (node != NULL)
    {
        if (node->id == id)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;  // 如果找不到节点，返回空指针
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
    double xRatio = 1080.0 / lonRange;
    double yRatio = 960.0 / latRange;
    double xOffset =5.0;
    double yOffset =5.0;
    int NODE_SIZE = 5;
    // 绘制地图

    node_t *node = map->nodes;
    while (node != NULL)
    {
        SDL_Rect node_rect = {
            (int)((node->lon - bound->minLon) * xRatio),
            (int)((node->lat - bound->minLat) * yRatio),
            NODE_SIZE, NODE_SIZE
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &node_rect);

        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            node_t *node1 = get_node_by_id(map, edge->node1);
            node_t *node2 = get_node_by_id(map, edge->node2);

            // 计算绘制的起始和终止坐标
            int x1 = (int)((node1->lon - bound->minLon) * xRatio);
            int y1 = (int)((node1->lat - bound->minLat) * yRatio);
            int x2 = (int)((node2->lon - bound->minLon) * xRatio);
            int y2 = (int)((node2->lat - bound->minLat) * yRatio);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            edge = edge->next;
        }
        node = node->next;
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
