#include "convert.h"
#include "allocator.h"
#include "string.h"
#include <stdbool.h>
#include <limits.h>

enum Base {
    BINARY = 2,
    OCTAL = 8,
    DECIMAL = 10,
    HEXADECIMAL = 16,
};

static bool is_digit_binary(char c) {
    return c == '0' || c == '1';
}

static bool is_digit_octal(char c) {
    return c >= '0' && c <= '7';
}

static bool is_digit_decimal(char c) {
    return c >= '0' && c <= '9';
}

static bool is_digit_hexadecimal(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool is_digit(char c, enum Base base) {
    switch (base)
    {
    case BINARY:
        return is_digit_binary(c);
    case OCTAL:
        return is_digit_octal(c);
    case DECIMAL:
        return is_digit_decimal(c);
    case HEXADECIMAL:
        return is_digit_hexadecimal(c);
    default:
        return false;
    }
}

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

    enum Base base = DECIMAL;

    if (*str == '0') {
        str++;
        if (*str == 'b' || *str == 'B') {
            base = BINARY;
            str++;
        } else if (*str == 'x' || *str == 'X') {
            base = HEXADECIMAL;
            str++;
        } else {
            base = OCTAL;
        }
    }

    int result = 0;
    while (is_digit(*str, base)) {
        int digit = 0;
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        }
        else if (*str >= 'a' && *str <= 'f') {
            digit = *str - 'a' + 10;
        }
        else if (*str >= 'A' && *str <= 'F') {
            digit = *str - 'A' + 10;
        }
        result = result * base + digit;
        str++;
    }

    return is_negative ? -result : result;
}

static char digit_symbols[] = "0123456789ABCDEF";

static void memmove_right_by_one(char* str, size_t len) {
    for (size_t i = len; i > 0; --i) {
        str[i] = str[i - 1];
    }
}

static size_t get_number_of_digits(int num, enum Base base) {
    unsigned int abs_num;

    if (num == INT_MIN) {
        abs_num = (unsigned int)INT_MAX + 1; // special case
    } else {
        abs_num = (num < 0) ? (unsigned int)(-num) : (unsigned int)num;
    }

    if (abs_num == 0) {
        return 1;
    }

    size_t counter = 0;
    while (abs_num) {
        abs_num /= base;
        ++counter;
    }

    return counter;
}

static char* int_to_string_any(int num, enum Base base) {
    bool is_negative = (num < 0);
    unsigned int abs_num;

    if (num == INT_MIN) {
        abs_num = (unsigned int)INT_MAX + 1;
    } else {
        abs_num = is_negative ? (unsigned int)(-num) : (unsigned int)num;
    }

    size_t number_of_digits = get_number_of_digits(num, base);

    // длина префикса: "0b", "0x", "0"
    size_t prefix_len = 0;
    switch (base) {
        case BINARY:      prefix_len = 2; break;
        case OCTAL:       prefix_len = 1; break;
        case HEXADECIMAL: prefix_len = 2; break;
        default:          prefix_len = 0; break;
    }

    // итоговый размер: цифры + префикс + минус (если есть) + '\0'
    size_t string_capacity = number_of_digits + prefix_len + (is_negative ? 1 : 0) + 1;

    char* result = (char*)allocate(string_capacity);
    if (!result) {
        return NULL;
    }

    result[string_capacity - 1] = '\0';

    // заполняем цифры справа налево
    size_t pos = string_capacity - 2; // последний символ перед '\0'
    unsigned int tmp = abs_num;

    if (tmp == 0) {
        result[pos--] = '0';
    } else {
        for (size_t i = 0; i < number_of_digits; ++i) {
            unsigned int digit = tmp % base;
            tmp /= base;
            result[pos--] = digit_symbols[digit];
        }
    }

    // префиксы
    switch (base) {
        case BINARY:
            result[0] = '0';
            result[1] = 'b';
            break;
        case OCTAL:
            result[0] = '0';
            break;
        case HEXADECIMAL:
            result[0] = '0';
            result[1] = 'x';
            break;
        default:
            break;
    }

    // минус
    if (is_negative) {
        if (prefix_len > 0) {
            memmove_right_by_one(result, prefix_len); // сдвигаем префикс вправо
            result[0] = '-';
        } else {
            result[0] = '-';
        }
    }

    return result;
}

char* int_to_string(int num) {
    return int_to_string_any(num, DECIMAL);
}

char* int_to_string_binary(int num) {
    return int_to_string_any(num, BINARY);
}

char* int_to_string_oct(int num) {
    return int_to_string_any(num, OCTAL);
}

char* int_to_string_hex(int num) {
    return int_to_string_any(num, HEXADECIMAL);
}