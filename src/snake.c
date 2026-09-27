#include "cnake.h"

SnakeCell* cell_create(int x, int y) {
    SnakeCell* cell = (SnakeCell*)malloc(sizeof(SnakeCell));
    if (cell == NULL) return NULL;

    cell->x = x, cell->y = y;
    cell->next = NULL;

    return cell;
}

SnakeBody* snake_create(void) {
    SnakeCell* tail = cell_create(0, ROWS / 2);
    SnakeCell* body = cell_create(1, ROWS / 2);
    SnakeCell* head = cell_create(2, ROWS / 2);
    tail->next      = body;
    body->next      = head;

    SnakeBody* new_snake = (SnakeBody*)malloc(sizeof(SnakeBody));
    new_snake->head      = head;
    new_snake->tail      = tail;
    new_snake->direction = (Vector2){1, 0};
    new_snake->length    = 3;

    return new_snake;
}

void snake_push_head(SnakeBody* snake, int x, int y) {
    SnakeCell* tmp    = cell_create(x, y);
    snake->head->next = tmp;
    snake->head       = tmp;
}

void snake_pop_tail(SnakeBody* snake) {
    if (snake == NULL || snake->tail == NULL || snake->tail->next == NULL) return;
    SnakeCell* tmp = snake->tail;
    snake->tail    = tmp->next;
    free(tmp);
}

void snake_destroy(SnakeBody* old_snake) {
    if (old_snake == NULL) return;

    SnakeCell* cur = old_snake->tail;
    while (cur) {
        SnakeCell* next = cur->next;
        free(cur);
        cur = next;
    }

    free(old_snake);
}

bool snake_collided(SnakeBody* snake) {
    for (const SnakeCell* cell = snake->tail; cell != snake->head; cell = cell->next)
        if (cell->x == snake->head->x && cell->y == snake->head->y) return true;

    return false;
}

bool snake_contains(SnakeBody* snake, int x, int y) {
    for (const SnakeCell* cell = snake->tail; cell != NULL; cell = cell->next)
        if (cell->x == x && cell->y == y) return true;

    return false;
}

void place_apple(Game* game) {
    do {
        game->apple = (Vector2){GetRandomValue(0, COLUMNS - 1), GetRandomValue(0, ROWS - 1)};
    } while (snake_contains(game->snake, game->apple.x, game->apple.y));
}

SnakeResult snake_step(SnakeBody* snake, Vector2 apple) {
    snake_push_head(
        snake,
        wrap_coordinate(snake->head->x + snake->direction.x, COLUMNS),
        wrap_coordinate(snake->head->y + snake->direction.y, ROWS)
    );

    const bool ate_apple = snake->head->x == apple.x && snake->head->y == apple.y;

    if (!ate_apple)
        snake_pop_tail(snake);
    else
        snake->length++;

    if (snake_collided(snake)) return SNAKE_COLLIDED;

    return ate_apple ? SNAKE_ATE_APPLE : SNAKE_MOVED;
}
