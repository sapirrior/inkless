#include "input.h"
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

int input_read_key(void) {
    char c;
    ssize_t nread;
    while ((nread = read(STDIN_FILENO, &c, 1)) == 0);
    if (nread <= 0) return KEY_NONE;

    if (c == '\x1b') {
        char seq[8];
        struct timeval tv = {0, 50000}; // 50ms timeout
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
                } else if (seq[2] == ';') {
                    // Handle combinations like \x1b[1;5C (Ctrl+Right) - just consume for now
                    char discard[2];
                    (void)read(STDIN_FILENO, discard, 2);
                    return '\x1b';
                }
            } else {
                switch (seq[1]) {
                    case 'A': return KEY_UP;
                    case 'B': return KEY_DOWN;
                    case 'C': return KEY_NONE; // Right arrow
                    case 'D': return KEY_NONE; // Left arrow
                    case 'H': return KEY_HOME;
                    case 'F': return KEY_END;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H': return KEY_HOME;
                case 'F': return KEY_END;
            }
        }
        return '\x1b';
    }

    // Map letter keys to InkKeys
    switch (c) {
        case ' ': case 'f': return KEY_PAGE_DOWN;
        case 'b':           return KEY_PAGE_UP;
        case 'd':           return KEY_HALF_DOWN;
        case 'u':           return KEY_HALF_UP;
        case 'g': case '<': return KEY_HOME;
        case 'G': case '>': return KEY_END;
        case 'j':           return KEY_DOWN;
        case 'k':           return KEY_UP;
    }

    return (int)c;
}
