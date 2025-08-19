#ifndef STD_PARSER
#define STD_PARSER

#include <stddef.h>

char* parse(const char* str, size_t pos, char symbol, size_t* slice_len);

#endif