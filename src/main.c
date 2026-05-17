#include "ui/terminal.h"
#include "ui/renderer.h"
#include "ui/input.h"
#include "core/document.h"
#include "core/layout.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <regex.h>

TerminalState ts;
Document doc;
Layout layout;
int scroll_y = 0;
const char *filename = NULL;
volatile sig_atomic_t resize_pending = 0;

char last_pattern[256] = {0};
int last_search_dir = 1; // 1 for forward, -1 for backward

void handle_sigint(int sig) {
    (void)sig;
    terminal_restore(&ts);
    doc_free(&doc);
    layout_free(&layout);
    exit(0);
}

void handle_sigwinch(int sig) {
    (void)sig;
    resize_pending = 1;
}

void do_search(const char *pattern, int dir) {
    if (!pattern || pattern[0] == '\0') return;
    strncpy(last_pattern, pattern, sizeof(last_pattern) - 1);
    last_search_dir = dir;

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB) != 0) return;

    // Find current raw line index from scroll_y
    size_t current_raw = 0;
    for (size_t i = 0; i < doc.line_count; i++) {
        if (layout.raw_to_display[i] <= (size_t)scroll_y) {
            current_raw = i;
        } else {
            break;
        }
    }

    int found = -1;
    if (dir == 1) {
        for (size_t i = current_raw + 1; i < doc.line_count; i++) {
            if (regexec(&regex, doc.raw_lines[i], 0, NULL, 0) == 0) {
                found = (int)i;
                break;
            }
        }
    } else {
        if (current_raw > 0) {
            for (int i = (int)current_raw - 1; i >= 0; i--) {
                if (regexec(&regex, doc.raw_lines[i], 0, NULL, 0) == 0) {
                    found = i;
                    break;
                }
            }
        }
    }

    if (found != -1) {
        scroll_y = (int)layout.raw_to_display[found];
    }

    regfree(&regex);
}

void read_prompt(char prompt_char, char *buf, size_t size) {
    size_t len = 0;
    buf[0] = '\0';

    while (1) {
        RenderBuf rb;
        rb_init(&rb);
        rb_printf(&rb, "\x1b[%d;1H\x1b[2K%c%s", ts.rows, prompt_char, buf);
        rb_flush(&rb);
        rb_free(&rb);

        int c = input_read_key();
        if (c == '\r' || c == '\n') break;
        if (c == '\x1b') { len = 0; buf[0] = '\0'; break; }
        if (c == 127 || c == 8) { // Backspace
            if (len > 0) buf[--len] = '\0';
        } else if (c >= 32 && c < 127 && len < size - 1) {
            buf[len++] = (char)c;
            buf[len] = '\0';
        }
    }
}

void render_screen(void) {
    RenderBuf rb;
    rb_init(&rb);

    // Clear screen and home cursor
    rb_append(&rb, "\x1b[H", 3);

    int margin = (ts.cols * 8) / 100;
    int view_height = ts.rows - 1;

    for (int i = 0; i < view_height; i++) {
        int line_idx = scroll_y + i;
        
        // Move to start of line and clear it
        rb_printf(&rb, "\x1b[%d;1H\x1b[2K", i + 1);

        if (line_idx < (int)layout.count) {
            // Apply left margin
            for (int j = 0; j < margin; j++) rb_append(&rb, " ", 1);
            rb_append(&rb, layout.display_lines[line_idx], strlen(layout.display_lines[line_idx]));
        }
    }

    // Status Bar (GNU less style)
    rb_printf(&rb, "\x1b[%d;1H\x1b[2K", ts.rows); // Move to last line and clear it

    int current_last_line = scroll_y + view_height;
    if (current_last_line >= (int)layout.count) {
        // End of file reached
        rb_append(&rb, "\x1b[7m(END)\x1b[m", 13);
    } else {
        // Normal state
        rb_append(&rb, ":", 1);
    }

    rb_flush(&rb);
    rb_free(&rb);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    filename = argv[1];
    doc_init(&doc);
    doc_load_file(&doc, filename);

    signal(SIGINT, handle_sigint);
    signal(SIGWINCH, handle_sigwinch);
    terminal_setup(&ts);
    
    layout_init(&layout);
    layout_compute(&layout, &doc, ts.cols);

    while (1) {
        if (resize_pending) {
            terminal_get_size(&ts);
            layout_compute(&layout, &doc, ts.cols);
            resize_pending = 0;
        }
        render_screen();

        int key = input_read_key();
        if (key == KEY_QUIT) break;

        int view_height = ts.rows - 1;

        switch (key) {
            case KEY_UP:
                if (scroll_y > 0) scroll_y--;
                break;
            case KEY_DOWN:
                if (scroll_y + view_height < (int)layout.count) scroll_y++;
                break;
            case KEY_PAGE_UP:
                scroll_y -= view_height;
                if (scroll_y < 0) scroll_y = 0;
                break;
            case KEY_PAGE_DOWN:
                scroll_y += view_height;
                if (scroll_y + view_height > (int)layout.count) {
                    scroll_y = (int)layout.count - view_height;
                    if (scroll_y < 0) scroll_y = 0;
                }
                break;
            case KEY_HALF_UP:
                scroll_y -= view_height / 2;
                if (scroll_y < 0) scroll_y = 0;
                break;
            case KEY_HALF_DOWN:
                scroll_y += view_height / 2;
                if (scroll_y + view_height > (int)layout.count) {
                    scroll_y = (int)layout.count - view_height;
                    if (scroll_y < 0) scroll_y = 0;
                }
                break;
            case KEY_HOME:
                scroll_y = 0;
                break;
            case KEY_END:
                scroll_y = (int)layout.count - view_height;
                if (scroll_y < 0) scroll_y = 0;
                break;
            case KEY_SEARCH_FORWARD: {
                char pattern[256];
                read_prompt('/', pattern, sizeof(pattern));
                do_search(pattern, 1);
                break;
            }
            case KEY_SEARCH_BACKWARD: {
                char pattern[256];
                read_prompt('?', pattern, sizeof(pattern));
                do_search(pattern, -1);
                break;
            }
            case KEY_SEARCH_NEXT:
                do_search(last_pattern, last_search_dir);
                break;
            case KEY_SEARCH_PREV:
                do_search(last_pattern, -last_search_dir);
                break;
        }
    }

    terminal_restore(&ts);
    doc_free(&doc);
    layout_free(&layout);

    return 0;
}
