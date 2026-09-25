#include <raylib.h>
#include <stdlib.h>

#include "cnake.h"

SnakeCell* init_cell(int x, int y) {
    SnakeCell* cell = (SnakeCell*)malloc(sizeof(SnakeCell));
    cell->x = x, cell->y = y;
    cell->next = NULL;

    return cell;
}

void enqueue(int x, int y) {
    SnakeCell* tmp    = init_cell(x, y);
    snake->head->next = tmp;
    snake->head       = tmp;
}

void castrate(void) {
    SnakeCell* tmp = snake->tail;
    snake->tail    = tmp->next;
    free(tmp);
}

void free_snake(SnakeBody* old_snake) {
    if (old_snake == NULL) return;

    SnakeCell* cur = old_snake->tail;
    while (cur) {
        SnakeCell* next = cur->next;
        free(cur);
        cur = next;
    }

    free(old_snake);
}

SnakeBody* init_snake(void) {
    SnakeCell *tail = init_cell(0, ROWS / 2), *body = init_cell(1, ROWS / 2),
              *head = init_cell(2, ROWS / 2);
    tail->next = body, body->next = head;

    SnakeBody* new_snake = (SnakeBody*)malloc(sizeof(SnakeBody));
    new_snake->head = head, new_snake->tail = tail;
    new_snake->direction = (Vector2){1, 0};

    score = 0;

    apple = (Vector2){GetRandomValue(1, COLUMNS - 1), GetRandomValue(0, ROWS - 1)};

    return new_snake;
}

bool in_snake(int x, int y, bool exclude_head) {
    SnakeCell* cur = snake->tail;
    while (cur) {
        if (exclude_head && cur == snake->head) break;
        if (cur->x == x && cur->y == y) return true;
        cur = cur->next;
    }
    return false;
}

void place_apple(void) {
    while (in_snake(apple.x, apple.y, false))
        apple = (Vector2){GetRandomValue(0, COLUMNS - 1), GetRandomValue(0, ROWS - 1)};
}

bool update_game(void) {
    enqueue(in_bounds(snake->head->x + snake->direction.x, COLUMNS),
            in_bounds(snake->head->y + snake->direction.y, ROWS));

    if (snake->head->x == apple.x && snake->head->y == apple.y) {
        score++;
        record = ((record > score) ? record : score);
        place_apple();
    } else
        castrate();

    return in_snake(snake->head->x, snake->head->y, true);
}
