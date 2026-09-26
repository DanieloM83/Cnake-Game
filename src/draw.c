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

void draw_win_text(void) {
    const char* main_msg = "YOU WON!";
    const char* sub_msg1 = "Congratilations, you beat the game! :o";
    const char* sub_msg2 = "Press any key to continue...";
    int main_width       = MeasureText(main_msg, 72);
    int sub_width1       = MeasureText(sub_msg1, 36);
    int sub_width2       = MeasureText(sub_msg2, 36);
    DrawText(main_msg, (SCREEN_WIDTH - main_width) / 2, SCREEN_HEIGHT / 2 - 36, 72, WHITE);
    DrawText(sub_msg1, (SCREEN_WIDTH - sub_width1) / 2, SCREEN_HEIGHT / 2 + 36, 36, TEXT_COLOR);
    DrawText(sub_msg2, (SCREEN_WIDTH - sub_width2) / 2, SCREEN_HEIGHT / 2 + 80, 36, TEXT_COLOR);
}

void draw_pause(float alpha) {
    Color color  = ColorAlpha(WHITE, alpha);
    int size     = 150;
    int wsmall   = 20;
    int hsmall   = size / 2;
    Rectangle r1 = {(SCREEN_WIDTH - size) / 2, (SCREEN_HEIGHT - size) / 2, size, size};
    Rectangle r2 = {SCREEN_WIDTH / 2 - wsmall - 10, (SCREEN_HEIGHT - hsmall) / 2, wsmall, hsmall};
    Rectangle r3 = {SCREEN_WIDTH / 2 + 10, (SCREEN_HEIGHT - hsmall) / 2, wsmall, hsmall};
    DrawRectangleRoundedLines(r1, 0.25, 0, 20, color);
    DrawRectangleRounded(r2, 1, 0, color);
    DrawRectangleRounded(r3, 1, 0, color);
}