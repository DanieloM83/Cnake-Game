#include <stdlib.h>

#include "cnake.h"

static SnakeCell* cell_create(int x, int y) {
    SnakeCell* cell = malloc(sizeof *cell);

    if (cell == NULL) return NULL;

    cell->x    = x;
    cell->y    = y;
    cell->next = NULL;

    return cell;
}

SnakeBody* snake_create(void) {
    SnakeBody* snake = malloc(sizeof *snake);

    if (snake == NULL) return NULL;

    SnakeCell* tail = cell_create(0, ROWS / 2);
    SnakeCell* body = cell_create(1, ROWS / 2);
    SnakeCell* head = cell_create(2, ROWS / 2);

    if (tail == NULL || body == NULL || head == NULL) {
        free(head);
        free(body);
        free(tail);
        free(snake);
        return NULL;
    }

    tail->next = body;
    body->next = head;

    snake->head      = head;
    snake->tail      = tail;
    snake->direction = (Vector2){1, 0};
    snake->length    = 3;

    return snake;
}

static bool snake_push_head(SnakeBody* snake, int x, int y) {
    if (snake == NULL || snake->head == NULL) return false;

    SnakeCell* new_head = cell_create(x, y);
    if (new_head == NULL) return false;

    snake->head->next = new_head;
    snake->head       = new_head;

    return true;
}

static void snake_pop_tail(SnakeBody* snake) {
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

static bool snake_collided(SnakeBody* snake) {
    if (snake == NULL || snake->head == NULL) return false;

    for (const SnakeCell* cell = snake->tail; cell != snake->head; cell = cell->next)
        if (cell->x == snake->head->x && cell->y == snake->head->y) return true;

    return false;
}

static bool snake_contains(SnakeBody* snake, int x, int y) {
    if (snake == NULL) return false;

    for (const SnakeCell* cell = snake->tail; cell != NULL; cell = cell->next)
        if (cell->x == x && cell->y == y) return true;

    return false;
}

bool place_apple(Game* game) {
    if (game == NULL || game->snake == NULL) return false;

    const int total_cells = ROWS * COLUMNS;

    if (game->snake->length >= total_cells) return false;

    const int free_cells = total_cells - game->snake->length;
    int target           = GetRandomValue(0, free_cells - 1);

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {
            if (snake_contains(game->snake, x, y)) continue;

            if (target == 0) {
                game->apple = (Vector2){x, y};
                return true;
            }

            target--;
        }
    }

    return false;
}

SnakeResult snake_step(SnakeBody* snake, Vector2 apple) {
    if (snake == NULL) return SNAKE_ALLOCATION_FAILED;

    const int next_x = wrap_coordinate(snake->head->x + (int)snake->direction.x, COLUMNS);
    const int next_y = wrap_coordinate(snake->head->y + (int)snake->direction.y, ROWS);

    if (!snake_push_head(snake, next_x, next_y)) {
        return SNAKE_ALLOCATION_FAILED;
    }

    const bool ate_apple = snake->head->x == apple.x && snake->head->y == apple.y;

    if (!ate_apple)
        snake_pop_tail(snake);
    else
        snake->length++;

    if (snake_collided(snake)) return SNAKE_COLLIDED;

    return ate_apple ? SNAKE_ATE_APPLE : SNAKE_MOVED;
}
