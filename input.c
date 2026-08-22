#include "input.h"
#include <stdio.h>

#ifdef _WIN32

#include <windows.h>
#include <conio.h>

void start_input(input *info) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(out, &mode);
    info->out_mode = mode;
    SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); /* make ANSI escapes work */
    info->codepage = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);                                    /* make box-drawing chars work */
}

int read_key() {
    int key = _getch();
    if (key == 0 || key == 224) {  /* arrow/function keys come as two codes; swallow the second */
        _getch();
        return 0;
    }
    return key;
}

void end_input(input *info) {
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), info->out_mode);
    SetConsoleOutputCP(info->codepage);
}

#else

#include <termios.h>
#include <signal.h>
#include <unistd.h>

/* POSIX-only ctrl-c teardown; ifdef around this when adding windows support.
 * same as show_cursor() + end_input(), but async-signal-safe (no stdio) */
static void on_sigint(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\033[?25h\n", 7);
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &t);
    _exit(130);
}

void start_input(input *info) {
    tcgetattr(0, info);            /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info->c_lflag &= ~(ICANON | ECHO); /* disable canonical mode and key echo */
    info->c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info->c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, info);   /* set immediately */
    signal(SIGINT, on_sigint);     /* restore terminal on ctrl-c */
}

int read_key() {
    return getc(stdin);
}

void end_input(input *info) {
    tcgetattr(0, info);
    info->c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, info);
}

#endif
