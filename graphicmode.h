#ifndef GRAPHICMODE_H_INCLUDED
#define GRAPHICMODE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "trips.h"
#include "listings.h"

#define TABLE_SIZE 650        // main space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define MAX_BOARD_POS 11      // maximum size of the board
#define MARGIN 5

// declaration of the functions related to graphical issues
void graphicmode(char**, tripnode**, stationnode**);
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
void RenderTable(int, TTF_Font *, SDL_Surface **, SDL_Renderer *, tripnode**, stationnode**);
void RenderBoard(int [][MAX_BOARD_POS], SDL_Surface **, int, int, int, SDL_Renderer *);
void DrawCircle (SDL_Renderer*, int, int, int, int, int, int);
void DrawRoutes(int, int, stationnode**, tripnode**, SDL_Window*, SDL_Renderer*);
void graphicroutelisting(tripnode**, stationnode**, int, SDL_Renderer*);
void graph_routelist(tripnode**, stationnode**, tripnode*, int, int[STATIONMAX], int[STATIONMAX], SDL_Renderer*);


#endif // GRAPHICMODE_H_INCLUDED
