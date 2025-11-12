#include "std.h"
#include "innerStd/convert.h"
#include "innerStd/allocator.h"
#include "innerStd/io.h"

int atoi(const char* str) {
    return string_to_int(str);
}

char* itoa(int num) {
    return int_to_string(num);
}

char* itoa_binary(int num) {
    return int_to_string_binary(num);
}

char* itoa_oct(int num) {
    return int_to_string_oct(num);
}

char* itoa_hex(int num) {
    return int_to_string_oct(num);
}

void* alloc(size_t capacity) {
    return allocate(capacity);
}

void free(void* ptr) {
    return deallocate(ptr);
}
