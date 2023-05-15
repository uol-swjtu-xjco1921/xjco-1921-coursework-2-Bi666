#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "shortSDL.h"
#include "createMap.h"
#include "optionSDL.h"

int sdl(map_t *map, path_t *path, range_t *bound)
{
    sizeMap_t size;
    initsize(&size, bound);
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Initialize TTF library
    if (TTF_Init() < 0)
    {
        printf("Error initializing TTF library: %s\n", TTF_GetError());
        return EXIT_SDL_FAILED;
    }

    // Loading Fonts
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf", 24);
    // 24 is the font size
    if (font == NULL)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        return EXIT_SDL_FAILED;
    }

    // Creating windows and renderers
    SDL_Window *window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1500, 960, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    origin(map, &size, bound, window, renderer); // Drawing the original map
    int result = axis(map, &size, bound, window, renderer, 1);
    if (result != EXIT_NO_ERRORS)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return result;
    }

    // Get user keyboard input events
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
                case SDLK_e:
                    option = 14;
                    break;
                default:
                    break;
                }
            }
            else if (eventM.type == SDL_MOUSEBUTTONDOWN)
            {
                // Iterate through the array of points
                SDL_Rect rect = {1050, 720, 430, 240};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                node_t *node = map->nodes;
                while (node != NULL)
                {
                    // Find if there is a mouse click on the point
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
        else if (option == 7)
        {
            geom(map, &size, bound, window, renderer);
        }
        else if (option == 11)
        {
            int routeresult = route(map, &size, bound, path, window, renderer, 1);
            if (routeresult != EXIT_NO_ERRORS)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return routeresult;
            }
        }
        else if (option == 12)
        {
            int routeresult = route(map, &size, bound, path, window, renderer, 2);
            if (routeresult != EXIT_NO_ERRORS)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return routeresult;
            }
        }
        else if (option == 13)
        {
            int routeresult = route(map, &size, bound, path, window, renderer, 3);
            if (routeresult != EXIT_NO_ERRORS)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return routeresult;
            }
        }
        else if (option == 14)
        {
            // Modify Map Properties
            SDL_Rect rect111 = {1050, 95, 450, 865};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect111);
            SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nM: exit mode\nQ: exit mode\n\nSelect the attribute:\n1: speed of way\n2: veg of link\n3: arch of link\n4: land of link\n", (SDL_Color){0, 0, 0}, 1000);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {1050, 95, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);

            bool quita = false;
            while (!quita)
            {
                SDL_Event eventa;
                int option = -1;
                while (SDL_PollEvent(&eventa))
                {
                    if (eventa.type == SDL_QUIT)
                    {
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        freeMap(map, bound, path);
                        exit(EXIT_NO_ERRORS);
                    }
                    else if (eventa.type == SDL_KEYDOWN)
                    {
                        switch (eventa.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                            quita = true;
                            break;
                        case SDLK_q:
                            quita = true;
                            break;
                        case SDLK_m:
                            quita = true;
                            break;
                        case SDLK_1:
                            option = 1;
                            break;
                        case SDLK_2:
                            option = 2;
                            break;
                        case SDLK_3:
                            option = 2;
                            break;
                        case SDLK_4:
                            option = 2;
                            break;
                        default:
                            break;
                        }
                    }
                }

                // Prompt for way id
                if (option == 1)
                {
                    char inputText[256] = "";
                    char inputText1[256] = "";
                    SDL_Surface *Surface = TTF_RenderText_Solid(font, "Enter the id of way: ", (SDL_Color){138, 43, 226});
                    SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Surface);
                    SDL_Rect textRect1 = {1050, 450, Surface->w, Surface->h};
                    SDL_RenderCopy(renderer, Texture, NULL, &textRect1);
                    SDL_Rect inputBox = {1050, 500, 300, 40};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &inputBox);
                    SDL_RenderPresent(renderer);

                    SDL_Event e;
                    int quittext = 0;
                    inputText[0] = '\0';
                    while (!quittext)
                    {
                        while (SDL_PollEvent(&e) != 0)
                        {
                            if (e.type == SDL_QUIT)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                                freeMap(map, bound, path);
                                exit(EXIT_NO_ERRORS);
                            }
                            else if (e.type == SDL_KEYDOWN)
                            {
                                switch (e.key.keysym.sym)
                                {
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

                                        SDL_Rect rect = {1020, 500, 450, 40};
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
                    // Enter road speed attributes
                    SDL_Rect rect111 = {1050, 450, 450, 150};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect111);
                    SDL_Surface *Surface1 = TTF_RenderText_Solid(font, "Enter limited speed of way: ", (SDL_Color){138, 43, 226});
                    SDL_Texture *Texture1 = SDL_CreateTextureFromSurface(renderer, Surface1);
                    SDL_Rect textRect2 = {1050, 450, Surface1->w, Surface1->h};
                    SDL_RenderCopy(renderer, Texture1, NULL, &textRect2);
                    SDL_Rect inputBox2 = {1050, 500, 300, 40};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &inputBox2);
                    SDL_RenderPresent(renderer);
                    SDL_Event eve;
                    quittext = 0;
                    inputText1[0] = '\0';
                    while (!quittext)
                    {
                        while (SDL_PollEvent(&eve) != 0)
                        {
                            if (eve.type == SDL_QUIT)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                                freeMap(map, bound, path);
                                exit(EXIT_NO_ERRORS);
                            }
                            else if (eve.type == SDL_KEYDOWN)
                            {
                                switch (eve.key.keysym.sym)
                                {
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

                                        SDL_Rect rect = {1020, 500, 450, 40};
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
                            else if (eve.type == SDL_TEXTINPUT)
                            {
                                strcat(inputText1, eve.text.text);

                                SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText1, (SDL_Color){100, 149, 237});
                                SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                                SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                                SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);
                                SDL_RenderPresent(renderer);
                            }
                        }
                    }
                    int way_id = atoi(inputText);
                    int speed = atoi(inputText1);
                    if (way_id == 0)
                    {
                        printf("Error: Invalid Way ID\n");
                        SDL_Rect rectclean = {1020, 95, 460, 865};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &rectclean);
                        SDL_RenderPresent(renderer);
                        break;
                    }
                    if (speed == 0)
                    {
                        printf("Error: Invalid Limited Speed Of Way\n");
                        SDL_Rect rectclean = {1020, 95, 460, 865};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &rectclean);
                        SDL_RenderPresent(renderer);
                        break;
                    }
                    if (speed < 20 || speed > 80)
                    {
                        printf("Error: Speed limit needs to be between 20 and 80\n");
                        SDL_Rect rectclean = {1020, 95, 460, 865};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &rectclean);
                        SDL_RenderPresent(renderer);
                        break;
                    }
                    way_t *cur_way = map->ways;
                    while (cur_way->next != NULL)
                    {
                        if (cur_way->id == way_id)
                        {
                            cur_way->speed_limit == speed;
                            break;
                        }
                        cur_way = cur_way->next;
                    }
                    if (cur_way->next == NULL)
                    {
                        printf("Error: Invalid Way ID\n");
                        SDL_Rect rectclean = {1020, 95, 460, 865};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &rectclean);
                        SDL_RenderPresent(renderer);
                        break;
                    }
                }
                else if (option == 2)
                {
                    char inputText[256] = "";
                    char inputText1[256] = "";
                    SDL_Surface *Surface = TTF_RenderText_Solid(font, "Enter the id of link: ", (SDL_Color){138, 43, 226});
                    SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Surface);
                    SDL_Rect textRect1 = {1050, 450, Surface->w, Surface->h};
                    SDL_RenderCopy(renderer, Texture, NULL, &textRect1);
                    SDL_Rect inputBox = {1050, 500, 300, 40};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &inputBox);
                    SDL_RenderPresent(renderer);

                    SDL_Event e;
                    int quittext = 0;
                    inputText[0] = '\0';
                    while (!quittext)
                    {
                        while (SDL_PollEvent(&e) != 0)
                        {
                            if (e.type == SDL_QUIT)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                                freeMap(map, bound, path);
                                exit(EXIT_NO_ERRORS);
                            }
                            else if (e.type == SDL_KEYDOWN)
                            {
                                switch (e.key.keysym.sym)
                                {
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

                                        SDL_Rect rect = {1020, 500, 450, 40};
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

                    SDL_Rect rect111 = {1050, 450, 450, 150};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect111);
                    SDL_Surface *Surface1 = TTF_RenderText_Solid(font, "Enter the value of attribute: ", (SDL_Color){138, 43, 226});
                    SDL_Texture *Texture1 = SDL_CreateTextureFromSurface(renderer, Surface1);
                    SDL_Rect textRect2 = {1050, 450, Surface1->w, Surface1->h};
                    SDL_RenderCopy(renderer, Texture1, NULL, &textRect2);
                    SDL_Rect inputBox1 = {1050, 500, 300, 40};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &inputBox1);
                    SDL_RenderPresent(renderer);
                    SDL_Event e2;
                    quittext = 0;
                    inputText1[0] = '\0';
                    while (!quittext)
                    {
                        while (SDL_PollEvent(&e2) != 0)
                        {
                            if (e2.type == SDL_QUIT)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                                freeMap(map, bound, path);
                                exit(EXIT_NO_ERRORS);
                            }
                            else if (e2.type == SDL_KEYDOWN)
                            {
                                switch (e2.key.keysym.sym)
                                {
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

                                        SDL_Rect rect = {1020, 500, 450, 40};
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
                            else if (e2.type == SDL_TEXTINPUT)
                            {
                                strcat(inputText1, e2.text.text);

                                SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText1, (SDL_Color){100, 149, 237});
                                SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                                SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                                SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);

                                SDL_RenderPresent(renderer);
                            }
                        }
                    }
                    int link_id = atoi(inputText);
                    int att = atoi(inputText1);
                    if (link_id == 0)
                    {
                        printf("Error: Invalid link ID\n");
                    }
                    if (att == 0)
                    {
                        printf("Error: Invalid attribute value\n");
                    }
                    int linkif = 0;
                    node_t *c_node = map->nodes;
                    while (c_node != NULL)
                    {
                        edge_t *cur_edge = c_node->edges;
                        while (cur_edge != NULL)
                        {
                            if (cur_edge->id == link_id)
                            {
                                linkif = 1;
                                break;
                            }
                            cur_edge = cur_edge->next;
                        }
                        if (linkif == 1)
                            break;
                        c_node = c_node->next;
                    }
                    if (linkif == 0)
                    {
                        printf("Error: Invalid Way ID\n");
                    }
                    SDL_Rect rectclean = {1020, 95, 460, 865};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rectclean);
                    SDL_RenderPresent(renderer);
                    break;
                }
            }
            SDL_Rect rectad = {1050, 95, 450, 865};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rectad);
        }
        int resultplot = axis(map, &size, bound, window, renderer, 1);
        if (resultplot != EXIT_NO_ERRORS)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return resultplot;
        }
    }

    // Release Resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_NO_ERRORS;
}
