#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "tinyfiledialogs.h"

//defining variables here
int SCREENHEIGHT = 600;
int SCREENWIDTH = 800;

int canvasH = 400;
int canvasW = 400;

int oldCanvasH , oldCanvasW;
SDL_Rect endRect;
int newCanvasMode = 0;
SDL_Rect drawingRect;
int height_save , width_save;
SDL_Surface* surface;
int selection = 0;
int save_x, save_y;
int mouse_x , mouse_y , local_x , local_y;
int first_x , first_y , first_x_global , first_y_global;
int click;
int mode = 0;
int holding = 0;
int pixelSize = 10;
int brushSize = 10;
int currentNotificationTime = 0;
char notificationText[] = "Notification";
char str[10];
SDL_Color drawColour = {0,0,0,255};
SDL_Color **savelist = NULL;

Uint32 frameStart;
int frameTime;

void SetColor(SDL_Renderer* r, SDL_Color c);
void MakeText(SDL_Renderer* r, TTF_Font* font, char text[],int dim[2]);
int Button(SDL_Rect r);
void DrawColourPallet(SDL_Renderer* r);
SDL_Texture* MakeCanvas(SDL_Renderer* renderer);
SDL_Color GetColour(SDL_Surface* surface,int x,int y);


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
        DEFAULTSCREENWIDTH,DEFAULTSCREENHEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    SDL_SetWindowMinimumSize(window,DEFAULTSCREENWIDTH,DEFAULTSCREENHEIGHT);

    //create the render and start events
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,0);
    int running = 1;
    SDL_Event event;

    //make the texture for drawing on
    SDL_Texture* baseLayer = MakeCanvas(renderer);
    
    //draw background
    SetColor(renderer, GRAY);
    SDL_RenderClear(renderer);

    //Make a surface to be read from
    surface = SDL_CreateRGBSurfaceWithFormat(0, SCREENHEIGHT, SCREENWIDTH, 32, SDL_PIXELFORMAT_RGBA32);



    //these objects should not be fuly updateed per frame.
    SDL_Rect redSlider = {500,SCREENHEIGHT-92,8,24};
    SDL_Rect greenSlider = {500,SCREENHEIGHT-62,8,24};
    SDL_Rect blueSlider = {500,SCREENHEIGHT-32,8,24};

    //main loop of window
    while (running){
        int oldsw = SCREENWIDTH;
        int oldsh = SCREENHEIGHT;
        SDL_GetWindowSize(window,&SCREENWIDTH,&SCREENHEIGHT);

        //uodate the surface
        SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_RGBA32,surface->pixels,surface->pitch);

        //define object positions per frame for window resizing

        //defining objects locations
        SDL_Rect bottomBar = {0,SCREENHEIGHT-100,SCREENWIDTH,100};

        int addCubeLabel[2] = {30,SCREENHEIGHT-76};
        SDL_Rect addCubeButton = {20,SCREENHEIGHT-80,80,30};

        int freeDrawLabel[2] = {30,SCREENHEIGHT-36};
        SDL_Rect freeDrawButton = {20,SCREENHEIGHT-40,80,30};

        SDL_Rect drawBrushPlus = {290,SCREENHEIGHT-40,30,30};
        int brushPlusLabel[2] = {298,SCREENHEIGHT-38};
        SDL_Rect drawBrushMinus = {220,SCREENHEIGHT-40,30,30};
        int brushMinusLabel[2] = {228,SCREENHEIGHT-38};
        int sizeLabel[2] = {260,SCREENHEIGHT-38};
        
        SDL_Rect fillButton = {220,SCREENHEIGHT-40,80,30};
        int fillButtonLabel[2] = {228,SCREENHEIGHT-38};

        SDL_Rect selectButton = {110,SCREENHEIGHT-80,100,30};
        int selectButtonLabel[2] = {118,SCREENHEIGHT-78};

        SDL_Rect eraseButton = {110,SCREENHEIGHT-40,100,30};
        int eraseButtonLabel[2] = {118,SCREENHEIGHT-38};

        //objects for menuBar
        SDL_Rect menuBar = {0,0,SCREENWIDTH,30};
        SDL_Rect saveButton = {0,0,100,30};
        int saveButtonLabel[2] = {20,2};
        SDL_Rect newButton = {102,0,100,30};
        int newButtonLabel[2] = {120,2};
        //Notification drawing
        SDL_Rect notificationBox = {SCREENWIDTH-200,SCREENHEIGHT-150,180,40};
        int notificationLabel[2] = {SCREENWIDTH-190,SCREENHEIGHT-142};

        //inputs definitions

        SDL_Rect previewColour = {SCREENWIDTH-380,SCREENHEIGHT-90,50,80};
        SDL_Rect redInput = {SCREENWIDTH-300,SCREENHEIGHT-90,256,20};
        SDL_Rect greenInput = {SCREENWIDTH-300,SCREENHEIGHT-60,256,20};
        SDL_Rect blueInput = {SCREENWIDTH-300,SCREENHEIGHT-30,256,20};

        
        redSlider.y = SCREENHEIGHT-92;
        greenSlider.y = SCREENHEIGHT-62;
        blueSlider.y = SCREENHEIGHT-32;

        if (oldsw != SCREENWIDTH){
            int dif = SCREENWIDTH-oldsw;
            redSlider.x +=dif;
            greenSlider.x +=dif;
            blueSlider.x+=dif;
        }

        //new canvas ui
        int newCanvasLabel[2]= {15,45};
        int plusHLabel[2]= {127,100};
        int minusHLabel[2]= {27,100};
        int plusWLabel[2]= {127,150};
        int minusWLabel[2]= {27,150};

        int hLabel[2] = {60,100};
        int wLabel[2] = {60,150};

        SDL_Rect newCanvasBackground = {10,40,300,250};
        SDL_Rect newCanvasPlusHeight = {120,100,30,30};
        SDL_Rect newCanvasMinusHeight = {20,100,30,30};
        SDL_Rect newCanvasPlusWidth = {120,150,30,30};
        SDL_Rect newCanvasMinusWidth = {20,150,30,30};

        SDL_Rect newCanvasButton = {200,250,100,30};
        int newCanvasButtonLabel[2] = {207,250};


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
        //draw background
        SetColor(renderer, GRAY);
        SDL_RenderClear(renderer);
        
        //set up the canvas before the UI
        SDL_Rect baseLayerDest = {(SCREENWIDTH/2)-(canvasW/2),((SCREENHEIGHT-100)/2)-(canvasH/2),canvasW,canvasH};
        SDL_RenderCopy(renderer,baseLayer,NULL,&baseLayerDest);
        //mouse coords local to the canvas
        local_x = ((mouse_x - baseLayerDest.x) / pixelSize) * pixelSize;
        local_y = ((mouse_y - baseLayerDest.y) / pixelSize) * pixelSize;

        //
        //Menu bar
        //
        SetColor(renderer,DARKERGRAY);
        SDL_RenderFillRect(renderer,&menuBar);
        SetColor(renderer,GRAY);
        SDL_RenderFillRect(renderer,&saveButton);
        MakeText(renderer,font,"Save",saveButtonLabel);

        SDL_RenderFillRect(renderer,&newButton);
        MakeText(renderer,font,"New",newButtonLabel);

        //Notification
        if (currentNotificationTime > 0){
            SetColor(renderer,MIDGRAY);
            SDL_RenderFillRect(renderer,&notificationBox);
            SetColor(renderer,BLACK);
            SDL_RenderDrawRect(renderer,&notificationBox);
            MakeText(renderer,font,notificationText,notificationLabel);

            currentNotificationTime--;
        }

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

        //erase button
        SetColor(renderer,GRAY);
        if (mode == 6){SetColor(renderer,MIDGRAY);}
        SDL_RenderFillRect(renderer,&eraseButton);
        MakeText(renderer,font,"Erase",eraseButtonLabel);

        //Fill button
        SetColor(renderer,GRAY);
        if (mode == 5){SetColor(renderer,MIDGRAY);}
        SDL_RenderFillRect(renderer,&selectButton);
        MakeText(renderer,font,"Select",selectButtonLabel);

        //Fill button
        //SetColor(renderer,GRAY);
        //if (mode == 3){SetColor(renderer,MIDGRAY);}
        //SDL_RenderFillRect(renderer,&fillButton);
        //MakeText(renderer,font,"Fill",fillButtonLabel);

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
        SetColor(renderer,BLACK);
        if(mode == 4){SDL_RenderDrawRect(renderer,&previewColour);}

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

        //
        //NEW CANVAS UI
        //
        if (newCanvasMode){
            //Prevent drawing while in UI
            mode = 0;
            SetColor(renderer,MIDGRAY);
            SDL_RenderFillRect(renderer,&newCanvasBackground);
            SetColor(renderer,BLACK);
            SDL_RenderDrawRect(renderer,&newCanvasBackground);

            MakeText(renderer,font,"New Canvas",newCanvasLabel);

            SetColor(renderer,GRAY);
            SDL_RenderFillRect(renderer,&newCanvasMinusWidth);
            MakeText(renderer,font,"-",minusWLabel);
            SDL_RenderFillRect(renderer,&newCanvasPlusWidth);
            MakeText(renderer,font,"+",plusWLabel);
            snprintf(str,sizeof(str),"%d",(canvasW));
            MakeText(renderer,font,str,wLabel);
            SDL_RenderFillRect(renderer,&newCanvasMinusHeight);
            MakeText(renderer,font,"-",minusHLabel);
            SDL_RenderFillRect(renderer,&newCanvasPlusHeight);
            MakeText(renderer,font,"+",plusHLabel);
            snprintf(str,sizeof(str),"%d",(canvasH));
            MakeText(renderer,font,str,hLabel);

            SDL_RenderFillRect(renderer,&newCanvasButton);
            MakeText(renderer,font,"Create",newCanvasButtonLabel);
        }
        
        //checking for button presses

        //New canvas buttons
        int newCanvasMinusWidth_btn = Button(newCanvasMinusWidth);
        if (newCanvasMinusWidth_btn){canvasW -= 10;}

        int newCanvasMinusHeight_btn = Button(newCanvasMinusHeight);
        if (newCanvasMinusHeight_btn){canvasH -= 10;}

        int newCanvasPlusWidth_btn = Button(newCanvasPlusWidth);
        if (newCanvasPlusWidth_btn){canvasW += 10;}

        int newCanvasPlusHeight_btn = Button(newCanvasPlusHeight);
        if (newCanvasPlusHeight_btn){canvasH += 10;}

        int newCanvasButton_btn = Button(newCanvasButton);
        if (newCanvasButton_btn){
            newCanvasMode = 0;
            SDL_DestroyTexture(baseLayer);
            SDL_Texture* baseLayer = MakeCanvas(renderer);
        }



        int redInput_btn = Button(redInput);
        if (redInput_btn){
            drawColour.r = mouse_x - redInput.x;
            drawColour.g = greenSlider.x -greenInput.x;
            drawColour.b = blueSlider.x - blueInput.x;
            redSlider.x = mouse_x;
        }

        int greenInput_btn = Button(greenInput);
        if (greenInput_btn){
            drawColour.g = mouse_x - greenInput.x;
            drawColour.b = blueSlider.x - blueInput.x;
            drawColour.r = redSlider.x - redInput.x;
            greenSlider.x = mouse_x;
        }

        int blueInput_btn = Button(blueInput);
        if (blueInput_btn){
            drawColour.b = mouse_x - blueInput.x;
            drawColour.g = greenSlider.x -greenInput.x;
            drawColour.r = redSlider.x - redInput.x;
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

        //int fillButton_btn = Button(fillButton);
        //if (fillButton_btn){mode = 3;}

        int selectButton_btn = Button(selectButton);
        if (selectButton_btn){mode = 5;}

        int previewColour_btn = Button(previewColour);
        if (previewColour_btn){mode = 4;}

        int eraseButton_btn = Button(eraseButton);
        if (eraseButton_btn){mode = 6;}


        //button presses for menu bar

        int saveButton_btn = Button(saveButton);
        if (saveButton_btn){
            currentNotificationTime = NOTIFICATIONTIME;
            strcpy(notificationText,"Saved!");
            SDL_Surface* saveSurface = SDL_CreateRGBSurfaceWithFormat(0,baseLayerDest.w,baseLayerDest.h,32,SDL_PIXELFORMAT_RGBA32);
            SDL_Rect area = {baseLayerDest.x,baseLayerDest.y,baseLayerDest.w,baseLayerDest.h};
            SDL_RenderReadPixels(renderer,&area,SDL_PIXELFORMAT_RGBA32,saveSurface->pixels, saveSurface->pitch);

            //Opens up tinyfiledialogs
            const char* filters[] = {".png"};
            const char* path = tinyfd_saveFileDialog("Save Image", "drawing.png", 1, filters, "Choose image file");

            if (path){
                if (IMG_SavePNG(saveSurface, path) != 0){
                    printf("Failed\n");
                }
            }

            SDL_FreeSurface(saveSurface);
        }

        int newButton_btn = Button(newButton);
        if (newButton_btn){
            if (newCanvasMode == 0){
                oldCanvasW = canvasW;
                oldCanvasH = canvasH;
                newCanvasMode = 1;
            }else{
                canvasH = oldCanvasH;
                canvasW = oldCanvasW;
                newCanvasMode = 0;
            }
        }

        //making objects logic
        if (mode == 1) {
            if (click == 1) {
                first_x = local_x;
                first_y = local_y;
                first_x_global = (mouse_x/pixelSize)*pixelSize;
                first_y_global = (mouse_y/pixelSize)*pixelSize;
            }
            if (holding == 1){
                SetColor(renderer,BLACK);
                int new_x = (mouse_x/pixelSize)*pixelSize;
                int new_y = (mouse_y/pixelSize)*pixelSize;
                SDL_Rect drawingRect = {first_x_global,first_y_global,new_x-first_x_global,new_y-first_y_global};
                SDL_RenderDrawRect(renderer,&drawingRect);
            }
            if (holding == 2){
                int new_x = local_x;
                int new_y = local_y;
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
                SDL_SetRenderTarget(renderer,baseLayer);
                SetColor(renderer,drawColour);
                SDL_Rect tmp = {local_x,local_y,brushSize,brushSize};
                SDL_RenderFillRect(renderer,&tmp);
                SDL_SetRenderTarget(renderer,NULL);
            }
        }

        if (mode == 3){
            if (click == 1){
                //turn mouse coords into pixel sizes
                int pixel_x = (mouse_x/pixelSize)*pixelSize;
                int pixel_y = (mouse_y/pixelSize)*pixelSize;
                int local_pixel_x = local_x;
                int local_pixel_y = local_y;

                //LINEBANK
                //currentColour = GetColour(surface,pixel_x,pixel_y); grabbing current colour
                //SDL_SetRenderTarget(renderer,baseLayer);//sets Target to canvas


                SDL_Color startColour = GetColour(surface, pixel_x,pixel_y);
                SetColor(renderer,drawColour);
                SDL_Color currentColour = startColour;
                int i = 0;
                int running = 1;
                while (running){
                    currentColour = GetColour(surface,pixel_x,pixel_y+i); 
                    if (startColour.r == currentColour.r && startColour.g == currentColour.g && startColour.b == currentColour.b){
                        SDL_SetRenderTarget(renderer,baseLayer);//sets Target to canvas


                        SDL_Rect tmp = {local_pixel_x,local_pixel_y+i,pixelSize,pixelSize};
                        SDL_RenderFillRect(renderer,&tmp);


                        SDL_SetRenderTarget(renderer,NULL);//resets Target
                        i+=pixelSize;
                    }else {running = 0;}
                }
            }
        }

        if (mode == 4){
            SDL_Color output = GetColour(surface,local_x,local_y);
            output.a = 255;
            drawColour = output;
        }

        if (mode == 5){
            int new_x , new_y;
            int save_x , save_y;
            if (selection == 0){
                if (click == 1) {
                    first_x = local_x;
                    first_y = local_y;
                    first_x_global = (mouse_x/pixelSize)*pixelSize;
                    first_y_global = (mouse_y/pixelSize)*pixelSize;
                }
                if (holding == 1){
                    SetColor(renderer,BLACK);
                    int new_x = (mouse_x/pixelSize)*pixelSize;
                    int new_y = (mouse_y/pixelSize)*pixelSize;
                    save_x = new_x;
                    save_y = new_y;
                    drawingRect = (SDL_Rect){first_x_global,first_y_global,save_x-first_x_global,save_y-first_y_global};
                    endRect = (SDL_Rect){first_x,first_y};
                    SDL_RenderDrawRect(renderer,&drawingRect);
                }
                if (holding == 2){
                    //uodate the surface
                    SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_RGBA32,surface->pixels,surface->pitch);

                    int new_x = local_x;
                    int new_y = local_y;
                    endRect = (SDL_Rect){first_x,first_y,new_x-first_x,new_y-first_y};
    
                    height_save = (new_y-first_y) / pixelSize;
                    width_save = (new_x-first_x) / pixelSize;

                    savelist = malloc(height_save * sizeof(SDL_Color*));
                    for (int i = 0; i < height_save; i++){
                        savelist[i] = malloc(width_save * sizeof(SDL_Color));
                    }

                    for (int i = 0; i < height_save; i++){
                        for (int j = 0; j < width_save; j++){
                            SDL_Color tmpc = GetColour(surface,first_x_global+j *pixelSize,first_y_global+i*pixelSize);
                            //printf("%d %d %d\n",tmpc.r,tmpc.g,tmpc.b);
                            savelist[i][j] = (SDL_Color){tmpc.r,tmpc.g,tmpc.b,255};
                        }
                    }
                    
                    holding = 0;
                    selection = 1;
                }
                
            }
            if (selection == 1){
                if (holding != 2){
                    SetColor(renderer,BLACK);
                    SDL_RenderDrawRect(renderer,&drawingRect);
                }
                if (click == 1){
                    first_x = local_x;
                    first_y = local_y;
                    first_x_global = (mouse_x/pixelSize)*pixelSize;
                    first_y_global = (mouse_y/pixelSize)*pixelSize;
                }
                if (holding == 1){
                    int new_x = (mouse_x/pixelSize)*pixelSize;
                    int new_y = (mouse_y/pixelSize)*pixelSize;
                    save_x = local_x;
                    save_y = local_y;
                    drawingRect.x = new_x;
                    drawingRect.y = new_y;
                }

                if (holding == 2){
                    SDL_SetRenderTarget(renderer,baseLayer);
                    //overwrites the old area
                    SetColor(renderer,WHITE);
                    SDL_RenderFillRect(renderer,&endRect);
                    for (int i = 0; i < height_save; i++){
                        for (int j = 0; j < width_save; j++){
                            //makes the new rectangle
                            SetColor(renderer,savelist[i][j]);
                            SDL_Rect tmpr = {save_x+j*pixelSize,save_y+i*pixelSize,pixelSize,pixelSize};
                            SDL_RenderFillRect(renderer,&tmpr);
                        }
                    }
                    SDL_SetRenderTarget(renderer,NULL);
                    selection = 0;
                    holding = 0;
                }
            }
        }

        if (mode == 6){
            if (holding == 1){
                SDL_SetRenderTarget(renderer,baseLayer);
                SetColor(renderer,WHITE);
                SDL_Rect tmp = {local_x,local_y,brushSize,brushSize};
                SDL_RenderFillRect(renderer,&tmp);
                SDL_SetRenderTarget(renderer,NULL);
            }
        }
        
        
        //updates the display
        SDL_RenderPresent(renderer);
        
        //FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAMEDELAY > frameTime){SDL_Delay(FRAMEDELAY - frameTime);}
    }



    // when ending program
    free(savelist);
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

SDL_Texture* MakeCanvas(SDL_Renderer* renderer){
    //create the base texture for the actual drawing
    SDL_Texture* baseLayer = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,canvasW,canvasH);
    SDL_SetTextureBlendMode(baseLayer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer,baseLayer);
    SetColor(renderer,WHITE);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer,NULL);
    return baseLayer;
}

SDL_Color GetColour(SDL_Surface* surface,int x, int y){
    Uint32 pixel;
    SDL_Color c;
    Uint32* pixels = (Uint32*)surface->pixels;
    pixel = pixels[y * surface->w + x];

    SDL_GetRGB(pixel, surface->format, &c.r,&c.g,&c.b);

    return c;
}