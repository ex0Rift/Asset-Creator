
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"

//defining variables here
int mouse_x , mouse_y;
int click;

//this function just makes it easier to change the colour by letting me use my own from utils.h
void SetColor(SDL_Renderer* r, SDL_Color c){
    SDL_SetRenderDrawColor(r,c.r,c.g,c.b,c.a);
}

void MakeText(SDL_Renderer* r, TTF_Font* font, char text[],int dim[2]){
    //makes surface and texture
    SDL_Surface* surface = TTF_RenderText_Solid(font,text,BLACK);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(r,surface);
    //frees the surface
    SDL_FreeSurface(surface);
    //checks size of text
    int textWidth , textHeight;
    TTF_SizeText(font,text, &textWidth, &textHeight);
    //draws the text
    SDL_Rect dest = {dim[0],dim[1],textWidth,textHeight};
    SDL_RenderCopy(r,texture,NULL,&dest);
}

int Button(SDL_Rect r){
    //checks if mouse pos is in the button
    if (
        r.x < mouse_x && 
        mouse_x < r.x+r.w && 
        r.y < mouse_y && 
        mouse_y < r.y+r.h
    ){
        //checks if left click is pressed
        if (click == 1){return 1;}
     }
    return 0;
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //initialising the font 
    TTF_Font* font = TTF_OpenFont("fonts/upheavtt.ttf",24);

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

    //defining objects locations
    SDL_Rect bottomBar = {0,SCREENHEIGHT-100,SCREENWIDTH,100};
    int addCubeLabel[2] = {25,SCREENHEIGHT-76};
    SDL_Rect addCubeButton = {20,SCREENHEIGHT-80,115,30};


    //main loop of window
    while (running){
        click = 0;//set clikc to zero at the start of each loop
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {running = 0;}

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                click = 1;
            }
        }
        
        //gets mouse pos and updates them
        Uint32 buttons = SDL_GetMouseState(&mouse_x,&mouse_y);

        //
        //start drawing area
        //
        SetColor(renderer, GRAY);
        SDL_RenderClear(renderer);

        //draw the bottom bar
        SetColor(renderer, DARKGRAY);
        SDL_RenderFillRect(renderer,&bottomBar);

        //writing text
        SetColor(renderer,GRAY);
        SDL_RenderFillRect(renderer,&addCubeButton);
        MakeText(renderer,font,"Add cube",addCubeLabel);

        //checking for button presses

        //add cube button
        int cubeButton = Button(addCubeButton);
        if (cubeButton == 1){printf("WElll dne\n");}

        
        //updates the display
        SDL_RenderPresent(renderer);
    }



    // when ending program
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}