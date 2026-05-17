#include "utils.h"
#include "../app/state.h"
#include <regex.h>
#include <string.h>

void utils_do_search(AppState *app, const char *pattern, int dir) {
    if (!pattern || pattern[0] == '\0') return;
    app->search_failed = false;

    if (app->doc.line_count == 0 || !app->layout.raw_to_display) {
        app->search_failed = true;
        return;
    }

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        app->search_failed = true;
        return;
    }

    size_t current_raw = 0;
    for (size_t i = 0; i < app->doc.line_count; i++) {
        if (app->layout.raw_to_display[i] <= (size_t)app->scroll_y) {
            current_raw = i;
        } else {
            break;
        }
    }

    int found = -1;
    if (dir == 1) {
        // Search forward from next line
        for (size_t i = current_raw + 1; i < app->doc.line_count; i++) {
            if (regexec(&regex, app->doc.raw_lines[i], 0, NULL, 0) == 0) {
                found = (int)i;
                break;
            }
        }
        // Wraparound
        if (found == -1) {
            for (size_t i = 0; i <= current_raw; i++) {
                if (regexec(&regex, app->doc.raw_lines[i], 0, NULL, 0) == 0) {
                    found = (int)i;
                    break;
                }
            }
        }
    } else {
        // Search backward from previous line
        if (current_raw > 0) {
            for (int i = (int)current_raw - 1; i >= 0; i--) {
                if (regexec(&regex, app->doc.raw_lines[i], 0, NULL, 0) == 0) {
                    found = i;
                    break;
                }
            }
        }
        // Wraparound
        if (found == -1) {
            for (int i = (int)app->doc.line_count - 1; i >= (int)current_raw; i--) {
                if (regexec(&regex, app->doc.raw_lines[i], 0, NULL, 0) == 0) {
                    found = i;
                    break;
                }
            }
        }
    }

    if (found != -1 && (size_t)found < app->doc.line_count) {
        app->scroll_y = (int)app->layout.raw_to_display[found];
        // Persistent pattern storage only on success
        strncpy(app->last_pattern, pattern, sizeof(app->last_pattern) - 1);
        app->last_pattern[sizeof(app->last_pattern) - 1] = '\0';
        app->last_search_dir = dir;
    } else {
        app->search_failed = true;
    }

    regfree(&regex);
}
