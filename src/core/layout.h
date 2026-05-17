#ifndef INK_LAYOUT_H
#define INK_LAYOUT_H

#include "document.h"
#include <stddef.h>

typedef struct {
    char **display_lines;
    size_t count;
    size_t cap;
    size_t *raw_to_display;
} Layout;

void layout_init(Layout *layout);
void layout_compute(Layout *layout, Document *doc, int cols);
void layout_free(Layout *layout);

#endif
