#include <raylib.h>

#include "cnake.h"

SnakeBody* snake;
Vector2 apple;
int score = 0, record = 0;

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(1000);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cnake!");

    snake = init_snake();

    float time_since_last_simulation = 0.0f;
    Vector2 next_direction           = snake->direction;

    while (!WindowShouldClose()) {
        time_since_last_simulation += GetFrameTime();

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && snake->direction.y != 1) {
            next_direction = (Vector2){0, -1};
        } else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && snake->direction.x != 1) {
            next_direction = (Vector2){-1, 0};
        } else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && snake->direction.y != -1) {
            next_direction = (Vector2){0, 1};
        } else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && snake->direction.x != -1) {
            next_direction = (Vector2){1, 0};
        }

        if (time_since_last_simulation >= 0.1f && IsWindowFocused()) {
            snake->direction = next_direction;
            update_game();
            time_since_last_simulation = 0;
        }

        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);
        draw_snake();
        draw_apple();
        draw_grid();
        draw_score();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
