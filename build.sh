#!/bin/bash
echo "Compiling..."
gcc main.c tinyfiledialogs.c -o a.out `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image utils.h
echo "Running..."
./a.out
echo "Finished..."