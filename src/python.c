#include <raylib.h>
#include <stdlib.h>
#include "cnake.h"

SnakeCell *init_cell(int x, int y) {
    SnakeCell *cell = (SnakeCell *)malloc(sizeof(SnakeCell));
    cell->x = x, cell->y = y; cell->next = NULL;

    return cell;
}

void enqueue(int x, int y) {
    SnakeCell *tmp = init_cell(x, y);
    snake->head->next = tmp;
    snake->head = tmp;
}

void castrate(void) {
    SnakeCell *tmp = snake->tail;
    snake->tail = tmp->next;
    free(tmp);
}

SnakeBody* init_snake(void) {
    SnakeCell *tail = init_cell(0, 0), *body = init_cell(0, 1), *head = init_cell(0, 2);
    tail->next = body, body->next = head;

    SnakeBody *new_snake = (SnakeBody*)malloc(sizeof(SnakeBody));
    new_snake->head = head, new_snake->tail = tail;
    new_snake->direction = (Vector2){0, 1};
    
    score = 0;

    apple = (Vector2){GetRandomValue(1, N-1), GetRandomValue(0, M-1)};

    return new_snake;
}

bool in_snake(int x, int y) {
    SnakeCell* cur = snake->tail;
    while (cur) {
        if (cur->x == x && cur->y == y)
            return true;
        cur = cur->next;
    }
    return false;
}

void place_apple(void) {
    while (in_snake(apple.x, apple.y))
        apple = (Vector2){GetRandomValue(0, N-1), GetRandomValue(0, M-1)};
}

void eval_pos(void) {
    enqueue(in_bounds(snake->head->x + snake->direction.x, N),
            in_bounds(snake->head->y + snake->direction.y, M));
    if (snake->head->x == apple.x && snake->head->y == apple.y) {
        score++;
        record = ((record > score) ? record : score);
        place_apple();
    } else castrate();
}
