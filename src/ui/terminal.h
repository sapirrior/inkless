#ifndef INK_TERMINAL_H
#define INK_TERMINAL_H

#include <termios.h>

typedef struct {
    int rows;
    int cols;
    struct termios orig_termios;
} TerminalState;

void terminal_setup(TerminalState *ts);
void terminal_restore(TerminalState *ts);
void terminal_get_size(TerminalState *ts);
void terminal_enter_alt_buffer(void);
void terminal_exit_alt_buffer(void);
void terminal_hide_cursor(void);
void terminal_show_cursor(void);
void terminal_clear(void);

#endif
