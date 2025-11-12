#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

//defining variables here
int mouse_x , mouse_y;
int first_x , first_y;
int click;
int mode = 0;
int holding = 0;
int pixelSize = 10;
int brushSize = 10;
char str[10];
SDL_Color drawColour = {0,0,0,255};

Uint32 frameStart;
int frameTime;

void SetColor(SDL_Renderer* r, SDL_Color c);
void MakeText(SDL_Renderer* r, TTF_Font* font, char text[],int dim[2]);
int Button(SDL_Rect r);
void DrawColourPallet(SDL_Renderer* r);



int main(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
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

    //create the base texture for the actual drawing
    SDL_Texture* baseLayer = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREENWIDTH,SCREENHEIGHT);
    SDL_SetTextureBlendMode(baseLayer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer,baseLayer);
    SetColor(renderer,WHITE);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer,NULL);

    //defining objects locations
    SDL_Rect bottomBar = {0,SCREENHEIGHT-100,SCREENWIDTH,100};

    int addCubeLabel[2] = {30,SCREENHEIGHT-76};
    SDL_Rect addCubeButton = {20,SCREENHEIGHT-80,80,30};

    int freeDrawLabel[2] = {30,SCREENHEIGHT-36};
    SDL_Rect freeDrawButton = {20,SCREENHEIGHT-40,80,30};
    SDL_Rect drawBrushPlus = {180,SCREENHEIGHT-40,30,30};
    int brushPlusLabel[2] = {188,SCREENHEIGHT-38};
    SDL_Rect drawBrushMinus = {110,SCREENHEIGHT-40,30,30};
    int brushMinusLabel[2] = {118,SCREENHEIGHT-38};
    int sizeLabel[2] = {150,SCREENHEIGHT-38};

    SDL_Rect redSlider = {500,SCREENHEIGHT-92,8,24};
    SDL_Rect greenSlider = {500,SCREENHEIGHT-62,8,24};
    SDL_Rect blueSlider = {500,SCREENHEIGHT-32,8,24};

    //objects for menuBar
    SDL_Rect menuBar = {0,0,SCREENWIDTH,30};
    SDL_Rect saveButton = {0,0,100,30};
    int saveButtonLabel[2] = {20,2};

    //inputs definitions

    SDL_Rect previewColour = {430,SCREENHEIGHT-90,50,80};

    SDL_Rect redInput = {500,SCREENHEIGHT-90,256,20};
    SDL_Rect greenInput = {500,SCREENHEIGHT-60,256,20};
    SDL_Rect blueInput = {500,SCREENHEIGHT-30,256,20};

    //draw background
    SetColor(renderer, GRAY);
    SDL_RenderClear(renderer);

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
        
        //draw the texture layer first
        SDL_RenderCopy(renderer,baseLayer,NULL,NULL);

        //
        //Menu bar
        //
        SetColor(renderer,DARKERGRAY);
        SDL_RenderFillRect(renderer,&menuBar);
        SetColor(renderer,GRAY);
        SDL_RenderFillRect(renderer,&saveButton);
        MakeText(renderer,font,"Save",saveButtonLabel);

        //
        //bottom bar
        //
        SetColor(renderer, DARKGRAY);
        SDL_RenderFillRect(renderer,&bottomBar);

        //cube buttton
        SetColor(renderer,GRAY);
        if (mode==1){SetColor(renderer,MIDGRAY);}
        SDL_RenderFillRect(renderer,&addCubeButton);
        MakeText(renderer,font,"Cube",addCubeLabel);

        //draw button
        SetColor(renderer,GRAY);
        if (mode == 2){SetColor(renderer,MIDGRAY);}
        SDL_RenderFillRect(renderer,&freeDrawButton);
        MakeText(renderer,font,"Draw",freeDrawLabel);

        //draw bush buttons
        SetColor(renderer,GRAY);
        SDL_RenderFillRect(renderer,&drawBrushPlus);
        SDL_RenderFillRect(renderer,&drawBrushMinus);
        MakeText(renderer,font,"+",brushPlusLabel);
        MakeText(renderer,font,"-",brushMinusLabel);
        snprintf(str,sizeof(str),"%d",(brushSize/pixelSize));
        MakeText(renderer,font,str,sizeLabel);

        //Colour preview
        SetColor(renderer,drawColour);
        SDL_RenderFillRect(renderer,&previewColour);

        for (int r = 0; r < 256; r+=8){
            SDL_Color tmpC = {r,0,0,255};
            SDL_Rect tmpR = {redInput.x+r,redInput.y,8,redInput.h};
            SetColor(renderer,tmpC);
            SDL_RenderFillRect(renderer,&tmpR);
        }
        SetColor(renderer,WHITE);
        SDL_RenderFillRect(renderer,&redSlider);

        for (int r = 0; r < 256; r+=8){
            SDL_Color tmpC = {0,r,0,255};
            SDL_Rect tmpR = {greenInput.x+r,greenInput.y,8,greenInput.h};
            SetColor(renderer,tmpC);
            SDL_RenderFillRect(renderer,&tmpR);
        }
        SetColor(renderer,WHITE);
        SDL_RenderFillRect(renderer,&greenSlider);

        for (int r = 0; r < 256; r+=8){
            SDL_Color tmpC = {0,0,r,255};
            SDL_Rect tmpR = {blueInput.x+r,blueInput.y,8,blueInput.h};
            SetColor(renderer,tmpC);
            SDL_RenderFillRect(renderer,&tmpR);
        }
        SetColor(renderer,WHITE);
        SDL_RenderFillRect(renderer,&blueSlider);
        
        //checking for button presses

        int redInput_btn = Button(redInput);
        if (redInput_btn){
            drawColour.r = mouse_x - redInput.x;
            redSlider.x = mouse_x;
        }

        int greenInput_btn = Button(greenInput);
        if (greenInput_btn){
            drawColour.g = mouse_x - greenInput.x;
            greenSlider.x = mouse_x;
        }

        int blueInput_btn = Button(blueInput);
        if (blueInput_btn){
            drawColour.b = mouse_x - blueInput.x;
            blueSlider.x = mouse_x;
        }

        int cubeButton_btn = Button(addCubeButton);
        if (cubeButton_btn){mode = 1;}

        int drawButton_btn = Button(freeDrawButton);
        if (drawButton_btn){mode = 2;}

        int brushPlus_btn = Button(drawBrushPlus);
        if (brushPlus_btn){brushSize+=pixelSize;}

        int brushMinus_btn = Button(drawBrushMinus);
        if (brushMinus_btn){if(brushSize > 0){brushSize-=pixelSize;}}

        //button presses for menu bar

        int saveButton_btn = Button(saveButton);
        if (saveButton_btn){
            SDL_Surface* saveSurface = SDL_CreateRGBSurfaceWithFormat(0,SCREENWIDTH,SCREENHEIGHT,32,SDL_PIXELFORMAT_RGBA32);
            SDL_Rect area = {0,30,SCREENWIDTH,SCREENHEIGHT-130};
            SDL_RenderReadPixels(renderer,&area,SDL_PIXELFORMAT_RGBA32,saveSurface->pixels, saveSurface->pitch);

            IMG_SavePNG(saveSurface,"output.png");
            SDL_FreeSurface(saveSurface);
        }


        //making objects logic
        if (mouse_y < (SCREENHEIGHT-100)){
            if (mode == 1) {
                if (click == 1) {
                    first_x = (mouse_x/pixelSize)*pixelSize;
                    first_y = (mouse_y/pixelSize)*pixelSize;
                }
                if (holding == 1){
                    SetColor(renderer,BLACK);
                    int new_x = (mouse_x/pixelSize)*pixelSize;
                    int new_y = (mouse_y/pixelSize)*pixelSize;
                    SDL_Rect drawingRect = {first_x,first_y,new_x-first_x,new_y-first_y};
                    SDL_RenderDrawRect(renderer,&drawingRect);
                }
                if (holding == 2){
                    int new_x = (mouse_x/pixelSize)*pixelSize;
                    int new_y = (mouse_y/pixelSize)*pixelSize;
                    SDL_Rect currentRect = {first_x,first_y,new_x-first_x,new_y-first_y};
                    //draw to the texture instead of base layer for improved performance
                    SDL_SetRenderTarget(renderer,baseLayer);
                    SetColor(renderer,drawColour);
                    SDL_RenderFillRect(renderer,&currentRect);
                    SDL_SetRenderTarget(renderer,NULL);
                    holding = 0;
                }
            }
            if (mode == 2){
                if (holding == 1){
                    int x = (mouse_x/pixelSize)*pixelSize;
                    int y = (mouse_y/pixelSize)*pixelSize;
                    SDL_SetRenderTarget(renderer,baseLayer);
                    SetColor(renderer,drawColour);
                    SDL_Rect tmp = {x,y,brushSize,brushSize};
                    SDL_RenderFillRect(renderer,&tmp);
                    SDL_SetRenderTarget(renderer,NULL);
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