#ifndef STRING_F_H
#define STRING_F_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char* str);
bool are_equal_strings(const char* a, const char* b);
char* get_dynamic_string_from_char_seq(const char* seq);
char* strcat_dynamic(const char* a, const char* b);

#endif