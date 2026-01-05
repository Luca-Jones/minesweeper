#ifndef INPUT_H
#define INPUT_H

#include <termios.h>

#define VK_ESCAPE 27
#define VK_ENTER 13

typedef struct termios input;

void start_input(input *input);
int read_key();
void end_input(input *input);

#endif