#include "screen.h"
#include <stdio.h>

void show_cursor() {
    puts("\033[?25h");
}
void hide_cursor() {
    puts("\033[?25l");
}
void clear_screen() {
    printf("\033[2J\033[H"); /* ANSI clear + home; works on Linux and Windows 10+ consoles */
}
/* no erase, just overprint: frame is fixed-size so it fully covers the last one */
void cursor_home() {
    printf("\033[H");
}
