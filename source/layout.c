#include "inkless.h"

void layout_init(Layout *layout) {
    layout->display_lines = NULL;
    layout->raw_to_display = NULL;
    layout->display_to_raw = NULL;
    layout->count = 0;
    layout->cap = 0;
}

static void layout_add_line(Layout *layout, const char *line, size_t raw_line) {
    if (layout->count >= layout->cap) {
        size_t new_cap = layout->cap == 0 ? 128 : layout->cap * 2;
        char **new_lines = realloc(layout->display_lines, sizeof(char *) * new_cap);
        size_t *new_d2r = realloc(layout->display_to_raw, sizeof(size_t) * new_cap);
        if (!new_lines || !new_d2r) ink_die("Memory allocation failed for layout expansion");
        layout->display_lines = new_lines;
        layout->display_to_raw = new_d2r;
        layout->cap = new_cap;
    }
    char *dup = strdup(line);
    if (!dup) ink_die("Memory allocation failed for line duplication in layout");
    layout->display_lines[layout->count] = dup;
    layout->display_to_raw[layout->count] = raw_line;
    layout->count++;
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
            layout_add_line(layout, "", i + 1);
            continue;
        }

        size_t start = 0;
        bool first_chunk = true;
        while (start < len) {
            size_t current_raw = first_chunk ? i + 1 : 0;
            size_t remaining = len - start;
            if (remaining <= (size_t)content_width) {
                layout_add_line(layout, raw + start, current_raw);
                break;
            }

            size_t end = start + content_width;
            size_t split = end;
            
            while (split > start && raw[split] != ' ' && raw[split] != '-') {
                split--;
            }

            if (split == start) {
                char *buf = malloc(content_width + 1);
                if (!buf) ink_die("Memory allocation failed for wrap buffer");
                memcpy(buf, raw + start, content_width);
                buf[content_width] = '\0';
                layout_add_line(layout, buf, current_raw);
                free(buf);
                start += content_width;
            } else {
                size_t split_len = split - start;
                if (raw[split] == '-') split_len++;
                
                char *buf = malloc(split_len + 1);
                if (!buf) ink_die("Memory allocation failed for wrap buffer");
                memcpy(buf, raw + start, split_len);
                buf[split_len] = '\0';
                layout_add_line(layout, buf, current_raw);
                free(buf);
                
                start = split;
                if (raw[split] == ' ') start++;
            }
            first_chunk = false;
        }
    }
}

void layout_free(Layout *layout) {
    if (layout->display_lines) {
        for (size_t i = 0; i < layout->count; i++) {
            free(layout->display_lines[i]);
        }
        free(layout->display_lines);
    }
    if (layout->raw_to_display) free(layout->raw_to_display);
    if (layout->display_to_raw) free(layout->display_to_raw);
    layout_init(layout);
}
