#include "snake.h"
#include <stdint.h>

Snake * initSnake(int x, int y) {
    Snake * snake = malloc(sizeof(Snake));
    snake->x = x;
    snake->y = y;
    Snake * snake1 = malloc(sizeof(Snake));
    snake->next = snake1;
    snake1->x = x+1;
    snake1->y = y;
    Snake * end = malloc(sizeof(Snake));
    snake1->next = end;
    end->x = x+2;
    end->y = y;
    end->next = NULL;

    return snake;
}
// if add is 1 it will lengthen the snake while moving it
Snake * moveSnake(Snake * snake, Direction direction, int add) {
    Snake * newHead = malloc(sizeof(Snake));
    newHead->x = snake->x;
    newHead->y = snake->y;
    switch (direction) {
        case DOWN:
            newHead->y--;
            if(newHead->y<0) newHead->y = DISPLAY_HEIGHT-1; 
            break;
        case UP:
            newHead->y++;
            if(newHead->y>DISPLAY_HEIGHT-1) newHead->y = 0;
            break;
        case RIGHT:
            newHead->x--;
            if(newHead->x<0) newHead->x = DISPLAY_WIDTH-1;
            break;
        case LEFT:
            newHead->x++;
            if(newHead->x>DISPLAY_WIDTH-1) newHead->x = 0;
    }
    newHead->next = snake;
    Snake * current = snake;
    
    while(current->next->next != NULL) {
        if(current->x == newHead->x && current->y == newHead->y) {
            return NULL;
        }
        current = current->next;
    }
    if(add==1) return newHead;
    free(current->next);
    current->next = NULL;
    return newHead;
}