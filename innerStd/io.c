#include "io.h"
#include "../vgaBufferTerminal/terminal.h"
#include "convert.h"
#include "allocator.h"
#include <stddef.h>
#include <stdarg.h>

typedef struct {
    enum StdinBuffer stdin;
    enum StdoutBuffer stdout;
} IoDispatcher;

static IoDispatcher io_dispatcher;

void init_io_dispatcher(enum StdinBuffer in, enum StdoutBuffer out) {
    io_dispatcher.stdin = in;
    io_dispatcher.stdout = out;
}

static void(*select_out_func())(char) {
    switch(io_dispatcher.stdout) {
        case VGA_BUFFER_OUT: return terminal_putchar;
        default: return NULL;
    }
}

static int(*select_in_func())() {
    switch(io_dispatcher.stdin) {
        case VGA_BUFFER_IN: return terminal_getchar;
        default: return NULL;
    }
}

void putchar(char c) {
    void(*out)(char) = select_out_func();
    if (!out) {
        return;
    }

    out(c);
}

int getchar() {
    int(*in)() = select_in_func();
    if (!in) {
        return -1;
    }

    return in();
}

void print(const char* str) {
    for (size_t i = 0; str[i]; ++i) {
        putchar(str[i]);
    }
}

void println(const char* str) {
    print(str);
    putchar('\n');
}

enum Base {
    OCT,
    DEC,
    HEX,
};

static inline char*(*determine_conversion_func(enum Base base))(int) {
    switch(base) {
        case OCT: return int_to_string_oct;
        case DEC: return int_to_string;
        case HEX: return int_to_string_hex;
        default: return int_to_string;
    }
}

static inline void put_formatted_int(int a, enum Base base) {
    char*(*conversion_func)(int) = determine_conversion_func(base);

    char* num_str = conversion_func(a);
    if (!num_str) {
        return;
    }

    print(num_str);
    deallocate(num_str);
}

static inline void put_formatted_element(va_list* factor, char c) {
    switch(c) {
        case 'd':   return put_formatted_int(va_arg(*factor, int), DEC);
        case 'o':   return put_formatted_int(va_arg(*factor, int), OCT);
        case 'x':   return put_formatted_int(va_arg(*factor, int), HEX);
        case 'c':   return putchar((char)va_arg(*factor, int));
        case 's':   return print((const char*)va_arg(*factor, int));
        case '%':   return putchar('%');
        //TODO: implement for floating point numbers
        default:    return;
    }
}

void printf(const char* format, ...) {
    va_list factor;
    va_start(factor, format);
    for (size_t i = 0; format[i]; ++i) {
        if (format[i] == '%') {
            put_formatted_element(&factor, format[++i]);
            continue;
        }

        putchar(format[i]);
    }
    va_end(factor);
}

void scan(char* buf, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        buf[i] = (char)getchar();
    }
}