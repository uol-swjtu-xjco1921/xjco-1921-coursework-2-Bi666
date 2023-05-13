#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "optionSDL.h"
#include "shortSDL.h"

void sdl(map_t *map, path_t *path, range_t *bound)
{
    sizeMap_t size;
    initsize(&size, bound);
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

    origin(map, &size, bound, window, renderer); // 绘制原始地图
    axis(map, &size, bound, window, renderer, 1);
    //随机添加速度属性
    add_speed(map);

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
                case SDLK_q:
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
                case SDLK_t:
                    option = 12;
                    break;
                case SDLK_p:
                    option = 13;
                    break;
                default:
                    break;
                }
            }
            else if (eventM.type == SDL_MOUSEBUTTONDOWN)
            {
                // 遍历点数组，查找是否有鼠标点击的点
                SDL_Rect rect = {1050, 720, 430, 240};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                node_t *node = map->nodes;
                while (node != NULL)
                {
                    if (eventM.button.x >= (int)((node->lon - bound->minLon + 0.001) * size.xRatio + size.xOffset - 2.5) &&
                        eventM.button.x <= (int)((node->lon - bound->minLon + 0.001) * size.xRatio + size.xOffset + 2.5) &&
                        eventM.button.y >= (int)((bound->maxLat - node->lat - 0.001) * size.yRatio + size.yOffset - 50 - 2.5) &&
                        eventM.button.y <= (int)((bound->maxLat - node->lat - 0.001) * size.yRatio + size.yOffset - 50 + 2.5))
                    {
                        char text[100];
                        sprintf(text, "node: id: %d\n      lat: %lf\n      lon: %lf", node->id, node->lat, node->lon);
                        SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, (SDL_Color){220, 20, 60}, 1000);
                        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                        SDL_Rect textRect = {1050, 720, textSurface->w, textSurface->h};
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
            origin(map, &size, bound, window, renderer);
        }
        else if (option == 2)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            node_t *node = map->nodes;
            while (node != NULL)
            {
                SDL_Rect node_rect = {
                    (int)((node->lon - bound->minLon + 0.001) * size.xRatio + size.xOffset - 2.5),
                    (int)((bound->maxLat - node->lat - 0.001) * size.yRatio + size.yOffset - 50 - 2.5),
                    size.NODE_SIZE, size.NODE_SIZE};
                SDL_SetRenderDrawColor(renderer, 65, 105, 225, 255);
                SDL_RenderFillRect(renderer, &node_rect);
                node = node->next;
            }
        }
        else if (option == 3)
        {
            link(map, &size, bound, window, renderer);
        }
        else if (option == 4)
        {
            extra(map, &size, bound, window, renderer, 1);
        }
        else if (option == 5)
        {
            extra(map, &size, bound, window, renderer, 2);
        }
        else if (option == 6)
        {
            extra(map, &size, bound, window, renderer, 3);
        }
        else if (option == 11)
        {
            route(map, &size, bound, path, window, renderer, 1);
        }
        else if (option == 12)
        {
            route(map, &size, bound, path, window, renderer, 2);
        }
        
        else if (option == 13)
        {
            route(map, &size, bound, path, window, renderer, 3);
        }
        axis(map, &size, bound, window, renderer, 1);
    }

    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


