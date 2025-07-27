#include "convert.h"
#include "allocator.h"
#include "string.h"
#include <stdbool.h>
#include <limits.h>

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static char digit_symbols[] = "0123456789";

int string_to_int(const char* str) {
    while (*str == ' ') {
        str++;
    }

    bool is_negative = false;
    if (*str == '-') {
        is_negative = true;
        str++;
    } else if (*str == '+') {
        str++;
    }

    int result = 0;
    while (is_digit(*str)) {
        int digit = *str - '0';
        result = result * 10 + digit;
        str++;
    }

    return is_negative ? -result : result;
}

static int get_number_of_digits(int num) {
    if (num == 0) {
        return 1;
    }

    unsigned int abs_num = num < 0 ? (unsigned int)(-num) : (unsigned int)num;

    int counter = 0;
    while (abs_num) {
        abs_num /= 10;
        ++counter;
    }

    return counter;
}

char* int_to_string(int num) {
    bool is_negative = num < 0;
    unsigned int abs_num;
    
    if (num == INT_MIN) {
        abs_num = (unsigned int)INT_MAX + 1;  // Avoiding overflow
    } else {
        abs_num = is_negative ? (unsigned int)(-num) : (unsigned int)num;
    }

    size_t number_of_digits = get_number_of_digits(num);
    size_t string_capacity = is_negative ? number_of_digits + 2 : number_of_digits + 1;

    char* result = (char*)allocate(string_capacity);
    if (!result) {
        return NULL;
    }

    result[string_capacity - 1] = '\0';

    for (size_t i = 0; i < number_of_digits; ++i) {
        unsigned int digit = abs_num % 10;
        abs_num /= 10;
        result[string_capacity - 2 - i] = digit_symbols[digit];
    }

    if (is_negative) {
        result[0] = '-';
    }

    return result;
}