#include "cnake.h"

void draw_cell(int x, int y, Color color) {
    const int pos_x  = (x * SCREEN_WIDTH / COLUMNS) + 1;
    const int pos_y  = (y * SCREEN_HEIGHT / ROWS) + 1;
    const int width  = SCREEN_WIDTH / COLUMNS;
    const int height = SCREEN_HEIGHT / ROWS;
    DrawRectangle(pos_x, pos_y, width, height, color);
}

void draw_snake(const Game* game, float death_percent) {
    const float offset = game->snake->length * death_percent;
    int index          = game->snake->length;
    for (const SnakeCell* cell = game->snake->tail; cell != NULL; cell = cell->next, index--) {
        Color color;
        if (index <= offset)
            color = SNAKE_DEAD_COLOR;
        else if (cell == game->snake->head)
            color = SNAKE_HEAD_COLOR;
        else
            color = SNAKE_BODY_COLOR;

        draw_cell(cell->x, cell->y, color);
    }
}

void draw_grid(void) {
    const int cell_width  = SCREEN_WIDTH / COLUMNS;
    const int cell_height = SCREEN_HEIGHT / ROWS;

    for (int y = 1; y < ROWS; y++) DrawRectangle(0, y * cell_height, SCREEN_WIDTH, 1, GRID_COLOR);

    for (int x = 1; x < COLUMNS; x++)
        DrawRectangle(x * cell_width, 0, 1, SCREEN_HEIGHT, GRID_COLOR);
}

void draw_apple(const Game* game) {
    draw_cell(game->apple.x, game->apple.y, APPLE_COLOR);
}

void draw_score(const Game* game) {
    const char* score_text  = TextFormat("Score: %d", game->score);
    const char* record_text = TextFormat("Record: %d", game->record);
    const int score_x       = (SCREEN_WIDTH - MeasureText(score_text, SUBTEXT_FS)) - 10;
    const int record_x      = (SCREEN_WIDTH - MeasureText(record_text, SUBTEXT_FS)) - 10;
    const int score_y       = 10;
    const int record_y      = score_y + SUBTEXT_FS + 10;
    DrawText(score_text, score_x, score_y, SUBTEXT_FS, SUBTEXT_COLOR);
    DrawText(record_text, record_x, record_y, SUBTEXT_FS, SUBTEXT_COLOR);
}

void draw_welcome_text(void) {
    const char* heading = "CNAKE!";
    const char* text    = "Press any key to start...";
    const int heading_x = (SCREEN_WIDTH - MeasureText(heading, HEADING_FS)) / 2;
    const int text_x    = (SCREEN_WIDTH - MeasureText(text, TEXT_FS)) / 2;
    const int heading_y = (SCREEN_HEIGHT - HEADING_FS) / 2;
    const int text_y    = (SCREEN_HEIGHT + HEADING_FS) / 2;
    DrawText(heading, heading_x, heading_y, HEADING_FS, HEADING_COLOR);
    DrawText(text, text_x, text_y, TEXT_FS, TEXT_COLOR);
}

void draw_game_over_text(void) {
    const char* heading = "GAME OVER!";
    const char* text    = "Press any key to restart...";
    const int heading_x = (SCREEN_WIDTH - MeasureText(heading, HEADING_FS)) / 2;
    const int text_x    = (SCREEN_WIDTH - MeasureText(text, TEXT_FS)) / 2;
    const int heading_y = (SCREEN_HEIGHT - HEADING_FS) / 2;
    const int text_y    = (SCREEN_HEIGHT + HEADING_FS) / 2;
    DrawText(heading, heading_x, heading_y, HEADING_FS, HEADING_COLOR);
    DrawText(text, text_x, text_y, TEXT_FS, TEXT_COLOR);
}

void draw_win_text(void) {
    const char* heading = "YOU WON!";
    const char* text1   = "Congratulations, you beat the game! :o";
    const char* text2   = "Press any key to continue...";
    const int heading_x = (SCREEN_WIDTH - MeasureText(heading, HEADING_FS)) / 2;
    const int text1_x   = (SCREEN_WIDTH - MeasureText(text1, TEXT_FS)) / 2;
    const int text2_x   = (SCREEN_WIDTH - MeasureText(text2, TEXT_FS)) / 2;
    const int heading_y = (SCREEN_HEIGHT - HEADING_FS) / 2;
    const int text1_y   = (SCREEN_HEIGHT + HEADING_FS) / 2;
    const int text2_y   = (SCREEN_HEIGHT + HEADING_FS) / 2 + TEXT_FS;
    DrawText(heading, heading_x, heading_y, HEADING_FS, HEADING_COLOR);
    DrawText(text1, text1_x, text1_y, TEXT_FS, TEXT_COLOR);
    DrawText(text2, text2_x, text2_y, TEXT_FS, TEXT_COLOR);
}

void draw_pause(float alpha) {
    const Color color          = ColorAlpha(RAYWHITE, alpha);
    const int center_x         = SCREEN_WIDTH / 2;
    const int center_y         = SCREEN_HEIGHT / 2;
    const int frame_size       = 150;
    const int bar_width        = 20;
    const int bar_height       = frame_size / 2;
    const int bar_gap          = 20;
    const float roundness      = 0.25f;
    const int round_segments   = 20;
    const int border_thickness = 20;

    const Rectangle frame = {
        center_x - frame_size / 2, center_y - frame_size / 2, frame_size, frame_size
    };

    const Rectangle left_bar = {
        center_x - bar_gap / 2 - bar_width, center_y - bar_height / 2, bar_width, bar_height
    };

    const Rectangle right_bar = {
        center_x + bar_gap / 2, center_y - bar_height / 2, bar_width, bar_height
    };

    DrawRectangleRoundedLines(frame, roundness, round_segments, border_thickness, color);
    DrawRectangleRounded(left_bar, roundness, round_segments, color);
    DrawRectangleRounded(right_bar, roundness, round_segments, color);
}

void game_render(const Game* game) {
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);

    switch (game->state) {
        case GAME_START:
            draw_grid();
            draw_score(game);
            draw_welcome_text();
            break;

        case GAME_PLAYING:
            draw_snake(game, 0.0f);
            draw_apple(game);
            draw_grid();
            draw_score(game);
            break;

        case GAME_PAUSE:
            draw_snake(game, 0.0f);
            draw_apple(game);
            draw_grid();
            draw_score(game);
            draw_pause(fabsf(game->pause_alpha_phase));
            break;

        case GAME_DYING:
            draw_snake(game, game->death_timer);
            draw_apple(game);
            draw_grid();
            draw_score(game);
            break;

        case GAME_OVER:
            draw_grid();
            draw_score(game);
            draw_game_over_text();
            break;

        case GAME_WIN:
            draw_snake(game, 0.0f);
            draw_grid();
            draw_score(game);
            draw_win_text();
            break;
    }

    EndDrawing();
}