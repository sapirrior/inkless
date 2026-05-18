#include "inkless.h"
#include <unistd.h>
#include <sys/select.h>

int input_read_key(void) {
    char c;
    if (read(STDIN_FILENO, &c, 1) <= 0) return KEY_NONE;

    if (c == '\x1b') {
        char seq[8];
        struct timeval tv = {0, 50000};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0) return '\x1b';
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1': case '7': return KEY_HOME;
                        case '4': case '8': return KEY_END;
                        case '5': return KEY_PAGE_UP;
                        case '6': return KEY_PAGE_DOWN;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return KEY_UP;
                    case 'B': return KEY_DOWN;
                    case 'H': return KEY_HOME;
                    case 'F': return KEY_END;
                }
            }
        } else if (seq[0] == 'O') {
            if (seq[1] == 'H') return KEY_HOME;
            if (seq[1] == 'F') return KEY_END;
        }
        return '\x1b';
    }

    return (int)c;
}
