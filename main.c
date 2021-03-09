#include "./main.h"
#include <stdio.h>

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

int getLastMultiple(int number){
    int multiple = RECT_SIZE;
    float remainder;
    if ((remainder = (number % multiple)) == 0)
        return number;
    else{
        number -= remainder;
    }
    return number;
}

void colorRect(SDL_Renderer *renderer, SDL_Rect r,SDL_Color color){
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

void displayImg(SDL_Renderer *renderer,char* path){
    SDL_Surface *image = SDL_LoadBMP(path);
    if (image == NULL){
        printf("cant load image\n");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_Rect image_pos = { 6,SCREEN_HEIGHT-55, 50, 50 };
    SDL_Rect rect = {0,SCREEN_HEIGHT-59,60,60};

    colorRect(renderer, rect, white);
    SDL_RenderCopy(renderer, texture, NULL,&image_pos);
}

void updateGame(SDL_Window *window,SDL_Renderer *renderer, SDL_Rect r,bool paused){
    //Loop through field-array and show rectangles if cell alive
    char *path;
    
    if (paused){
        path = "./assets/start.bmp";
    } else{
        path = "./assets/pause.bmp";
    }

    for (int i = 0; i < WIDTH;i++){
        for (int j = 0; j < HEIGHT;j++){
            if (Field[i][j] == true){
                r.x = i * 20;
                r.y = j * 20;
                colorRect(renderer, r, blue);
            } else {
                r.x = i * 20;
                r.y = j * 20;
                colorRect(renderer, r, white);
            }
        }
    }

    drawGrid(renderer,black);
    displayImg(renderer, path);

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


bool buttonPress(int x,int y){
    if (x <= 2 && x >= 0 && y >= (HEIGHT - 3) && y <= HEIGHT){
        return true;
    }
    return false;
}

Uint32 my_callbackfunc( Uint32 interval, void *param ){
    SDL_Event e;

    e.user.type = SDL_USEREVENT;
    e.user.code = 0;
    e.user.data1 = NULL;
    e.user.data2 = NULL;

    SDL_PushEvent( &e );

    return interval;
}

int main (int argc, char** argv)
{
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

