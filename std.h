#ifndef STD_H
#define STD_H

#include <stddef.h>

#include "innerStd/io.h"
#include "innerStd/string.h"
#include "innerStd/dataStructures/Vector.h"
#include "innerStd/dataStructures/Stack.h"
#include "innerStd/dataStructures/LinkedList.h"
#include "innerStd/dataStructures/HashMap.h"

int atoi(const char* str);
char* itoa(int num);
char* itoa_binary(int num);
char* itoa_oct(int num);
char* itoa_hex(int num);

void* alloc(size_t size);
void free(void* ptr);

#endif