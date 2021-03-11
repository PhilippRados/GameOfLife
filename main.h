#include <stdio.h>
#include <stdlib.h>
#include "./include/SDL2/SDL.h"
#include "include/SDL2/SDL_error.h"
#include "include/SDL2/SDL_events.h"
#include "include/SDL2/SDL_keycode.h"
#include "include/SDL2/SDL_messagebox.h"
#include "include/SDL2/SDL_pixels.h"
#include "include/SDL2/SDL_rect.h"
#include "include/SDL2/SDL_render.h"
#include "include/SDL2/SDL_stdinc.h"
#include "include/SDL2/SDL_surface.h"
#include "include/SDL2/SDL_video.h"
#include <stdbool.h>
#include <unistd.h>

#define RECT_SIZE 20
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define WIDTH (SCREEN_WIDTH / RECT_SIZE)
#define HEIGHT (SCREEN_HEIGHT / RECT_SIZE)

typedef bool multi_arr[WIDTH][HEIGHT];

int getLastMultiple(int number);
void colorRect(SDL_Renderer *renderer, SDL_Rect r,SDL_Color color);
void drawGrid(SDL_Renderer *renderer,SDL_Color color);
void displayImg(SDL_Renderer *renderer,char* path);
void updateGame(SDL_Window *window,SDL_Renderer *renderer, SDL_Rect r,bool paused);
int countAliveNeighbors(int x, int y);
void copyArray(multi_arr arr1,multi_arr arr2);
void nextEpoch();
bool buttonPress(int x,int y);
Uint32 my_callbackfunc(Uint32 interval, void *param);
