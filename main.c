#include "input.h"
#include "tile.h"
#include "screen.h"
#include <stdio.h>

int main() { 

    int HEIGHT = 10;
    int WIDTH = 10;
    int MINES = 10;
    int cursor_x = 0;
    int cursor_y = 0;

    tile_t field[HEIGHT][WIDTH];
    init_field(HEIGHT, WIDTH, MINES, field);

    input info;
    start_input(&info);
    
    hide_cursor();
    clear_screen();
    print_field(HEIGHT, WIDTH, field, cursor_x, cursor_y);
    
    int key = 0;
    int is_game_over = 0;
    do {
        key = read_key();
        if (key == VK_ESCAPE || key == 'q' || key == 'Q') {
            break;
        }
        if (key != 0) {
            switch (key) {
                case 'W':
                case 'w':
                    cursor_x += 0;
                    cursor_y += -1;
                    break;
                case 'A':
                case 'a':
                    cursor_x += -1;
                    cursor_y += 0;
                    break;
                case 'S':
                case 's':
                    cursor_x += 0;
                    cursor_y += 1;
                    break;
                case 'D':
                case 'd':
                    cursor_x += 1;
                    cursor_y += 0;
                    break;
                case 'M':
                case 'm':
                    is_game_over = mine(cursor_y, cursor_x, HEIGHT, WIDTH, field);
                    // tile_reveal(&field[cursor_y][cursor_x]);
                    break;
                case 'F':
                case 'f':
                    tile_flag(&field[cursor_y][cursor_x]);
                    break;
                default:
                    break;  
            }

            if (cursor_x >= WIDTH) cursor_x = WIDTH - 1;
            if (cursor_x < 0) cursor_x = 0;
            if (cursor_y >= HEIGHT) cursor_y = HEIGHT - 1;
            if (cursor_y < 0) cursor_y = 0;

            clear_screen();
            print_field(HEIGHT, WIDTH, field, cursor_x, cursor_y);

            if (is_game_over) {
                clear_screen();
                for (int row = 0; row < HEIGHT; row++) {
                    for (int col = 0; col < WIDTH; col++) {
                        field[row][col].is_revealed = true;
                    }
                }
                cursor_x = -1;
                cursor_y = -1;
                print_field(HEIGHT, WIDTH, field, cursor_x, cursor_y);
                printf("Game Over! You hit a mine :(\n");
            } else if (is_field_finished(HEIGHT, WIDTH, field)) {
                printf("You win!\n");
                break;
            }

            // printf("%c %d %d\n", key, cursor_x, cursor_y); // debugging line
        }
    } while (is_game_over == 0);

    show_cursor();
    end_input(&info);

    return 0;
}
