#include "tile.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	int im = 0;
	for (int in = height*width-mines; in < height*width && im < mines; in++) {
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
                    printf(" @");
                } else {
                    printf(" %d", current_tile.adjacent_tiles);
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
    if (field[row][col].is_revealed) return 0;
    if (field[row][col].is_mine) return -1;
    tile_reveal(&field[row][col]);
    if (field[row][col].adjacent_tiles > 0) return 0;
    if (row-1 >= 0) mine_helper(row-1, col, height, width, field);
    if (row+1 < height) mine_helper(row+1, col, height, width, field);
    if (col-1 >= 0) mine_helper(row, col-1, height, width, field);
    if (col+1 < width) mine_helper(row, col+1, height, width, field);
    return 0;
}
