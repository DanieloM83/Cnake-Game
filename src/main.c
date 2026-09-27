#include "cnake.h"

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cnake!");

    Game game = {0};

    if (!game_init(&game)) {
        TraceLog(LOG_ERROR, "Failed to initialize game");
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {
        const float delta_time = GetFrameTime();

        game_update(&game, delta_time);
        game_render(&game);
    }

    game_destroy(&game);
    CloseWindow();

    return 0;
}
