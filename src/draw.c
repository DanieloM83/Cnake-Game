#include <raylib.h>
#include "cnake.h"

void draw_cell(int x, int y, Color color) {
    DrawRectangle((x * S_WIDTH / N) + 1, (y * S_HEIGHT / M) + 1, S_WIDTH / N, S_HEIGHT / M, color);
}

void draw_snake(void) {
    SnakeCell* cur = snake->tail;
    while (cur->next) {
        if (cur->x == snake->head->x && cur->y == snake->head->y) {
            snake = init_snake();
            return;
        }
        draw_cell(cur->x, cur->y, DARKGREEN);
        cur = cur->next;
    }
    draw_cell(cur->x, cur->y, GREEN);
}

void draw_grid(void) {
    Color color = GetColor(0x7e7e7eff);
    for (int i = 1; i < M; i++)
        DrawRectangle(0, i * S_HEIGHT / M, S_WIDTH, 1, color);
    for (int i = 1; i < N; i++)
        DrawRectangle(i * S_WIDTH / N, 0, 1, S_HEIGHT, color);
}

void draw_apple(void) {
    draw_cell(apple.x, apple.y, RED);
}

void draw_score(void) {
    const char *score_msg = TextFormat("Score: %d", score);
    const char *record_msg = TextFormat("Record: %d", record);
    int score_width = MeasureText(score_msg, 20);
    int record_width = MeasureText(record_msg, 20);
    DrawText(score_msg, S_WIDTH - score_width - 10, 10, 20, ColorAlpha(RAYWHITE, 0.5));
    DrawText(record_msg, S_WIDTH - record_width - 10, 40, 20, ColorAlpha(RAYWHITE, 0.5));
}