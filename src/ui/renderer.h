#ifndef INK_RENDERER_H
#define INK_RENDERER_H

#include <stddef.h>

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} RenderBuf;

void rb_init(RenderBuf *rb);
void rb_append(RenderBuf *rb, const char *s, size_t len);
void rb_printf(RenderBuf *rb, const char *fmt, ...);
void rb_flush(RenderBuf *rb);
void rb_free(RenderBuf *rb);

#endif
