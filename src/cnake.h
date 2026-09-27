#if !defined(CNAKE_H)
#define CNAKE_H

#include <math.h>
#include <raylib.h>
#include <stdlib.h>

// Size consants
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
#define DEATH_ANIMATION_DURATION 1.75f

// Math functions
inline static int wrap_coordinate(int value, int size) {
    return (value < 0 ? (size + value) % size : value % size);
}

inline static int max_int(int a, int b) {
    return (a > b ? a : b);
}

// Game-specific structs and enums
typedef struct SnakeCell {
    struct SnakeCell* next;
    int x;
    int y;
} SnakeCell;

typedef struct SnakeBody {
    SnakeCell *head, *tail;
    Vector2 direction;
    int length;
} SnakeBody;

typedef enum { SNAKE_MOVED, SNAKE_ATE_APPLE, SNAKE_COLLIDED } SnakeResult;

typedef enum { GAME_START, GAME_PLAYING, GAME_DYING, GAME_OVER, GAME_PAUSE, GAME_WIN } GameState;

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

// draw.c
void game_render(const Game* game);

// game.c
void game_init(Game* game);
void game_update(Game* game, float delta_time);
void game_destroy(Game* game);

// python.c
SnakeBody* snake_create(void);
void snake_destroy(SnakeBody* old_snake);
void place_apple(Game* game);
SnakeResult snake_step(SnakeBody* snake, Vector2 apple);

#endif  // CNAKE_H
