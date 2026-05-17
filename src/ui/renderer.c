#include "ui/renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

void rb_init(RenderBuf *rb) {
    rb->cap = 16384;
    rb->data = malloc(rb->cap);
    rb->len = 0;
    if (rb->data) rb->data[0] = '\0';
}

void rb_append(RenderBuf *rb, const char *s, size_t len) {
    if (rb->len + len >= rb->cap) {
        size_t new_cap = rb->cap * 2 + len;
        char *new_data = realloc(rb->data, new_cap);
        if (!new_data) return;
        rb->data = new_data;
        rb->cap = new_cap;
    }
    memcpy(rb->data + rb->len, s, len);
    rb->len += len;
    rb->data[rb->len] = '\0';
}

void rb_printf(RenderBuf *rb, const char *fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0) rb_append(rb, buf, (size_t)n);
}

void rb_flush(RenderBuf *rb) {
    if (rb->len > 0) {
        write(STDOUT_FILENO, rb->data, rb->len);
        rb->len = 0;
        rb->data[0] = '\0';
    }
}

void rb_free(RenderBuf *rb) {
    free(rb->data);
    rb->data = NULL;
    rb->len = 0;
    rb->cap = 0;
}
