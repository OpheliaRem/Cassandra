#include "string.h"

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