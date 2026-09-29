/**
 * @file cnake.h
 * @brief Public types, constants, and API for the Cnake game.
 *
 * This header contains the shared game model and the public interfaces
 * for game updates, rendering, snake management, and apple placement.
 */

#if !defined(CNAKE_H)
#define CNAKE_H

#include <raylib.h>
#include <stdbool.h>

// Size constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COLUMNS 20
#define ROWS 15

// Theme constants
#define BACKGROUND_COLOR GetColor(0x181818FF)
#define GRID_COLOR GetColor(0x7e7e7eff)

#define SNAKE_BODY_COLOR DARKGREEN
#define SNAKE_HEAD_COLOR GREEN
#define SNAKE_DEAD_COLOR PINK
#define APPLE_COLOR RED

#define HEADING_COLOR RAYWHITE
#define TEXT_COLOR ColorAlpha(RAYWHITE, 0.5)
#define SUBTEXT_COLOR ColorAlpha(RAYWHITE, 0.5)

#define HEADING_FS 72
#define TEXT_FS 36
#define SUBTEXT_FS 20

// Timers and intervals constants
#define GAME_TICK_INTERVAL 0.1f
#define DEATH_ANIMATION_DURATION 1.0f
#define DEATH_POST_ANIMATION_DELAY 0.75f
#define DEATH_TOTAL_DURATION (DEATH_ANIMATION_DURATION + DEATH_POST_ANIMATION_DELAY)

// Assets constants
#define ASSETS_THEME_SPEED 0.25f  // speed of fading-in / fading-out

// Math functions
static inline int wrap_coordinate(int value, int size) {
    return (value < 0 ? (size + value) % size : value % size);
}

static inline int max_int(int a, int b) {
    return (a > b ? a : b);
}

static inline float max_float(float a, float b) {
    return (a > b ? a : b);
}

static inline float min_float(float a, float b) {
    return (a < b ? a : b);
}

// Game-specific structs and enums
/**
 * @brief A single cell of the snake linked list.
 */
typedef struct SnakeCell {
    struct SnakeCell* next;
    int x;
    int y;
} SnakeCell;

/**
 * @brief Linked-list representation of the snake.
 *
 * The snake grows by adding a new head cell and normally moves by
 * removing its tail cell.
 */
typedef struct SnakeBody {
    SnakeCell *head, *tail;
    Vector2 direction;
    int length;
} SnakeBody;

/**
 * @brief Result of advancing the snake by one simulation step.
 */
typedef enum {
    SNAKE_MOVED,
    SNAKE_ATE_APPLE,
    SNAKE_COLLIDED,
    SNAKE_ALLOCATION_FAILED,
    SNAKE_INVALID_ARGUMENT
} SnakeResult;

/**
 * @brief High-level states of the game lifecycle.
 */
typedef enum {
    GAME_START,
    GAME_PLAYING,
    GAME_DYING,
    GAME_OVER,
    GAME_PAUSE,
    GAME_WIN,
    GAME_ERROR
} GameState;

/**
 * @brief Complete mutable state of one game session.
 *
 * The Game object owns the current snake and stores the score,
 * game state, timers, apple position, and pending direction.
 */
typedef struct Game {
    SnakeBody* snake;
    Vector2 apple;

    int score;
    int record;
    GameState state;

    Vector2 next_direction;
    float simulation_timer;
    float death_timer;
    float pause_alpha_phase;
} Game;

/**
 * @brief Renders the current game state.
 *
 * This function only reads the game state and must not mutate the Game object.
 *
 * @param game Game state to render. If NULL, nothing is rendered.
 */
void game_render(const Game* game);

/**
 * @brief Initializes a new game object.
 *
 * The function is intended for a new Game object that does not currently
 * own any resources. It resets the structure and creates the initial snake
 * and apple.
 *
 * Do not call this function on an already initialized Game object because
 * it overwrites the structure without destroying its existing snake.
 * Use the game's reset flow for restarting an existing game.
 *
 * @param game Game object to initialize.
 *
 * @return true if initialization succeeds, false if the argument is NULL
 *         or a required resource cannot be allocated.
 */
bool game_init(Game* game);

/**
 * @brief Advances the game simulation and processes input.
 *
 * @param game Game state to update.
 * @param delta_time Elapsed time since the previous frame, in seconds.
 */
void game_update(Game* game, float delta_time);

/**
 * @brief Releases resources owned by a game object.
 *
 * @param game Game object whose resources should be released.
 */
void game_destroy(Game* game);

/**
 * @brief Creates a new snake in its initial state.
 *
 * @return A newly allocated snake, or NULL if memory allocation fails.
 *
 * @note The returned snake must eventually be released with snake_destroy().
 */
SnakeBody* snake_create(void);

/**
 * @brief Releases a snake and all of its cells.
 *
 * @param snake Snake to destroy.
 */
void snake_destroy(SnakeBody* snake);

/**
 * @brief Places the apple on a free grid cell.
 *
 * The apple is never placed on a cell occupied by the snake.
 *
 * @param game Game whose apple position should be updated.
 * @return true if the apple was placed successfully, false if the game is invalid or there are no
 * free cells.
 */
bool place_apple(Game* game);

/**
 * @brief Advances the snake by one grid cell.
 *
 * A new head cell is created in the current direction. The tail is removed
 * unless the snake eats the apple. The function also checks self-collision.
 *
 * @param snake Snake to advance.
 * @param apple Current apple position.
 *
 * @return The result of the simulation step:
 *         SNAKE_MOVED,
 *         SNAKE_ATE_APPLE,
 *         SNAKE_COLLIDED,
 *         SNAKE_ALLOCATION_FAILED,
 *         or SNAKE_INVALID_ARGUMENT.
 *
 * @note The snake remains mutated after a collision so the renderer can
 *       display the death animation.
 */
SnakeResult snake_step(SnakeBody* snake, Vector2 apple);

void assets_init(void);
void sound_play_death(void);
void sound_play_move(void);
void sound_play_eat(void);
void music_play_theme(void);
void music_update_theme(const float delta_time);
void music_fade_out(void);
void music_fade_in(void);
void assets_destroy(void);
void assets_toggle_mute(void);

#endif  // CNAKE_H
