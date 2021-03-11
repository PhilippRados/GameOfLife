#include "./main.h"
#include <stdio.h>

extern multi_arr Field;
extern multi_arr nextState;

extern SDL_Color blue;
extern SDL_Color black;
extern SDL_Color white;

int main (int argc, char** argv){
    SDL_Init(SDL_INIT_EVERYTHING);
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
    if (renderer == NULL){
        printf("Error creating renderer\n");
    }

    SDL_Event e;
    
    bool paused = true;
    int mouseX;
    int mouseY;

    SDL_SetRenderDrawColor(renderer, white.r,white.g,white.b ,white.a);

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = RECT_SIZE;
    r.h = RECT_SIZE;

    SDL_RenderClear(renderer);
    updateGame(window, renderer, r,paused);

    SDL_TimerID timer = SDL_AddTimer( 500, my_callbackfunc, NULL );
        if (timer == 0){
            return EXIT_FAILURE;
        }

    while (SDL_WaitEvent(&e)){ 
        switch(e.type){
            case SDL_QUIT:
                goto quit_game;
            
            case SDL_USEREVENT:
                if (!paused){
                    nextEpoch();
                    updateGame(window,renderer,r,paused);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseX = getLastMultiple(e.button.x) / RECT_SIZE;
                mouseY = getLastMultiple(e.button.y) / RECT_SIZE;
                
                if (buttonPress(mouseX,mouseY)){
                    paused = !paused;
                    updateGame(window,renderer,r,paused);
                }
                else if (Field[mouseX][mouseY] == false){
                    //activate cell at position x,y
                    Field[mouseX][mouseY] = true;
                    updateGame(window,renderer,r,paused);
                }else if (Field[mouseX][mouseY] == true){
                    //Deactivate cell
                    Field[mouseX][mouseY] = false;
                    updateGame(window,renderer,r,paused);
                break;

            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_SPACE){
                    nextEpoch();
                    updateGame(window, renderer, r,paused);
                }
                else if (e.key.keysym.sym == SDLK_r){
                    memset(Field,0,sizeof(Field[0][0]) * WIDTH * HEIGHT);
                    memset(nextState,0,sizeof(nextState[0][0]) * WIDTH * HEIGHT);
                    updateGame(window,renderer, r,paused);
                }
            }
        }
    }
quit_game:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

