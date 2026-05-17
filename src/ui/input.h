#ifndef INK_INPUT_H
#define INK_INPUT_H

typedef enum {
    KEY_NONE = 0,
    KEY_UP = 1000,
    KEY_DOWN,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HALF_UP,
    KEY_HALF_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_SEARCH_FORWARD = '/',
    KEY_SEARCH_BACKWARD = '?',
    KEY_SEARCH_NEXT = 'n',
    KEY_SEARCH_PREV = 'N',
    KEY_QUIT = 'q',
} InkKey;

int input_read_key(void);

#endif
