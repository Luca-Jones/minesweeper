#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

typedef struct tile {
    int x,y;
    int adjacent_tiles;
    bool is_mine, is_revealed, is_flagged;
} tile_t;

void tile_reveal(tile_t *tile);
void tile_flag(tile_t *tile);

void init_field(int height, int width, int mines, tile_t field[height][width]);
void print_field(int height, int width, tile_t field[height][width], int cursor_x, int cursor_y);
int mine(int row, int col, int height, int width, tile_t field[height][width]);
int is_field_finished(int height, int width, tile_t field[height][width]);

#endif