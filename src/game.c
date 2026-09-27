/**
 * @file game.c
 * @brief Game lifecycle, input handling, and simulation updates.
 *
 * This module owns the high-level game state machine. It processes input,
 * advances the simulation, handles scoring, and transitions between
 * start, playing, pause, death, win, and error states.
 */

#include <stddef.h>

#include "cnake.h"

/**
 * @brief Creates the snake and resets per-round state.
 *
 * The current record is intentionally not modified here.
 */
static bool game_setup_round(Game* game) {
    if (game == NULL) return false;

    game->snake = snake_create();
    if (game->snake == NULL) {
        game->state = GAME_ERROR;
        return false;
    }

    game->score             = 0;
    game->state             = GAME_START;
    game->next_direction    = game->snake->direction;
    game->simulation_timer  = 0.0f;
    game->death_timer       = 0.0f;
    game->pause_alpha_phase = 0.0f;

    if (!place_apple(game)) {
        snake_destroy(game->snake);
        game->snake = NULL;
        game->state = GAME_ERROR;
        return false;
    }

    return true;
}

bool game_init(Game* game) {
    if (game == NULL) return false;

    *game = (Game){0};

    return game_setup_round(game);
}

/**
 * @brief Restarts the current round while preserving the record.
 */
static bool game_reset(Game* game) {
    if (game == NULL) {
        return false;
    }

    const int record = game->record;

    snake_destroy(game->snake);

    *game        = (Game){0};
    game->record = record;
    game->state  = GAME_ERROR;

    return game_setup_round(game);
}

void game_update(Game* game, float delta_time) {
    if (game == NULL) return;

    switch (game->state) {
        case GAME_START:
        case GAME_OVER:
            if (GetKeyPressed() != 0) {
                if (game_reset(game)) game->state = GAME_PLAYING;
            }
            break;

        case GAME_PLAYING:
            if (game->snake == NULL) {
                game->state = GAME_ERROR;
                return;
            }
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
                game->simulation_timer -= GAME_TICK_INTERVAL;
                switch (snake_step(game->snake, game->apple)) {
                    case SNAKE_ATE_APPLE:
                        game->score++;
                        game->record = max_int(game->record, game->score);
                        if (game->snake->length == ROWS * COLUMNS) {
                            game->state = GAME_WIN;
                            return;
                        } else if (!place_apple(game)) {
                            game->state = GAME_ERROR;
                            return;
                        }
                        break;
                    case SNAKE_COLLIDED:
                        game->state = GAME_DYING;
                        return;
                    case SNAKE_INVALID_ARGUMENT:
                    case SNAKE_ALLOCATION_FAILED:
                        game->state = GAME_ERROR;
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
            if (game->death_timer >= DEATH_TOTAL_DURATION) {
                game->death_timer = 0;
                game->state       = GAME_OVER;
            }
            break;

        case GAME_WIN:
            if (GetKeyPressed() != 0)
                if (game_reset(game)) game->state = GAME_PLAYING;
            break;
    }
}

void game_destroy(Game* game) {
    if (game == NULL) return;

    snake_destroy(game->snake);
    game->snake = NULL;
}