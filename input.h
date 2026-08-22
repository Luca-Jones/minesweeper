#ifndef INPUT_H
#define INPUT_H

#define VK_ESCAPE 27
#define VK_ENTER 13

#ifdef _WIN32
typedef struct {
	unsigned long out_mode;
	unsigned int codepage;
} input;
#else
#include <termios.h>
typedef struct termios input;
#endif

void start_input(input *input);
int read_key();
void end_input(input *input);

#endif
