#include "utils.h"
#include <stddef.h>

static const char *help_lines[] = {
    "",
    "  Navigation",
    "    j, Down          Move down one line",
    "    k, Up            Move up one line",
    "    f, Space         Page down",
    "    b                Page up",
    "    d                Half-page down",
    "    u                Half-page up",
    "    g, <, Home       Jump to beginning",
    "    G, >, End        Jump to end",
    "",
    "  Search",
    "    /                Search forward (Regex)",
    "    ?                Search backward (Regex)",
    "    n                Repeat search forward",
    "    N                Repeat search backward",
    "    Esc              Clear active search highlights",
    "",
    "  System",
    "    h                Toggle this help menu",
    "    q                Quit Ink",
    "",
    "  Press Esc or h to return to the document..."
};

const char **utils_get_help_lines(int *count) {
    if (count) *count = sizeof(help_lines) / sizeof(help_lines[0]);
    return help_lines;
}
