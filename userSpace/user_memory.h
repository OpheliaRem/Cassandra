#ifndef USER_MEMORY_H
#define USER_MEMORY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Проверка, что указатель принадлежит пользовательскому пространству
bool is_user_pointer(const void* ptr, size_t size);

// Проверка валидности пользовательского указателя
bool validate_user_pointer(const void* ptr, size_t size);

// Копирование данных из пользовательского пространства в ядро
int copy_from_user(void* kernel_dst, const void* user_src, size_t size);

// Копирование данных из ядра в пользовательское пространство  
int copy_to_user(void* user_dst, const void* kernel_src, size_t size);

// Копирование строки из пользовательского пространства
// Возвращает длину скопированной строки или -1 при ошибке
int copy_string_from_user(char* kernel_dst, const char* user_src, size_t max_len);

#endif