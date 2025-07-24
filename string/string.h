#ifndef STRING_F_H
#define STRING_F_H

#include <stddef.h>

static size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}

#endif