#if !defined(CNAKE_H)
#define CNAKE_H

#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COLUMNS 20  // 20
#define ROWS 1      // 15

#define BACKGROUND_COLOR GetColor(0x181818FF)
#define GRID_COLOR GetColor(0x7e7e7eff)
#define TEXT_COLOR ColorAlpha(RAYWHITE, 0.5)

inline static int in_bounds(int x, int bounds) {
    return (x < 0 ? (bounds + x) % bounds : x % bounds);
}

typedef struct SnakeCell {
    struct SnakeCell* next;
    int x;
    int y;
} SnakeCell;

typedef struct SnakeBody {
    SnakeCell *head, *tail;
    Vector2 direction;
} SnakeBody;

typedef enum { GAME_START, GAME_PLAYING, GAME_DYING, GAME_OVER, GAME_PAUSE, GAME_WIN } GameState;

void free_snake(SnakeBody* old_snake);
SnakeBody* init_snake(void);
void place_apple(void);
bool update_game(void);

void draw_cell(int x, int y, Color color);
void draw_snake(float death_percent);
void draw_grid(void);
void draw_apple(void);
void draw_score(void);
void draw_welcome_text(void);
void draw_game_over_text(void);
void draw_win_text(void);
void draw_pause(float alpha);

extern SnakeBody* snake;
extern Vector2 apple;
extern int score, record;
extern GameState game_state;

#endif  // CNAKE_H
