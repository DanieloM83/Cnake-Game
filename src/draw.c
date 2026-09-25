#include <raylib.h>

#include "cnake.h"

void draw_cell(int x, int y, Color color) {
    DrawRectangle((x * SCREEN_WIDTH / COLUMNS) + 1, (y * SCREEN_HEIGHT / ROWS) + 1,
                  SCREEN_WIDTH / COLUMNS, SCREEN_HEIGHT / ROWS, color);
}

void draw_snake(float death_percent) {
    int n          = 0;
    SnakeCell* cur = snake->tail;
    while (cur) {
        cur = cur->next;
        n += 1;
    }

    int i = n;
    cur   = snake->tail;
    while (cur->next) {
        draw_cell(cur->x, cur->y, (n * death_percent >= i) ? PINK : DARKGREEN);
        cur = cur->next;
        i -= 1;
    }

    draw_cell(cur->x, cur->y, (n * death_percent >= i) ? PINK : GREEN);
}

void draw_grid(void) {
    for (int i = 1; i < ROWS; i++)
        DrawRectangle(0, i * SCREEN_HEIGHT / ROWS, SCREEN_WIDTH, 1, GRID_COLOR);
    for (int i = 1; i < COLUMNS; i++)
        DrawRectangle(i * SCREEN_WIDTH / COLUMNS, 0, 1, SCREEN_HEIGHT, GRID_COLOR);
}

void draw_apple(void) {
    draw_cell(apple.x, apple.y, RED);
}

void draw_score(void) {
    const char* score_msg  = TextFormat("Score: %d", score);
    const char* record_msg = TextFormat("Record: %d", record);
    int score_width        = MeasureText(score_msg, 20);
    int record_width       = MeasureText(record_msg, 20);
    DrawText(score_msg, SCREEN_WIDTH - score_width - 10, 10, 20, TEXT_COLOR);
    DrawText(record_msg, SCREEN_WIDTH - record_width - 10, 40, 20, TEXT_COLOR);
}

void draw_welcome_text(void) {
    const char* main_msg = "CNAKE!";
    const char* sub_msg  = "Press any key to start...";
    int main_width       = MeasureText(main_msg, 72);
    int sub_width        = MeasureText(sub_msg, 36);
    DrawText(main_msg, (SCREEN_WIDTH - main_width) / 2, SCREEN_HEIGHT / 2 - 36, 72, WHITE);
    DrawText(sub_msg, (SCREEN_WIDTH - sub_width) / 2, SCREEN_HEIGHT / 2 + 36, 36, TEXT_COLOR);
}

void draw_game_over_text(void) {
    const char* main_msg = "GAME OVER!";
    const char* sub_msg  = "Press any key to restart...";
    int main_width       = MeasureText(main_msg, 72);
    int sub_width        = MeasureText(sub_msg, 36);
    DrawText(main_msg, (SCREEN_WIDTH - main_width) / 2, SCREEN_HEIGHT / 2 - 36, 72, WHITE);
    DrawText(sub_msg, (SCREEN_WIDTH - sub_width) / 2, SCREEN_HEIGHT / 2 + 36, 36, TEXT_COLOR);
}