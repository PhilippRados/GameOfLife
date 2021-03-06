#include <stdio.h>
#include <stdlib.h>
#include "./include/SDL2/SDL.h"
#include "include/SDL2/SDL_error.h"
#include "include/SDL2/SDL_events.h"
#include "include/SDL2/SDL_keycode.h"
#include "include/SDL2/SDL_pixels.h"
#include "include/SDL2/SDL_render.h"
#include "include/SDL2/SDL_stdinc.h"
#include "include/SDL2/SDL_surface.h"
#include "include/SDL2/SDL_video.h"
#include <stdbool.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define RECT_SIZE 20
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define WIDTH (SCREEN_WIDTH / RECT_SIZE)
#define HEIGHT (SCREEN_HEIGHT / RECT_SIZE)

typedef bool multi_arr[WIDTH][HEIGHT];
multi_arr Field = {0,};
multi_arr nextState = {0,};

SDL_Color blue = {0,0,255,255};
SDL_Color black = {0,0,0,255};
SDL_Color white = {244,244,244,50};


int getNearestMultiple(int number){
    int multiple = 20;
    float remainder;
    if ((remainder = (number % multiple)) == 0)
        return number;
    else{
        if (remainder >= (multiple / 2)){
            number += multiple - remainder;
        }
        else {
            number -= remainder;
        }
    }
    return number;
}

void colorRect(SDL_Renderer *renderer, SDL_Rect r,int mouseX, int mouseY,SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b ,255);

    // Render rect
    SDL_RenderFillRect(renderer, &r);
}


void drawGrid(SDL_Renderer *renderer,SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
    for (int i = 20; i < SCREEN_WIDTH; i += 20){
        SDL_RenderDrawLine(renderer, i,0,i,SCREEN_HEIGHT);
    }
    for (int i = 20; i < SCREEN_HEIGHT;i += 20){
        SDL_RenderDrawLine(renderer, 0,i,SCREEN_WIDTH,i);
    }
}

void updateGame(SDL_Window *window,SDL_Renderer *renderer, SDL_Rect r){
    //Loop through field-array and show rectangles if cell alive

    for (int i = 0; i < WIDTH;i++){
        for (int j = 0; j < HEIGHT;j++){
            if (Field[i][j] == true){
                r.x = i * 20;
                r.y = j * 20;
                colorRect(renderer, r, i,j,blue);
            } else {
                r.x = i * 20;
                r.y = j * 20;
                colorRect(renderer, r, i,j,white);
            }
        }
    }

    drawGrid(renderer,black);
    SDL_RenderPresent(renderer);
}



int countAliveNeighbors(int x, int y){
    int count = 0;

    for (int rows = -1; rows < 2; rows++){
        for (int columns = -1; columns < 2;columns++){
            if ((x+columns) >= 0 && (x+columns) < WIDTH && (y+rows) >= 0 && (y+rows) < HEIGHT){
                if (Field[x+columns][y+rows] == true){
                    count++;
                }
            }
        }
    }
    if (Field[x][y] == true){
        return count -1;
    }
    return count;
}

void copyArray(multi_arr arr1,multi_arr arr2){
    for (int i = 0; i < WIDTH;i++){
        for (int j = 0; j < HEIGHT;j++){
            arr1[i][j] = arr2[i][j];
        }
    }
}

void nextEpoch(){
    for (int i = 0; i < WIDTH;i++){
        for (int j = 0; j < HEIGHT;j++){
            if (Field[i][j] == true && (countAliveNeighbors(i, j) == 3 || countAliveNeighbors(i, j) == 2)){
                nextState[i][j] = true;               
            } else if (Field[i][j] == false && countAliveNeighbors(i, j) == 3){
                nextState[i][j] = true;
            } else {
                nextState[i][j] = false;
            }
        }
    }
    copyArray(Field, nextState);
}

int main (int argc, char** argv)
{
    SDL_Window* window = NULL;
    window = SDL_CreateWindow
    (
        "Game of Life", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );


    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    bool quit = false;
    
    int mouseX;
    int mouseY;

    SDL_SetRenderDrawColor(renderer, white.r,white.g,white.b ,white.a);

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = RECT_SIZE;
    r.h = RECT_SIZE;

    SDL_RenderClear(renderer);
    updateGame(window, renderer, r);

    while (!quit){
        while (SDL_PollEvent(&e)){ 
            if (e.type == SDL_QUIT){
                quit = true;
            }
            
            SDL_GetMouseState(&mouseX, &mouseY);
            mouseX = getNearestMultiple(mouseX) / RECT_SIZE;
            mouseY = getNearestMultiple(mouseY) / RECT_SIZE;

            if (e.type == SDL_MOUSEBUTTONDOWN && Field[mouseX][mouseY] == false){
                //activate cell at position x,y
                Field[mouseX][mouseY] = true;
                updateGame(window,renderer,r);
            }else if (e.type == SDL_MOUSEBUTTONDOWN && Field[mouseX][mouseY] == true){
                //Deactivate cell
                Field[mouseX][mouseY] = false;
                updateGame(window,renderer,r);
            }else if (e.type == SDL_KEYDOWN){
                if (e.key.keysym.sym == SDLK_SPACE){
                    nextEpoch();
                    updateGame(window, renderer, r);
                }
                else if (e.key.keysym.sym == SDLK_r){
                    memset(Field,0,sizeof(Field[0][0]) * WIDTH * HEIGHT);
                    memset(nextState,0,sizeof(nextState[0][0]) * WIDTH * HEIGHT);
                    updateGame(window,renderer, r);
                }
                else if (e.key.keysym.sym == SDLK_p){
                    SDL_Event run;
                    bool stop = false;
                    while (stop == false && SDL_PollEvent(&run)){
                        nextEpoch();
                        updateGame(window, renderer, r);
                        sleep(1);
                        if (run.type == SDL_KEYDOWN){
                            if (run.key.keysym.sym == SDLK_q){
                                printf("pressed P\n");
                                stop = true;
                            }
                        }
                    }
                }
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

