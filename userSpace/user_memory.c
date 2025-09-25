// user_memory.c
#include "user_memory.h"
#include "../gdt/gdt_init.h"

// Границы пользовательского пространства (типичные для x86)
#define USER_SPACE_START 0x00000000
#define USER_SPACE_END   0xBFFFFFFF

bool is_user_pointer(const void* ptr, size_t size) {
    uint32_t address = (uint32_t)ptr;
    uint32_t end_address = address + size;
    
    // Проверяем, что адрес и весь диапазон находятся в пользовательском пространстве
    return (end_address >= address && // защита от переполнения
            end_address <= USER_SPACE_END);
}

bool validate_user_pointer(const void* ptr, size_t size) {
    if (!ptr) {
        return false;
    }

    if (!is_user_pointer(ptr, size)) {
        return false;
    }
    
    return true;
}

int copy_from_user(void* kernel_dst, const void* user_src, size_t size) {
    if (!validate_user_pointer(user_src, size) || !kernel_dst) {
        return -1;
    }
    
    // Копируем побайтно с проверкой на доступность каждой страницы
    uint8_t* dst = (uint8_t*)kernel_dst;
    const uint8_t* src = (const uint8_t*)user_src;
    
    for (size_t i = 0; i < size; i++) {
        // Проверяем доступность каждого байта
        if (!validate_user_pointer(src + i, 1)) {
            return -1;
        }
        dst[i] = src[i];
    }
    
    return 0;
}

int copy_to_user(void* user_dst, const void* kernel_src, size_t size) {
    if (!validate_user_pointer(user_dst, size) || !kernel_src) {
        return -1;
    }
    
    uint8_t* dst = (uint8_t*)user_dst;
    const uint8_t* src = (const uint8_t*)kernel_src;
    
    for (size_t i = 0; i < size; i++) {
        if (!validate_user_pointer(dst + i, 1)) {
            return -1;
        }
        dst[i] = src[i];
    }
    
    return 0;
}

int copy_string_from_user(char* kernel_dst, const char* user_src, size_t max_len) {
    if (!validate_user_pointer(user_src, 1) || !kernel_dst || max_len == 0) {
        return -1;
    }
    
    for (size_t i = 0; i < max_len - 1; i++) {
        // Проверяем доступность каждого байта
        if (!validate_user_pointer(user_src + i, 1)) {
            return -1;
        }
        
        kernel_dst[i] = user_src[i];
        
        if (user_src[i] == '\0') {
            return i; // Успешно скопировали строку
        }
    }
    
    // Добавляем нулевой терминатор если не нашли его
    kernel_dst[max_len - 1] = '\0';
    return max_len - 1;
}