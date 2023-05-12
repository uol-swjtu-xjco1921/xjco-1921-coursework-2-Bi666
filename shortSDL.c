#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <math.h>
#include "map.h"
#include "dijkstra.h"
#include "optionSDL.h"

void add_speed(map_t *map)
{
    way_t *current_way = map->ways;
    // 设置随机数种子
    srand((unsigned)time(NULL));
    while (current_way->next != NULL)
    {
        // 生成随机速度限制
        int speed_li = (rand() % 50) + 30;
        current_way->speed_limit = speed_li;
        for (int i = 0; i < current_way->node_count - 1; i++)
        {
            int node1 = current_way->node[i];
            int node2 = current_way->node[i + 1];
            node_t *node = map->nodes;
            while (node != NULL)
            {
                if (node->id == node1 || node->id == node2)
                {
                    edge_t *edge = node->edges;
                    while (edge != NULL)
                    {
                        if ((edge->node1 == node1 && edge->node2 == node2) || (edge->node1 == node2 && edge->node2 == node1))
                        {
                            edge->speed = speed_li;
                            break;
                        }
                        edge = edge->next;
                    }
                }
                node = node->next;
            }
        }
        current_way = current_way->next;
    }
}

void route(map_t *map, sizeMap_t *size, path_t *path, SDL_Window *window, SDL_Renderer *renderer, int type)
{
    // 加载字体
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf", 24); // 24为字体大小
    if (font == NULL)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }

    SDL_Rect rect111 = {1050, 95, 1480, 960};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect111);
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
                    SDL_Rect node_rect1 = {1020, 275, 10, 10};
                    SDL_SetRenderDrawColor(renderer, 192, 14, 235, 255);
                    SDL_RenderFillRect(renderer, &node_rect1);
                    SDL_RenderPresent(renderer);
                    break;
                case SDLK_2:
                    option = 2;

                    SDL_Rect node_rect2 = {1020, 310, 10, 10};
                    SDL_SetRenderDrawColor(renderer, 192, 14, 235, 255);
                    SDL_RenderFillRect(renderer, &node_rect2);
                    SDL_RenderPresent(renderer);
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
            int quittext = 0;
            char inputText[256] = "";

            SDL_Surface *Surface = TTF_RenderText_Solid(font, "Enter start node: ", (SDL_Color){138, 43, 226});
            SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Surface);
            SDL_Rect textRect = {1050, 450, Surface->w, Surface->h};
            SDL_RenderCopy(renderer, Texture, NULL, &textRect);
            SDL_Rect inputBox = {1050, 500, 300, 40};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &inputBox);
            SDL_RenderPresent(renderer);

            SDL_Event e;
            inputText[0] = '\0';
            while (!quittext)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quittext = 1;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                            quittext = 1;
                            break;
                        case SDLK_BACKSPACE:
                            if (strlen(inputText) > 0)
                            {
                                if (strlen(inputText) == 1)
                                    inputText[0] = ' ';
                                else
                                    inputText[strlen(inputText) - 1] = '\0';

                                SDL_Rect rect = {1050, 500, 1350, 540};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText, (SDL_Color){100, 149, 237});
                                SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                                SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                                SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderDrawRect(renderer, &inputBox);
                                SDL_RenderPresent(renderer);
                            }
                        default:
                            break;
                        }
                    }
                    else if (e.type == SDL_TEXTINPUT)
                    {
                        strcat(inputText, e.text.text);

                        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText, (SDL_Color){100, 149, 237});
                        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                        SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                        SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);

                        SDL_RenderPresent(renderer);
                    }
                }
            }

            SDL_Rect rect = {1050, 450, 1480, 600};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_Surface *Surface1 = TTF_RenderText_Solid(font, "Enter end node: ", (SDL_Color){138, 43, 226});
            SDL_Texture *Texture1 = SDL_CreateTextureFromSurface(renderer, Surface1);
            SDL_Rect textRect1 = {1050, 450, Surface->w, Surface->h};
            SDL_RenderCopy(renderer, Texture1, NULL, &textRect1);
            SDL_Rect inputBox1 = {1050, 500, 300, 40};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &inputBox1);
            SDL_RenderPresent(renderer);

            char inputText1[256] = "";
            quittext = 0;
            inputText1[0] = '\0';
            SDL_Event e1;
            while (!quittext)
            {
                while (SDL_PollEvent(&e1) != 0)
                {
                    if (e1.type == SDL_QUIT)
                    {
                        quittext = 1;
                    }
                    else if (e1.type == SDL_KEYDOWN)
                    {
                        switch (e1.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                            quittext = 1;
                            break;
                        case SDLK_BACKSPACE:
                            if (strlen(inputText1) > 0)
                            {
                                if (strlen(inputText1) == 1)
                                    inputText1[0] = ' ';
                                else
                                    inputText1[strlen(inputText1) - 1] = '\0';

                                SDL_Rect rect = {1050, 500, 1350, 540};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText1, (SDL_Color){100, 149, 237});
                                SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                                SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                                SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderDrawRect(renderer, &inputBox);

                                SDL_RenderPresent(renderer);
                            }
                        default:
                            break;
                        }
                    }
                    else if (e1.type == SDL_TEXTINPUT)
                    {
                        strcat(inputText1, e1.text.text);

                        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText1, (SDL_Color){100, 149, 237});
                        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                        SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                        SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);

                        SDL_RenderPresent(renderer);
                    }
                }
            }
            int start_node_id = atoi(inputText);
            int end_node_id = atoi(inputText1);
            if (start_node_id == 0 || end_node_id == 0)
            {
                printf("Error: Invalid node ID\n");
                quit = true;
                SDL_Rect rectclean = {1020, 95, 1480, 960};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rectclean);
                return;
            }

            printf("Enter start node ID: %d\n", start_node_id);
            printf("Enter end node ID: %d\n", end_node_id);

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
            quit = true;
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
                        if (eventnode.button.x >= (int)((node->lon + 1.565) * size->xRatio + size->xOffset - 2.5) &&
                            eventnode.button.x <= (int)((node->lon + 1.565) * size->xRatio + size->xOffset + 2.5) &&
                            eventnode.button.y >= (int)((53.812 - node->lat) * size->yRatio + size->yOffset - 50 - 2.5) &&
                            eventnode.button.y <= (int)((53.812 - node->lat) * size->yRatio + size->yOffset - 50 + 2.5))
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
                        if (eventnode.button.x >= (int)((node->lon + 1.565) * size->xRatio + size->xOffset - 2.5) &&
                            eventnode.button.x <= (int)((node->lon + 1.565) * size->xRatio + size->xOffset + 2.5) &&
                            eventnode.button.y >= (int)((53.812 - node->lat) * size->yRatio + size->yOffset - 50 - 2.5) &&
                            eventnode.button.y <= (int)((53.812 - node->lat) * size->yRatio + size->yOffset - 50 + 2.5))
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
        }

        if (type == 1)
        {
            dijkstra(map, start_node, end_node, path, 1);
            plotpath(map, path, size, window, renderer, 1);
        }
        else
        {
            dijkstra(map, start_node, end_node, path, 2);
            plotpath(map, path, size, window, renderer, 2);
        }
        quit = true;
    }
    SDL_Rect rectclean = {1020, 95, 1480, 960};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rectclean);
}
