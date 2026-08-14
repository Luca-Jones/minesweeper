#include "input.h"
#include "tile.h"
#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int WIDTH = 10, HEIGHT = 10;
    if (argc > 1) {
        int n = sscanf(argv[1], "%dx%d", &WIDTH, &HEIGHT);
        if (n == 1) HEIGHT = WIDTH;
        if (n < 1 || WIDTH < 2 || HEIGHT < 2) {
            fprintf(stderr, "usage: %s [size | width x height]  (min 2)\n", argv[0]);
            return 1;
        }
    }
    int MINES = HEIGHT*WIDTH/10;
    int cursor_x = 0;
    int cursor_y = 0;

    tile_t field[HEIGHT][WIDTH];
    init_field(HEIGHT, WIDTH, MINES, field);

    input info;
    start_input(&info);
    hide_cursor();
    clear_screen();
    print_field(HEIGHT, WIDTH, MINES, field, cursor_x, cursor_y);
    int key = 0;
    int is_game_over = 0;
    int armed_x = -1, armed_y = -1;  /* first mine-press on a revealed tile arms a chord here */
    do {
        key = read_key();
        if (key == VK_ESCAPE || key == 'q' || key == 'Q') {
            break;
        }
        if (key != 0) {
            switch (key) {
                case 'K':
                case 'k':
                case 'W':
                case 'w':
                    cursor_x += 0;
                    cursor_y += -1;
                    break;
                case 'H':
                case 'h':
                case 'A':
                case 'a':
                    cursor_x += -1;
                    cursor_y += 0;
                    break;
                case 'J':
                case 'j':
                case 'S':
                case 's':
                    cursor_x += 0;
                    cursor_y += 1;
                    break;
                case 'L':
                case 'l':
                case 'D':
                case 'd':
                    cursor_x += 1;
                    cursor_y += 0;
                    break;
                case VK_ENTER:
                case '\n': // terminal keeps ICRNL, Enter arrives as \n
                case 'M':
                case 'm':
                    if (field[cursor_y][cursor_x].is_revealed
                        && !(cursor_x == armed_x && cursor_y == armed_y)) {
                        armed_x = cursor_x;  /* arm: second press here chords */
                        armed_y = cursor_y;
                    } else {
                        is_game_over = mine(cursor_y, cursor_x, HEIGHT, WIDTH, field);
                        armed_x = armed_y = -1;
                    }
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

            /* moving off the armed tile cancels the pending chord */
            if (cursor_x != armed_x || cursor_y != armed_y) {
                armed_x = armed_y = -1;
            }

            cursor_home();
            print_field(HEIGHT, WIDTH, MINES, field, cursor_x, cursor_y);

            if (is_game_over) {
                cursor_home();
                for (int row = 0; row < HEIGHT; row++) {
                    for (int col = 0; col < WIDTH; col++) {
                        field[row][col].is_revealed = true;
                    }
                }
                cursor_x = -1;
                cursor_y = -1;
                print_field(HEIGHT, WIDTH, MINES, field, cursor_x, cursor_y);
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
