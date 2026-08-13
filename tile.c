#include "tile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ANSI colors — edit these to change the look
#define COLOR_RESET "\033[0m"
#define COLOR_MINE  "\033[1;37;41m" // white on red
#define COLOR_1     "\033[34m"   // blue
#define COLOR_2     "\033[32m"   // green
#define COLOR_3     "\033[31m"   // red
#define COLOR_4     "\033[35m"   // magenta
#define COLOR_5     "\033[33m"   // yellow
#define COLOR_6     "\033[36m"   // cyan
#define COLOR_7     "\033[37m"   // white
#define COLOR_8     "\033[90m"   // gray

static const char *number_colors[] = {
    COLOR_RESET, COLOR_1, COLOR_2, COLOR_3, COLOR_4,
    COLOR_5, COLOR_6, COLOR_7, COLOR_8
};

void tile_reveal(tile_t *tile) {
    if (tile->is_revealed) return;
    tile->is_revealed = true;
    tile->is_flagged = false;
}

void tile_flag(tile_t *tile) {
    if (tile->is_revealed || tile->is_flagged) return;
    tile->is_flagged = true;
}

void init_field(int height, int width, int mines, tile_t field[height][width]) {

	for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row ++) {
            field[row][col] = (tile_t) {
                .x = col, .y = row,
                .adjacent_tiles = 0,
                .is_revealed = false, .is_flagged = false, .is_mine = false
            };
        }
    }

    srand(time(NULL));
    int is_used[height*width];
    memset(is_used, 0, sizeof is_used);
	for (int in = height*width-mines; in < height*width; in++) {
        int r = rand() % (in+1);
        if (is_used[r]) {
            r = in;
        }
        field[r/width][r%width].is_mine = true;
        is_used[r] = 1;
    }

    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row ++) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (
                        (dx == 0 && dy == 0) ||
                        col+dx<0 || col+dx>=width ||
                        row+dy<0 || row+dy>=height
                    ) {continue;}
                    if (field[row+dy][col+dx].is_mine) {
                        field[row][col].adjacent_tiles++;
                    }
                }
            }
        }
    }

}

void print_field(int height, int width, tile_t field[height][width], int cursor_x, int cursor_y) {
    printf("╔═");
    for (int i = 0; i < width; i ++) {
        printf("══");
    }
    printf("═╗\n");

    for (int row = 0; row < height; row ++) {
        printf("║ ");
        for (int col = 0; col < width; col ++) {
            tile_t current_tile = field[row][col];
            if (col == cursor_x && row == cursor_y) {
                printf("██");
            } else if (current_tile.is_revealed) {
                if (current_tile.is_mine) {
                    printf(" " COLOR_MINE "@" COLOR_RESET);
                } else {
                    printf(" %s%d" COLOR_RESET, number_colors[current_tile.adjacent_tiles], current_tile.adjacent_tiles);
                }
            } else if (current_tile.is_flagged) {
                printf("║▀");
            } else {
                printf("░░");
            }
        }
        printf(" ║\n");
    }

    printf("╚═");
    for (int i = 0; i < width; i ++) {
        printf("══");
    }
    printf("═╝\n");
}

int is_field_finished(int height, int width, tile_t field[height][width]) {
    for (int row = 0; row < height; row ++) {
        for (int col = 0; col < width; col ++) {
            if (!field[row][col].is_revealed && !field[row][col].is_mine) {
                return 0;
            }
        }
    }
    return 1;
}

void mine_helper(int row, int col, int height, int width, tile_t field[height][width]) {
    if (field[row][col].is_revealed || field[row][col].is_mine) return;
    tile_reveal(&field[row][col]);
    if (field[row][col].adjacent_tiles > 0) return;
    if (row-1 >= 0) {
        mine_helper(row-1, col, height, width, field);
        if (col-1 >= 0) mine_helper(row-1, col-1, height, width, field);
        if (col+1 < width) mine_helper(row-1, col+1, height, width, field);
    }
    if (col-1 >= 0) mine_helper(row, col-1, height, width, field);
    if (col+1 < width) mine_helper(row, col+1, height, width, field);
    if (row+1 < height) {
        mine_helper(row+1, col, height, width, field);
        if (col-1 >= 0) mine_helper(row+1, col-1, height, width, field);
        if (col+1 < width) mine_helper(row+1, col+1, height, width, field);
    }
}

int mine(int row, int col, int height, int width, tile_t field[height][width]) {
    if (field[row][col].is_revealed) {
        /* chord: mine all 8 neighbors, skipping flagged ones */
        int hit = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int r = row+dy, c = col+dx;
                if ((dx == 0 && dy == 0) || r < 0 || r >= height || c < 0 || c >= width) continue;
                if (field[r][c].is_flagged || field[r][c].is_revealed) continue;
                if (mine(r, c, height, width, field) == -1) hit = -1;
            }
        }
        return hit;
    }
    if (field[row][col].is_mine) return -1;
    mine_helper(row, col, height, width, field);
    return 0;
}
