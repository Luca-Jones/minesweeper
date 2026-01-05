#include "input.h"
#include <stdio.h>
#include <termios.h>

void start_input(input *info) {
    tcgetattr(0, info);            /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info->c_lflag &= ~ICANON;      /* disable canonical mode */
    info->c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info->c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, info);   /* set immediately */
}

int read_key() {
    return getc(stdin);
}

void end_input(input *info) {
    tcgetattr(0, info);
    info->c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, info);
}