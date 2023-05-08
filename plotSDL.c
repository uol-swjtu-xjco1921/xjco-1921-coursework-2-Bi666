#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "dijkstra.h"
#include "optionSDL.h"

void sdl(map_t *map, path_t *path)
{
    sizeMap_t size;
    initsize(&size);
    // 初始化 SDL
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口和渲染器
    SDL_Window *window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1380, 960, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    origin(map, &size, window, renderer); //绘制原始地图
    axis(map, &size, window, renderer);

    //获取用户键盘输入事件
    int option = -1;
    bool quit = false;
    SDL_Event eventM;
    while (!quit)
    {
        option = -1;
        while (SDL_PollEvent(&eventM))
        {
            if (eventM.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (eventM.type == SDL_KEYDOWN)
            {
                switch (eventM.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_m:
                    option = 0;
                    break;
                case SDLK_1:
                    option = 1;
                    break;
                case SDLK_2:
                    option = 2;
                    break;
                case SDLK_3:
                    option = 3;
                    break;
                case SDLK_4:
                    option = 4;
                    break;
                case SDLK_5:
                    option = 5;
                    break;
                case SDLK_6:
                    option = 6;
                    break;
                case SDLK_7:
                    option = 7;
                    break;
                default:
                    break;
                }
            }
        }
        if (option == -1)
            continue;
        if (option == 1)
        {
            origin(map, &size, window, renderer);
        }
        else if (option == 2)
        {
            link(map, &size, window, renderer);
        }
        else if(option == 3)
        {
            //清空窗口
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

                        // 计算绘制的起始和终止坐标
                        int x1 = (int)((node1->lon + 1.565) * size.xRatio + size.xOffset);
                        int y1 = (int)((53.812 - node1->lat) * size.yRatio + size.yOffset - 50);
                        int x2 = (int)((node2->lon + 1.565) * size.xRatio + size.xOffset);
                        int y2 = (int)((53.812 - node2->lat) * size.yRatio + size.yOffset - 50);
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
                    edge = edge->next;
                }
                node = node->next;
            }
        }
        else if (option == 4)
        {
            //清空窗口
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

                        // 计算绘制的起始和终止坐标
                        int x1 = (int)((node1->lon + 1.565) * size.xRatio + size.xOffset);
                        int y1 = (int)((53.812 - node1->lat) * size.yRatio + size.yOffset - 50);
                        int x2 = (int)((node2->lon + 1.565) * size.xRatio + size.xOffset);
                        int y2 = (int)((53.812 - node2->lat) * size.yRatio + size.yOffset - 50);
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
                    edge = edge->next;
                }
                node = node->next;
            }
        }
        else if (option == 5)
        {
            //清空窗口
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

                        // 计算绘制的起始和终止坐标
                        int x1 = (int)((node1->lon + 1.565) * size.xRatio + size.xOffset);
                        int y1 = (int)((53.812 - node1->lat) * size.yRatio + size.yOffset - 50);
                        int x2 = (int)((node2->lon + 1.565) * size.xRatio + size.xOffset);
                        int y2 = (int)((53.812 - node2->lat) * size.yRatio + size.yOffset - 50);
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
                    edge = edge->next;
                }
                node = node->next;
            }
        }
        axis(map, &size, window, renderer);
    }
    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();  
}

