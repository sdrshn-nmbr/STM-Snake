#ifndef __SNAKE_H
#define __SNAKE_H
#include "stdlib.h"
// Define the size of the display
#define DISPLAY_WIDTH  24
#define DISPLAY_HEIGHT 32 //NEED TO CHANGE


struct Snake; // Forward declaration of struct Snake

typedef struct Snake
{
    int x, y;
    struct Snake * next;
} Snake;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

Snake * moveSnake(Snake * snake, Direction direction, int add);
Snake * initSnake(int x, int y);

#endif