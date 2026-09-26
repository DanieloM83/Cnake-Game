#include <raylib.h>

#include "cnake.h"

SnakeBody* snake;
Vector2 apple;
int score = 0, record = 0;
GameState game_state = GAME_START;

void reset_game(void) {
    free_snake(snake);
    snake = init_snake();
    place_apple();
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cnake!");

    snake = init_snake();

    float alpha                      = 0.0f;
    bool fadingIn                    = true;
    float time_since_last_simulation = 0.0f;
    float death_timer                = 0.0f;
    Vector2 next_direction           = snake->direction;

    while (!WindowShouldClose()) {
        time_since_last_simulation += GetFrameTime();

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        switch (game_state) {
            case GAME_START:
                if (GetKeyPressed() != 0) {
                    reset_game();
                    game_state = GAME_PLAYING;
                }

                draw_grid();
                draw_score();
                draw_welcome_text();
                break;

            case GAME_PLAYING:
                if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && snake->direction.y != 1) {
                    next_direction = (Vector2){0, -1};
                } else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) &&
                           snake->direction.x != 1) {
                    next_direction = (Vector2){-1, 0};
                } else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) &&
                           snake->direction.y != -1) {
                    next_direction = (Vector2){0, 1};
                } else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) &&
                           snake->direction.x != -1) {
                    next_direction = (Vector2){1, 0};
                } else if ((IsKeyPressed(KEY_P))) {
                    game_state = GAME_PAUSE;
                }

                if (time_since_last_simulation >= 0.1f && IsWindowFocused()) {
                    snake->direction           = next_direction;
                    time_since_last_simulation = 0;
                    if (update_game()) {
                        game_state  = GAME_DYING;
                        death_timer = 0.0f;
                    }
                }

                draw_snake(0.0f);
                draw_apple();
                draw_grid();
                draw_score();
                break;

            case GAME_PAUSE:
                if ((IsKeyPressed(KEY_P))) {
                    alpha                      = 0.0f;
                    time_since_last_simulation = 0;
                    game_state                 = GAME_PLAYING;
                }

                if (fadingIn)
                    alpha += GetFrameTime();
                else
                    alpha -= GetFrameTime();

                if (alpha >= 1.0f) {
                    alpha    = 1.0f;
                    fadingIn = false;
                } else if (alpha <= 0.0f) {
                    alpha    = 0.0f;
                    fadingIn = true;
                }

                draw_snake(0.0f);
                draw_apple();
                draw_grid();
                draw_score();
                draw_pause(alpha);
                break;

            case GAME_DYING:
                death_timer += GetFrameTime();
                float death_percent = death_timer / 1;

                draw_snake(death_percent);
                draw_apple();
                draw_grid();
                draw_score();

                if (death_percent >= 1.75) {
                    death_timer = 0;
                    game_state  = GAME_OVER;
                }
                break;
            case GAME_OVER:
                if (GetKeyPressed() != 0) {
                    reset_game();
                    game_state     = GAME_PLAYING;
                    next_direction = snake->direction;
                }

                draw_grid();
                draw_score();
                draw_game_over_text();
                break;

            case GAME_WIN:
                if (GetKeyPressed() != 0) {
                    game_state = GAME_START;
                }

                draw_snake(0.0f);
                draw_grid();
                draw_score();
                draw_win_text();
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    free_snake(snake);

    return 0;
}
