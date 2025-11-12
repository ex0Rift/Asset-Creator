#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"

//defining variables here
int mouse_x , mouse_y;
int first_x , first_y;
int click;
int mode = 0;
int holding = 0;
SDL_Color drawColour;

Uint32 frameStart;
int frameTime;

int cubesN = 0;
SDL_Rect* cubes;
SDL_Color* cubeColours;


void SetColor(SDL_Renderer* r, SDL_Color c);
void MakeText(SDL_Renderer* r, TTF_Font* font, char text[],int dim[2]);
int Button(SDL_Rect r);
void DrawColourPallet(SDL_Renderer* r);



int main(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //initialise the cube array
    cubes = malloc(cubesN * sizeof(SDL_Rect));
    cubeColours = malloc(cubesN * sizeof(SDL_Color));

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
    int addCubeLabel[2] = {30,SCREENHEIGHT-76};
    SDL_Rect addCubeButton = {20,SCREENHEIGHT-80,80,30};

    //inputs definitions
    SDL_Rect redInput = {500,SCREENHEIGHT-90,256,20};
    SDL_Rect greenInput = {500,SCREENHEIGHT-60,256,20};
    SDL_Rect blueInput = {500,SCREENHEIGHT-30,256,20};

    //main loop of window
    while (running){
        frameStart = SDL_GetTicks();
        click = 0;//set clikc to zero at the start of each loop
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {running = 0;}

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                click = 1;
                if (mouse_y < SCREENHEIGHT-100){holding = 1;}
            }
            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT){
                if (mouse_y <SCREENHEIGHT-100){holding = 2;}
            }
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    mode = 0;
                }
            }
        }
        
        //gets mouse pos and updates them
        Uint32 buttons = SDL_GetMouseState(&mouse_x,&mouse_y);

        //
        //start drawing area
        //
        SetColor(renderer, GRAY);
        SDL_RenderClear(renderer);

        //draw cubes to screen
        for (int i =0; i < cubesN; i++){
            SetColor(renderer,cubeColours[i]);
            SDL_RenderFillRect(renderer,&cubes[i]);
        }

        //draw the bottom bar
        SetColor(renderer, DARKGRAY);
        SDL_RenderFillRect(renderer,&bottomBar);

        //
        //UI on bottom bar
        //

        //cube buttton
        SetColor(renderer,GRAY);
        if (mode==1){SetColor(renderer,MIDGRAY);}
        SDL_RenderFillRect(renderer,&addCubeButton);
        MakeText(renderer,font,"Cube",addCubeLabel);

        for (int r = 0; r < 256; r+=8){
            SDL_Color tmpC = {r,0,0,255};
            SDL_Rect tmpR = {redInput.x+r,redInput.y,8,redInput.h};
            SetColor(renderer,tmpC);
            SDL_RenderFillRect(renderer,&tmpR);
        }

        for (int r = 0; r < 256; r+=8){
            SDL_Color tmpC = {0,r,0,255};
            SDL_Rect tmpR = {greenInput.x+r,greenInput.y,8,greenInput.h};
            SetColor(renderer,tmpC);
            SDL_RenderFillRect(renderer,&tmpR);
        }

        for (int r = 0; r < 256; r+=8){
            SDL_Color tmpC = {0,0,r,255};
            SDL_Rect tmpR = {blueInput.x+r,blueInput.y,8,blueInput.h};
            SetColor(renderer,tmpC);
            SDL_RenderFillRect(renderer,&tmpR);
        }
        
        //checking for button presses

        int redInput_btn = Button(redInput);
        if (redInput_btn){drawColour.r = mouse_x - redInput.x;}

        int greenInput_btn = Button(greenInput);
        if (greenInput_btn){drawColour.g = mouse_x - greenInput.x;}

        int blueInput_btn = Button(blueInput);
        if (blueInput_btn){drawColour.b = mouse_x - blueInput.x;}

        int cubeButton_btn = Button(addCubeButton);
        if (cubeButton_btn){mode = 1;}


        //making objects logic
        if (mouse_y < (SCREENHEIGHT-100)){
            if (mode == 1) {
                if (click == 1) {
                    first_x = mouse_x;
                    first_y = mouse_y;
                }
                if (holding == 1){
                    SetColor(renderer,BLACK);
                    SDL_Rect drawingRect = {first_x,first_y,mouse_x-first_x,mouse_y-first_y};
                    SDL_RenderDrawRect(renderer,&drawingRect);
                }
                if (holding == 2){
                    cubesN += 1;
                    cubes = realloc(cubes, cubesN * sizeof(SDL_Rect));
                    cubeColours = realloc(cubeColours,cubesN * sizeof(SDL_Color));
                    cubes[cubesN-1] = (SDL_Rect) {first_x,first_y,mouse_x-first_x,mouse_y-first_y};
                    cubeColours[cubesN-1] = drawColour;
                    holding = 0;
                }
            }
        }

        
        //updates the display
        SDL_RenderPresent(renderer);

        //FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAMEDELAY > frameTime){SDL_Delay(FRAMEDELAY - frameTime);}
    }



    // when ending program
    free(cubes);
    free(cubeColours);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    
    return 0;
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

void SetColor(SDL_Renderer* r, SDL_Color c){
    SDL_SetRenderDrawColor(r,c.r,c.g,c.b,c.a);
}

void DrawColourPallet(SDL_Renderer* renderer){
    for (int r = 0; r < 256; r+=10){
        for (int g =0; g < 256; g+=10){
            for (int b = 0; b < 256; b+=10){
                SDL_Color tcolour = {g,r,b,255};
                SDL_Rect tcube = {r,g,10,10};
                SetColor(renderer,tcolour);
                SDL_RenderFillRect(renderer,&tcube);
            }
        }
    }
}  