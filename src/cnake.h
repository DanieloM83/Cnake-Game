#if !defined(CNAKE_H)
#define CNAKE_H

#include <raylib.h>

#define S_WIDTH 800
#define S_HEIGHT 600
#define N 20
#define M 15

inline static int in_bounds(int x, int bounds) {
    return (x < 0 ? (bounds + x) % bounds : x % bounds);
}

typedef struct SnakeCell {
    struct SnakeCell *next;
    int x;
    int y;
} SnakeCell;

typedef struct SnakeBody {
    SnakeCell *head, *tail;
    Vector2 direction;
} SnakeBody;

SnakeBody* init_snake(void);
void eval_pos(void);
void place_apple(void);

void draw_cell(int x, int y, Color color);
void draw_snake(void);
void draw_grid(void);
void draw_apple(void);
void draw_score(void);

extern SnakeBody *snake;
extern Vector2 apple;
extern int score, record;

#endif // CNAKE_H
