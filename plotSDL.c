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

void sdl(map_t *map, path_t *path, int option)
{
    // 初始化 SDL
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口和渲染器
    SDL_Window *window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1380, 960, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 计算绘图缩放比例和平移量
    double latRange = 53.812 - 53.800;
    double lonRange = 1.565 - 1.540;
    double xRatio = 850.0 / lonRange;
    double yRatio = 840.0 / latRange;
    double xOffset = 150.0;
    double yOffset = 120.0;
    int NODE_SIZE = 5;


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //设置渲染器颜色为白色
    SDL_RenderClear(renderer); //清空渲染器，填充白色

    if (path->count == 0 && option == 0)
    {
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
                (int)((node->lon + 1.565) * xRatio + xOffset - 2.5),
                (int)((53.812 - node->lat) * yRatio + yOffset - 50 - 2.5),
                NODE_SIZE, NODE_SIZE};
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
                    int x1 = (int)((node1->lon + 1.565) * xRatio + xOffset);
                    int y1 = (int)((53.812 - node1->lat) * yRatio + yOffset - 50);
                    int x2 = (int)((node2->lon + 1.565) * xRatio + xOffset);
                    int y2 = (int)((53.812 - node2->lat) * yRatio + yOffset - 50);

                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
                edge = edge->next;
            }
            node = node->next;
        }
    }
    else if (path->count == 0 && option == 1)
    {
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
                    int x1 = (int)((node1->lon + 1.565) * xRatio + xOffset);
                    int y1 = (int)((53.812 - node1->lat) * yRatio + yOffset - 50);
                    int x2 = (int)((node2->lon + 1.565) * xRatio + xOffset);
                    int y2 = (int)((53.812 - node2->lat) * yRatio + yOffset - 50);
                    if (edge->veg <= edge->length && edge->veg > 0) {
                        SDL_SetRenderDrawColor(renderer, 0, 250, 154, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else if (edge->veg > edge->length){
                        SDL_SetRenderDrawColor(renderer, 46, 139, 87, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
                edge = edge->next;
            }
            node = node->next;
        }
    }
    else if (path->count == 0 && option == 2)
    {
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
                    int x1 = (int)((node1->lon + 1.565) * xRatio + xOffset);
                    int y1 = (int)((53.812 - node1->lat) * yRatio + yOffset - 50);
                    int x2 = (int)((node2->lon + 1.565) * xRatio + xOffset);
                    int y2 = (int)((53.812 - node2->lat) * yRatio + yOffset - 50);
                    if (edge->arch <= edge->length && edge->arch > 0) {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else if (edge->arch > edge->length){
                        SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
                edge = edge->next;
            }
            node = node->next;
        }
    }
    else if (path->count == 0 && option == 3)
    {
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
                    int x1 = (int)((node1->lon + 1.565) * xRatio + xOffset);
                    int y1 = (int)((53.812 - node1->lat) * yRatio + yOffset - 50);
                    int x2 = (int)((node2->lon + 1.565) * xRatio + xOffset);
                    int y2 = (int)((53.812 - node2->lat) * yRatio + yOffset - 50);
                    if (edge->land <= edge->length && edge->land > 0) {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else if (edge->land > edge->length){
                        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
                edge = edge->next;
            }
            node = node->next;
        }
    }
    else {
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
                (int)((node->lon + 1.565) * xRatio + xOffset - 2.5),
                (int)((53.812 - node->lat) * yRatio + yOffset - 50 - 2.5),
                NODE_SIZE, NODE_SIZE};
            int judge = -1;
            for (int i = 0; i < path->count; i++){
                if (path->pathCount[i] == node->count) {
                    judge = 0;
                    break;
                }
            }
            if (judge == -1) {
                SDL_SetRenderDrawColor(renderer, 65, 105, 225, 255);
                SDL_RenderFillRect(renderer, &node_rect);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 220, 20 , 60, 255);
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
                    int x1 = (int)((node1->lon + 1.565) * xRatio + xOffset);
                    int y1 = (int)((53.812 - node1->lat) * yRatio + yOffset - 50);
                    int x2 = (int)((node2->lon + 1.565) * xRatio + xOffset);
                    int y2 = (int)((53.812 - node2->lat) * yRatio + yOffset - 50);

                    int judge = 2;
                    for (int i = 0; i < path->count; i++){
                        if (path->pathCount[i] == node1->count || path->pathCount[i] == node2->count) 
                            judge--;
                        if (judge == 0) break;
                    }
                    if (judge != 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255);
                        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                    }
                }
                edge = edge->next;
            }
            node = node->next;
        }
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, xOffset, 960 - yOffset, 1080 - xOffset + 60, 960 - yOffset);
    SDL_RenderDrawLine(renderer, 1080 - xOffset + 60, 960 - yOffset, 1080 - xOffset + 60, yOffset - 50);
    SDL_RenderDrawLine(renderer, xOffset, yOffset - 50, 1080 - xOffset + 60, yOffset - 50);
    SDL_RenderDrawLine(renderer, xOffset, 960 - yOffset, xOffset, yOffset - 50);

    // 绘制X轴标尺和文本
    for (int i = 0; i <= 5; i++)
    {
        int x = (int)(i * 168) + xOffset;
        int y = 711 + yOffset;
        SDL_RenderDrawLine(renderer, x, y, x, y + 10);
        SDL_RenderDrawLine(renderer, x, y - 761, x, y - 761 + 10);
        char text[16];
        sprintf(text, "%.3f", (-1.565) + i * (lonRange / 5.0));
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
        int x = xOffset;
        int y = 790- (int)(i * 70) + yOffset;
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x + 830, y, x + 830 + 10, y);
        char text[16];
        sprintf(text, "%.3f", 53.80 + i * (latRange / 12.0));
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){0, 0, 0});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x - 30 - surface->w, y - surface->h / 2, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "'map.out'", (SDL_Color){0, 0, 0});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {710, 80, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderDrawLine(renderer, 850, 96, 910, 96);
    SDL_Rect rect1 = { 880, 94, NODE_SIZE, NODE_SIZE};
    SDL_RenderFillRect(renderer, &rect1);
    
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nM: display menu\n\nShow the map:\n1: original map\n2: link\n3: veg\n4: arch\n5: land\n6: way\n7: geom", (SDL_Color){0, 0, 0}, 1000);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = {1080, 95,  textSurface->w, textSurface->h};
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    
    // 显示地图
    SDL_RenderPresent(renderer);

    SDL_Event event;
    // 等待用户关闭窗口
    while (true)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            SDL_PollEvent(&event);
            break;
        }
    }
    
    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}