#include "cnake.h"

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cnake!");

    Game game = {0};
    game_init(&game);

    while (!WindowShouldClose()) {
        const float delta_time = GetFrameTime();

        game_update(&game, delta_time);
        game_render(&game);
    }

    game_destroy(&game);
    CloseWindow();

    return 0;
}
