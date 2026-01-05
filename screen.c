#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

void show_cursor() {
    puts("\033[?25h");
}
void hide_cursor() {
    puts("\033[?25l");
}
void clear_screen() {
    system("clear");
}
