#ifndef STRING_F_H
#define STRING_F_H

#include <stddef.h>
#include <stdbool.h>
#include "dataStructures/Vector.h"

size_t string_get_len(const char* str);
bool string_compare(const char* a, const char* b);
void string_copy(char* destination, const char* source);
char* string_dynamic_copy(const char* seq);
char* string_dynamic_concatenate(const char* a, const char* b);
char* string_dynamic_join(const char** array_of_strings, size_t size, char separator);
Vector string_dynamic_split_to_vector(const char* str, char separator);
char* string_dynamic_full_trim(const char* str, char char_to_be_erased);
char* string_dynamic_append(const char* str, char c);

#endif