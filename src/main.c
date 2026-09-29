/**
 * @file main.c
 * @brief Application entry point and raylib window lifecycle.
 *
 * This module initializes raylib, creates the Game object, runs the main
 * update/render loop, and releases resources on exit.
 *
 * Gameplay and rendering logic are implemented in separate modules.
 */

#include "cnake.h"

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cnake!");

    if (!IsWindowReady()) {
        TraceLog(LOG_ERROR, "Failed to initialize window");
        return 1;
    }

    if (!assets_init()) {
        TraceLog(LOG_ERROR, "Failed to initialize assets");
        assets_destroy();
        CloseWindow();
        return 1;
    }

    Game game = {0};

    if (!game_init(&game)) {
        TraceLog(LOG_ERROR, "Failed to initialize game");
        assets_destroy();
        CloseWindow();
        return 1;
    }

    music_play_theme();
    while (!WindowShouldClose()) {
        const float delta_time = GetFrameTime();

        game_update(&game, delta_time);
        game_render(&game);
    }

    assets_destroy();
    game_destroy(&game);
    CloseWindow();

    return 0;
}
