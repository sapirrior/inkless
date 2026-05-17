#include "layout.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>

static void layout_add_line(Layout *layout, const char *line) {
    if (layout->count >= layout->cap) {
        size_t new_cap = layout->cap == 0 ? 128 : layout->cap * 2;
        char **new_lines = realloc(layout->display_lines, sizeof(char *) * new_cap);
        if (!new_lines) ink_die("Memory allocation failed for layout lines");
        layout->display_lines = new_lines;
        layout->cap = new_cap;
    }
    char *dup = strdup(line);
    if (!dup) ink_die("Memory allocation failed for line duplication in layout");
    layout->display_lines[layout->count++] = dup;
}

void layout_compute(Layout *layout, Document *doc, int cols) {
    layout_free(layout);
    layout_init(layout);

    if (doc->line_count == 0) return;
    
    layout->raw_to_display = malloc(sizeof(size_t) * doc->line_count);
    if (!layout->raw_to_display) ink_die("Memory allocation failed for layout mapping");

    int margin = (cols * 8) / 100;
    int content_width = cols - (2 * margin);
    if (content_width <= 0) content_width = 1;

    for (size_t i = 0; i < doc->line_count; i++) {
        layout->raw_to_display[i] = layout->count;
        char *raw = doc->raw_lines[i];
        size_t len = strlen(raw);

        if (len == 0) {
            layout_add_line(layout, "");
            continue;
        }

        size_t start = 0;
        while (start < len) {
            size_t remaining = len - start;
            if (remaining <= (size_t)content_width) {
                layout_add_line(layout, raw + start);
                break;
            }

            size_t end = start + content_width;
            size_t split = end;
            
            // Look for space or hyphen to split
            while (split > start && raw[split] != ' ' && raw[split] != '-') {
                split--;
            }

            if (split == start) {
                // No split point found, force break at content_width
                char *buf = malloc(content_width + 1);
                if (!buf) ink_die("Memory allocation failed for wrap buffer");
                memcpy(buf, raw + start, content_width);
                buf[content_width] = '\0';
                layout_add_line(layout, buf);
                free(buf);
                start += content_width;
            } else {
                size_t split_len = split - start;
                if (raw[split] == '-') split_len++; // Keep hyphen on current line
                
                char *buf = malloc(split_len + 1);
                if (!buf) ink_die("Memory allocation failed for wrap buffer");
                memcpy(buf, raw + start, split_len);
                buf[split_len] = '\0';
                layout_add_line(layout, buf);
                free(buf);
                
                start = split;
                if (raw[split] == ' ') start++; // Skip space at start of next line
            }
        }
    }
}
