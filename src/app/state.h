#ifndef INK_STATE_H
#define INK_STATE_H

#include "../terminal/terminal.h"
#include "../document/document.h"
#include "../layout/layout.h"
#include <signal.h>
#include <stdbool.h>

typedef struct AppState {
    TerminalState ts;
    Document doc;
    Layout layout;
    int scroll_y;
    const char *filename;
    volatile sig_atomic_t resize_pending;
    char last_pattern[256];
    int last_search_dir;
    bool search_failed;
    bool show_help;
    bool running;
} AppState;

#endif
