#if !defined(CNAKE_H)
#define CNAKE_H

#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COLUMNS 20
#define ROWS 15

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

SnakeBody* init_snake(void);
void update_game(void);
void place_apple(void);

void draw_cell(int x, int y, Color color);
void draw_snake(void);
void draw_grid(void);
void draw_apple(void);
void draw_score(void);

extern SnakeBody* snake;
extern Vector2 apple;
extern int score, record;

#endif  // CNAKE_H
