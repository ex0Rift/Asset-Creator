#ifndef UTILS_H
#define UTILS_H
#include <SDL2/SDL.h>

const int DEFAULTSCREENHEIGHT = 600;
const int DEFAULTSCREENWIDTH = 800;

const int FPS = 120;
const int FRAMEDELAY = 1000 / FPS;

const int NOTIFICATIONTIME = 240;

const SDL_Color RED = {255,0,0,255};
const SDL_Color GREEN = {0,255,0,255};
const SDL_Color BLUE = {0,0,255,255};
const SDL_Color BLACK ={0,0,0,255};
const SDL_Color WHITE = {255,255,255,255};
const SDL_Color GRAY = {180,180,180,255};
const SDL_Color DARKGRAY = {120,120,120,255};
const SDL_Color DARKERGRAY = {100,100,100,255};
const SDL_Color MIDGRAY = {140,140,140,255};

#endif