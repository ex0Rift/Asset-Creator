
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"

TTF_Init();

//initialising the font
TTF_Font* font = TTF_OpenFont("fonts/font.ttf",24);

//defining variables here
int mouse_x , mouse_y;

//this function just makes it easier to change the colour by letting me use my own from utils.h
void SetColor(SDL_Renderer* r, SDL_Color c){
    SDL_SetRenderDrawColor(r,c.r,c.g,c.b,c.a);
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);


    //initiate window
    SDL_Window* window = SDL_CreateWindow(
        "Asset Creator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREENWIDTH,SCREENHEIGHT,
        0
    );

    //create the render and start events
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,0);
    int running = 1;
    SDL_Event event;

    //crates elements
    SDL_Rect object = {100,100,100,100};
    SDL_Rect bottomBar = {0,SCREENHEIGHT-100,SCREENWIDTH,100};


    //main loop of window
    while (running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {running = 0;}
        }
        
        //gets mouse pos and updates them
        Uint32 buttons = SDL_GetMouseState(&mouse_x,&mouse_y);

        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
            object.x = mouse_x;
            object.y = mouse_y;
        }

        
        //start drawing area
        SetColor(renderer, GRAY);
        SDL_RenderClear(renderer);

        SetColor(renderer,BLACK);
        SDL_RenderFillRect(renderer,&object);

        //draw the bottom bar
        SetColor(renderer, DARKGRAY);
        SDL_RenderFillRect(renderer,&bottomBar);

        //updates the display
        SDL_RenderPresent(renderer);
    }



    // when ending program
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CLoseFont(font);
    SDL_Quit();
    return 0;
}