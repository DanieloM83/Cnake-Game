#include <raylib.h>
#include "cnake.h"

SnakeBody *snake;
Vector2 apple;
int score = 0, record = 0;

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(S_WIDTH, S_HEIGHT, "Cnake!");

    snake = init_snake();
    
    bool direction_setted = true;
    float time_since_last_simulation = 0.0f;

    while (!WindowShouldClose()) {
        time_since_last_simulation += GetFrameTime();
        
        if (!direction_setted)  {
            if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && snake->direction.y != 1)
                snake->direction = (Vector2){0, -1}, direction_setted = true;
            else if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && snake->direction.x != 1)
                snake->direction = (Vector2){-1, 0}, direction_setted = true;
            else if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && snake->direction.y != -1)
                snake->direction = (Vector2){0, 1}, direction_setted = true;
            else if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && snake->direction.x != -1)
                snake->direction = (Vector2){1, 0}, direction_setted = true;
        }

        if (time_since_last_simulation >= 0.1f && IsWindowFocused()) {
            eval_pos();
            time_since_last_simulation = 0;
            direction_setted = false;
        }

        BeginDrawing();
        
        ClearBackground(GetColor(0x181818FF));
        draw_snake();
        draw_apple();
        draw_grid();
        draw_score();
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
