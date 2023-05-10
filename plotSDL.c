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

    // 创建窗口和渲染器
    SDL_Window *window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1500, 960, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    origin(map, &size, window, renderer); // 绘制原始地图
    axis(map, &size, window, renderer, 1);

    // 获取用户键盘输入事件
    bool quit = false;
    while (!quit)
    {
        SDL_Event eventM;
        int option = -1;
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
                    quit = true;
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
                case SDLK_s:
                    option = 11;
                    break;
                default:
                    break;
                }
            }
            else if (eventM.type == SDL_MOUSEBUTTONDOWN)
            {
                // 遍历点数组，查找是否有鼠标点击的点
                SDL_Rect rect = {1050, 700, 1480, 960};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                node_t *node = map->nodes;
                while (node != NULL)
                {
                    if (eventM.button.x >= (int)((node->lon + 1.565) * size.xRatio + size.xOffset - 2.5) &&
                        eventM.button.x <= (int)((node->lon + 1.565) * size.xRatio + size.xOffset + 2.5) &&
                        eventM.button.y >= (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 - 2.5) &&
                        eventM.button.y <= (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 + 2.5))
                    {
                        char text[100];
                        sprintf(text, "node: id: %d\n      lat: %lf\n      lon: %lf", node->id, node->lat, node->lon);
                        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, (SDL_Color){220, 20, 60}, 1000);
                        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                        SDL_Rect textRect = {1050, 700, textSurface->w, textSurface->h};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                        SDL_RenderPresent(renderer);
                        break;
                    }
                    node = node->next;
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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            node_t *node = map->nodes;
            while (node != NULL)
            {
                SDL_Rect node_rect = {
                    (int)((node->lon + 1.565) * size.xRatio + size.xOffset - 2.5),
                    (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 - 2.5),
                    size.NODE_SIZE, size.NODE_SIZE};
                SDL_SetRenderDrawColor(renderer, 65, 105, 225, 255);
                SDL_RenderFillRect(renderer, &node_rect);
                node = node->next;
            }
        }
        else if (option == 3)
        {
            link(map, &size, window, renderer);
        }
        else if (option == 4)
        {
            // 清空窗口
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
        else if (option == 5)
        {
            // 清空窗口
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
        else if (option == 6)
        {
            // 清空窗口
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
        else if (option == 11)
        {
            SDL_Rect rect = {1050, 95, 1480, 960};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nM: display menu\nQ: exit mode\n\nSelect the input mode:\nEnter the node id: press(1)\nClick on node in map(2)\n", (SDL_Color){0, 0, 0}, 1000);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRect = {1050, 95, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
            bool quit = false;
            while (!quit)
            {
                int option = -1;
                SDL_Event eventM;
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
                            quit = true;
                            break;
                        case SDLK_1:
                            option = 1;
                            break;
                        case SDLK_2:
                            option = 2;
                            break;
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_m:
                            quit = true;
                            break;
                        default:
                            break;
                        }
                    }
                }
                node_t *start_node = NULL;
                node_t *end_node = NULL;
                if (option == -1)
                    continue;
                if (option == 1)
                {
                    int start_node_id = 0, end_node_id = 0;
                    printf("Enter start node ID: ");
                    scanf("%d", &start_node_id);
                    printf("Enter end node ID: ");
                    scanf("%d", &end_node_id);

                    node_t *current_node = map->nodes;
                    while (current_node != NULL)
                    {
                        if (current_node->id == start_node_id)
                        {
                            start_node = current_node;
                        }
                        if (current_node->id == end_node_id)
                        {
                            end_node = current_node;
                        }
                        current_node = current_node->next;
                    }

                    if (start_node == NULL || end_node == NULL)
                    {
                        printf("Error: Invalid node ID\n");
                        quit = true;
                        break;
                        // free_map(map);
                        // free(bound);
                        // return EXIT_BAD_DATA;
                    }
                }
                else if (option == 2)
                {
                    SDL_Event eventnode;
                    while (true)
                    {
                        SDL_WaitEvent(&eventnode);
                        if (eventnode.type == SDL_MOUSEBUTTONDOWN)
                        {
                            SDL_Rect rect = {1050, 700, 1480, 960};
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            node_t *node = map->nodes;
                            while (node != NULL)
                            {
                                if (eventnode.button.x >= (int)((node->lon + 1.565) * size.xRatio + size.xOffset - 2.5) &&
                                    eventnode.button.x <= (int)((node->lon + 1.565) * size.xRatio + size.xOffset + 2.5) &&
                                    eventnode.button.y >= (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 - 2.5) &&
                                    eventnode.button.y <= (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 + 2.5))
                                {
                                    start_node = node;
                                    char text[100];
                                    sprintf(text, "node: id: %d\n      lat: %lf\n      lon: %lf", node->id, node->lat, node->lon);
                                    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, (SDL_Color){220, 20, 60}, 1000);
                                    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                                    SDL_Rect textRect = {1050, 700, textSurface->w, textSurface->h};
                                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                                    SDL_RenderPresent(renderer);
                                    printf("Enter start node ID: %d\n", node->id);
                                    break;
                                }
                                node = node->next;
                            }
                            if (start_node != NULL)
                            {
                                break;
                            }
                        }
                    }
                    while (true)
                    {
                        SDL_WaitEvent(&eventnode);
                        if (eventnode.type == SDL_MOUSEBUTTONDOWN)
                        {
                            SDL_Rect rect = {1050, 700, 1480, 960};
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            node_t *node = map->nodes;
                            while (node != NULL)
                            {
                                if (eventnode.button.x >= (int)((node->lon + 1.565) * size.xRatio + size.xOffset - 2.5) &&
                                    eventnode.button.x <= (int)((node->lon + 1.565) * size.xRatio + size.xOffset + 2.5) &&
                                    eventnode.button.y >= (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 - 2.5) &&
                                    eventnode.button.y <= (int)((53.812 - node->lat) * size.yRatio + size.yOffset - 50 + 2.5))
                                {
                                    end_node = node;
                                    char text[100];
                                    sprintf(text, "node: id: %d\n      lat: %lf\n      lon: %lf", node->id, node->lat, node->lon);
                                    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, (SDL_Color){220, 20, 60}, 1000);
                                    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                                    SDL_Rect textRect = {1050, 700, textSurface->w, textSurface->h};
                                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                                    SDL_RenderPresent(renderer);
                                    printf("Enter end node ID: %d\n", node->id);
                                    break;
                                }
                                node = node->next;
                            }
                            if (end_node != NULL)
                            {
                                break;
                            }
                        }
                    }
                    dijkstra(map, start_node, end_node, path);
                    plotpath(map, path, &size, window, renderer);
                }

                axis(map, &size, window, renderer, 2);
            }
            if (quit)
            {
                SDL_Rect rect = {1050, 95, 1480, 960};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        axis(map, &size, window, renderer, 1);
    }

    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}