#ifndef IO_H_FILE
#define IO_H_FILE

enum StdinBuffer {
    VGA_BUFFER_IN,
};

enum StdoutBuffer {
    VGA_BUFFER_OUT,
};

void init_io_dispatcher(enum StdinBuffer in, enum StdoutBuffer out);

void putchar(char c);
int getchar();

void print(const char* str);
void println(const char* str);
void printf(const char* format, ...);

#endif