#include "string.h"
#include "allocator.h"
#include <stddef.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}

bool are_equal_strings(const char* a, const char* b) {
    size_t len;
    if ((len = strlen(a)) != strlen(b)) {
        return false;
    }

    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

char* get_dynamic_string_from_char_seq(const char* seq) {
    size_t capacity = strlen(seq) + 1;

    char* result = (char*)allocate(capacity * sizeof(char));
    if (!result) {
        return NULL;
    }

    result[capacity - 1] = '\0';

    for (size_t i = 0; i < capacity - 1; ++i) {
        result[i] = seq[i];
    }

    return result;
}