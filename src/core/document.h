#ifndef INK_DOCUMENT_H
#define INK_DOCUMENT_H

#include <stddef.h>

typedef struct {
    char **raw_lines;
    size_t line_count;
    size_t line_cap;
} Document;

void doc_init(Document *doc);
void doc_load_file(Document *doc, const char *filename);
void doc_free(Document *doc);

#endif
