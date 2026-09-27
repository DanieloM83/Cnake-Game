#include "cnake.h"

void game_setup_round(Game* game) {
    game->snake             = snake_create();
    game->score             = 0;
    game->state             = GAME_START;
    game->next_direction    = game->snake->direction;
    game->simulation_timer  = 0.0f;
    game->death_timer       = 0.0f;
    game->pause_alpha_phase = 0.0f;

    place_apple(game);
}

void game_init(Game* game) {
    *game = (Game){0};
    game_setup_round(game);
}

void game_reset(Game* game) {
    const int record = game->record;

    snake_destroy(game->snake);

    *game        = (Game){0};
    game->record = record;

    game_setup_round(game);
}

void game_update(Game* game, float delta_time) {
    switch (game->state) {
        case GAME_START:
        case GAME_OVER:
            if (GetKeyPressed() != 0) {
                game_reset(game);
                game->state = GAME_PLAYING;
            }
            break;

        case GAME_PLAYING:
            game->simulation_timer += delta_time;

            if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && game->snake->direction.y != 1) {
                game->next_direction = (Vector2){0, -1};
            } else if (
                (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && game->snake->direction.x != 1
            ) {
                game->next_direction = (Vector2){-1, 0};
            } else if (
                (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && game->snake->direction.y != -1
            ) {
                game->next_direction = (Vector2){0, 1};
            } else if (
                (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && game->snake->direction.x != -1
            ) {
                game->next_direction = (Vector2){1, 0};
            } else if ((IsKeyPressed(KEY_P))) {
                game->state = GAME_PAUSE;
                return;
            }

            if (game->simulation_timer >= GAME_TICK_INTERVAL && IsWindowFocused()) {
                game->snake->direction = game->next_direction;
                game->simulation_timer = 0;
                switch (snake_step(game->snake, game->apple)) {
                    case SNAKE_ATE_APPLE:
                        game->score++;
                        game->record = max_int(game->record, game->score);
                        if (game->snake->length == ROWS * COLUMNS) {
                            game->state = GAME_WIN;
                            return;
                        } else {
                            place_apple(game);
                        }
                        break;
                    case SNAKE_COLLIDED:
                        game->state = GAME_DYING;
                        return;
                    case SNAKE_MOVED:
                        break;
                }
            }
            break;

        case GAME_PAUSE:
            game->pause_alpha_phase += delta_time;
            if (IsKeyPressed(KEY_P)) {
                game->pause_alpha_phase = 0;
                game->state             = GAME_PLAYING;
                return;
            }
            if (game->pause_alpha_phase >= 1) {
                game->pause_alpha_phase *= -1;
            }
            break;

        case GAME_DYING:
            game->death_timer += delta_time;
            if (game->death_timer >= DEATH_ANIMATION_DURATION) {
                game->death_timer = 0;
                game->state       = GAME_OVER;
            }
            break;

        case GAME_WIN:
            if (GetKeyPressed() != 0) game_reset(game);
            break;
    }
}

void game_destroy(Game* game) {
    snake_destroy(game->snake);
    game->snake = NULL;
}