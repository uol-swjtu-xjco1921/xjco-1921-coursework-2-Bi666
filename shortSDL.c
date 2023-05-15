#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "map.h"
#include "createMap.h"
#include "dijkstra.h"
#include "optionSDL.h"

int route(map_t *map, sizeMap_t *size, range_t *bound, path_t *path, SDL_Window *window, SDL_Renderer *renderer, int type)
{
    // Loading Fonts
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf", 24);
    if (font == NULL)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        return EXIT_SDL_FAILED;
    }

    // Rewrite the menu bar
    SDL_Rect rect111 = {1050, 95, 450, 865};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect111);
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "Menu\nQ: exit mode\nM: exit mode\n\nSelect the input mode:\nEnter the node id: press(1)\nClick on node in map(2)\n", (SDL_Color){0, 0, 0}, 1000);
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
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                freeMap(map, bound, path);
                exit(EXIT_NO_ERRORS);
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

        // Text box input start end node
        if (option == 1)
        {
            char inputText[256] = "";
            SDL_Surface *Surface = TTF_RenderText_Solid(font, "Enter start node: ", (SDL_Color){138, 43, 226});
            SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Surface);
            SDL_Rect textRect = {1050, 400, Surface->w, Surface->h};
            SDL_RenderCopy(renderer, Texture, NULL, &textRect);
            SDL_Rect inputBox = {1050, 450, 300, 40};
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

                                SDL_Rect rect = {1020, 450, 450, 40};
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
            // Refresh the page to enter the end node
            SDL_Rect rect = {1020, 400, 450, 200};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_Surface *Surface1 = TTF_RenderText_Solid(font, "Enter end node: ", (SDL_Color){138, 43, 226});
            SDL_Texture *Texture1 = SDL_CreateTextureFromSurface(renderer, Surface1);
            SDL_Rect textRect1 = {1050, 400, Surface->w, Surface->h};
            SDL_RenderCopy(renderer, Texture1, NULL, &textRect1);
            SDL_Rect inputBox1 = {1050, 450, 300, 40};
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
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        freeMap(map, bound, path);
                        exit(EXIT_NO_ERRORS);
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

                                SDL_Rect rect = {1020, 450, 450, 40};
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

            // Check if the start end node is legal
            int start_node_id = atoi(inputText);
            int end_node_id = atoi(inputText1);
            if (start_node_id == 0 || end_node_id == 0)
            {
                printf("Error: Invalid node ID\n");
                quit = true;
                SDL_Rect rectclean = {1020, 95, 460, 865};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rectclean);
                break;
            }

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
            }
            printf("Enter start node ID: %d\n", start_node_id);
            printf("Enter end node ID: %d\n", end_node_id);

            quit = true;
        }
        else if (option == 2)
        {
            SDL_Event eventnode;
            while (true)
            {
                SDL_WaitEvent(&eventnode);
                if (eventnode.type == SDL_QUIT)
                {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();

                    freeMap(map, bound, path);
                    exit(EXIT_NO_ERRORS);
                }
                else if (eventnode.type == SDL_MOUSEBUTTONDOWN)
                {
                    SDL_Rect rect = {1020, 700, 450, 260};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    node_t *node = map->nodes;
                    while (node != NULL)
                    {
                        if (eventnode.button.x >= (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset - 2.5) &&
                            eventnode.button.x <= (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset + 2.5) &&
                            eventnode.button.y >= (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 - 2.5) &&
                            eventnode.button.y <= (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 + 2.5))
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
                    SDL_Rect rect = {1020, 700, 450, 260};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    node_t *node = map->nodes;
                    while (node != NULL)
                    {
                        if (eventnode.button.x >= (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset - 2.5) &&
                            eventnode.button.x <= (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset + 2.5) &&
                            eventnode.button.y >= (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 - 2.5) &&
                            eventnode.button.y <= (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 + 2.5))
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
            int dijresult = dijkstra(map, start_node, end_node, path, 1);
            if (dijresult != EXIT_NO_ERRORS && dijresult != EXIT_NO_PATH_FOUND)
            {
                return dijresult;
            }
            plotpath(map, path, path, size, bound, window, renderer, 1, dijresult);
        }
        else if (type == 2)
        {
            int dijresult = dijkstra(map, start_node, end_node, path, 2);
            if (dijresult != EXIT_NO_ERRORS && dijresult != EXIT_NO_PATH_FOUND)
            {
                return dijresult;
            }
            plotpath(map, path, path, size, bound, window, renderer, 2, dijresult);
        }
        else
        {
            // Waiting for input a pass location
            SDL_Rect rectif = {1050, 400, 450, 150};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rectif);

            char inputText3[256] = "";
            SDL_Surface *Surface = TTF_RenderText_Solid(font, "Enter a pass location: ", (SDL_Color){138, 43, 226});
            SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Surface);
            SDL_Rect textRect = {1050, 400, Surface->w, Surface->h};
            SDL_RenderCopy(renderer, Texture, NULL, &textRect);
            SDL_Rect inputBox = {1050, 450, 300, 40};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &inputBox);

            SDL_Event e;
            int quittext = 0;
            inputText3[0] = '\0';
            node_t *pass_node = NULL;
            SDL_RenderPresent(renderer);
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
                        case SDLK_ESCAPE:
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                            quittext = 1;
                            break;
                        case SDLK_BACKSPACE:
                            if (strlen(inputText3) > 0)
                            {
                                if (strlen(inputText3) == 1)
                                    inputText3[0] = ' ';
                                else
                                    inputText3[strlen(inputText3) - 1] = '\0';

                                SDL_Rect rect = {1020, 450, 450, 40};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText3, (SDL_Color){100, 149, 237});
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
                        strcat(inputText3, e.text.text);

                        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText3, (SDL_Color){100, 149, 237});
                        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                        SDL_Rect textRect = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                        SDL_RenderCopy(renderer, inputTexture, NULL, &textRect);

                        SDL_RenderPresent(renderer);
                    }
                    else if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        // Iterate through the array of points
                        node_t *node = map->nodes;
                        while (node != NULL)
                        {
                            // Find if there is a mouse click on the point
                            if (e.button.x >= (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset - 2.5) &&
                                e.button.x <= (int)((node->lon - bound->minLon + 0.001) * size->xRatio + size->xOffset + 2.5) &&
                                e.button.y >= (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 - 2.5) &&
                                e.button.y <= (int)((bound->maxLat - node->lat - 0.001) * size->yRatio + size->yOffset - 50 + 2.5))
                            {
                                SDL_Rect rectnode = {1020, 700, 450, 200};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &rectnode);
                                pass_node = node;
                                char text[100];
                                sprintf(text, "node: id: %d\n      lat: %lf\n      lon: %lf", node->id, node->lat, node->lon);
                                SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, (SDL_Color){220, 20, 60}, 1000);
                                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                                SDL_Rect textRect = {1050, 700, textSurface->w, textSurface->h};
                                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                                char str[50] = "";
                                sprintf(str, "%d", node->id);
                                SDL_Rect rect = {1020, 450, 450, 40};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_Surface *inputSurface = TTF_RenderText_Solid(font, str, (SDL_Color){100, 149, 237});
                                SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
                                SDL_Rect textRect33 = {inputBox.x + 10, inputBox.y + 5, inputSurface->w, inputSurface->h};
                                SDL_RenderCopy(renderer, inputTexture, NULL, &textRect33);
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderDrawRect(renderer, &inputBox);
                                SDL_RenderPresent(renderer);

                                quittext = 1;
                                break;
                            }
                            node = node->next;
                        }
                    }
                }
            }
            // Check if the pass node is legal
            if (pass_node == NULL)
            {
                int pass_node_id = atoi(inputText3);
                if (pass_node_id == 0)
                {
                    printf("Error: Invalid node ID\n");
                    quit = true;
                    SDL_Rect rectclean = {1050, 95, 450, 865};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rectclean);
                    break;
                }
                node_t *current_node = map->nodes;
                while (current_node != NULL)
                {
                    if (current_node->id == pass_node_id)
                    {
                        pass_node = current_node;
                        break;
                    }
                    current_node = current_node->next;
                }

                if (pass_node == NULL)
                {
                    printf("Error: Invalid node ID\n");
                    quit = true;
                    break;
                }
            }
            printf("Enter pass node ID: %d\n", pass_node->id);

            SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, "S: shortest route\nT: quickest route\n", (SDL_Color){138, 43, 226}, 1000);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRectif = {1050, 550, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRectif);
            SDL_RenderPresent(renderer);

            // Choose to draw the shortest path or the quickest time
            SDL_Event eventM;
            path_t *path1 = (path_t *)malloc(sizeof(path_t));
            bool quitif = false;
            while (!quitif)
            {
                while (SDL_PollEvent(&eventM))
                {
                    if (eventM.type == SDL_QUIT)
                    {
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();

                        freeMap(map, bound, path);
                        exit(EXIT_NO_ERRORS);
                    }
                    else if (eventM.type == SDL_KEYDOWN)
                    {
                        int dijresult, dij, dijresult1, dij1;
                        switch (eventM.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                            quitif = true;
                            break;
                        case SDLK_s:
                            dijresult = dijkstra(map, start_node, pass_node, path, 1);
                            if (dijresult != EXIT_NO_ERRORS && dijresult != EXIT_NO_PATH_FOUND)
                            {
                                return dijresult;
                            }
                            dij = dijkstra(map, pass_node, end_node, path1, 1);
                            if (dij != EXIT_NO_ERRORS && dij != EXIT_NO_PATH_FOUND)
                            {
                                return dij;
                            }
                            if (dijresult != EXIT_NO_PATH_FOUND && dij != EXIT_NO_PATH_FOUND)
                            {
                                plotpath(map, path, path1, size, bound, window, renderer, 1, EXIT_NO_PATH_FOUND);
                            }
                            else
                            {
                                plotpath(map, path, path1, size, bound, window, renderer, 1, EXIT_NO_ERRORS);
                            }
                            quitif = true;
                            break;
                        case SDLK_t:
                            dijresult1 = dijkstra(map, start_node, pass_node, path, 2);
                            if (dijresult1 != EXIT_NO_ERRORS && dijresult1 != EXIT_NO_PATH_FOUND)
                            {
                                return dijresult1;
                            }
                            dij1 = dijkstra(map, pass_node, end_node, path1, 2);
                            if (dij1 != EXIT_NO_ERRORS && dij1 != EXIT_NO_PATH_FOUND)
                            {
                                return dij1;
                            }
                            if (dijresult1 != EXIT_NO_PATH_FOUND && dij1 != EXIT_NO_PATH_FOUND)
                            {
                                plotpath(map, path, path1, size, bound, window, renderer, 2, EXIT_NO_PATH_FOUND);
                            }
                            else
                            {
                                plotpath(map, path, path1, size, bound, window, renderer, 2, EXIT_NO_ERRORS);
                            }
                            quitif = true;
                            break;
                        case SDLK_q:
                        case SDLK_m:
                            quitif = true;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
        quit = true;
    }
    SDL_Rect rectclean = {1020, 95, 450, 865};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rectclean);
    return EXIT_NO_ERRORS;
}
