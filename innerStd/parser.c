#include "parser.h"
#include "string.h"
#include "allocator.h"

char* parse(const char* str, size_t pos, char symbol, size_t* slice_len) {
    size_t len = strlen(str);

    if (pos > len) {
        return NULL;
    }

    str += pos;
    const char* beg = str;

    *slice_len = 0;
    while (*str && *str != symbol) {
        (*slice_len)++;
        str++;
    }

    char* slice = (char*)allocate((*slice_len + 1) * sizeof(char));

    if (!slice) {
        return NULL;
    }

    slice[*slice_len] = '\0';
    for (size_t i = 0; i < *slice_len; ++i) {
        slice[i] = beg[i];
    }

    return slice;
}