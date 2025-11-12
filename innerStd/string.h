#ifndef STRING_F_H
#define STRING_F_H

#include <stddef.h>
#include <stdbool.h>
#include "dataStructures/Vector.h"

size_t string_len(const char* str);
bool string_compare(const char* a, const char* b);
void string_copy_without_alloc(char* destination, const char* source);
char* string_copy(const char* seq);
char* string_concatenate(const char* a, const char* b);
char* string_join_from_array(const char** array_of_strings, size_t size, char separator);
char* string_join(const Vector* strings, char separator);
Vector string_split(const char* str, char separator);
char* string_full_trim(const char* str, char char_to_be_erased);
char* string_trim(const char* str, char char_to_be_erased);
char* string_append(const char* str, char c);
size_t string_get_num_of_char_repeatings(const char* str, char c);
size_t string_get_num_of_words(const char* str, bool(*is_char_a_part_of_word)(char));
void string_mutate_full_trim(char** str, char char_to_be_erased);
void string_mutate_trim(char** str, char char_to_be_erased);
void string_mutate_append(char** str, char c);

#endif