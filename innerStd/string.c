#include "string.h"
#include "allocator.h"
#include <stddef.h>

size_t string_len(const char* str) {
    if (!str) {
        return 0;
    }

    size_t len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}

bool string_compare(const char* a, const char* b) {
    if (a == b) {
        return true;
    }

    size_t len;
    if ((len = string_len(a)) != string_len(b)) {
        return false;
    }

    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

char* string_copy_n_chars(const char* seq, size_t n) {
    if (!seq || string_len(seq) < n) {
        return NULL;
    }

    char* res = (char*)allocate((n + 1) * sizeof(char));
    if (!res) {
        return NULL;
    }

    for (size_t i = 0; i < n; ++i) {
        res[i] = seq[i];
    }
    res[n] = '\0';

    return res;
}

char* string_copy(const char* seq) {
    return string_copy_n_chars(seq, string_len(seq));
}

void string_copy_without_alloc(char* destination, const char* source) {
    if (!destination || !source) {
        return;
    }

    size_t len = string_len(source);
    for (size_t i = 0; i < len; ++i) {
        destination[i] = source[i];
    }
    destination[len] = '\0';
}

char* string_concatenate(const char* a, const char* b) {
    size_t len_a = string_len(a);
    size_t len_b = string_len(b);
    size_t len = len_a + len_b;

    char* result = (char*)allocate((len + 1) * sizeof(char));

    for (size_t i = 0; i < len_a; ++i) {
        result[i] = a[i];
    }

    for (size_t i = 0; i < len_b; ++i) {
        result[len_a + i] = b[i];
    }

    result[len] = '\0';

    return result;
}

char* string_join_from_array(const char** array_of_strings, size_t size, char separator) {
    if (!array_of_strings) {
        return NULL;
    }

    size_t full_len = 0;
    for (size_t i = 0; i < size; ++i) {
        full_len += string_len(array_of_strings[i]);
    }

    full_len += size - 1; // for separator symbols

    char* res = (char*)allocate((full_len + 1) * sizeof(char));
    if (!res) {
        return NULL;
    }

    size_t i, j = 0;
    for (i = 0; i < size; ++i) {
        string_copy_without_alloc(res + j, array_of_strings[i]);
        size_t current_len = string_len(array_of_strings[i]);
        res[j + current_len] = separator;
        j += current_len + 1;
    }

    res[full_len] = '\0';

    return res;
}

char* string_join(const Vector* strings, char separator) {
    return string_join_from_array((const char**)strings->data, strings->size, separator);
}

static bool take_piece_of_string_and_push_to_vector(const char* source, size_t piece_begin, size_t piece_end, Vector* destination) {
    char* piece = string_copy_n_chars(source + piece_begin, piece_end - piece_begin);
    if (!piece) {
        vector_foreach(destination, deallocate);
        vector_free(destination);
        return false;
    }
    vector_push(destination, piece);
    return true;
}

Vector string_split(const char* str, char separator) {
    if (!str) {
        return vector_new();
    }

    Vector res = vector_new();

    size_t piece_begin = 0, i;
    for (i = 0; str[i]; ++i) {
        if (str[i] == separator) {
            if (!take_piece_of_string_and_push_to_vector(str, piece_begin, i, &res)) {
                return vector_new();
            }
            piece_begin = i + 1;
        }
    }
    if (!take_piece_of_string_and_push_to_vector(str, piece_begin, i, &res)) {
        return vector_new();
    }

    return res;
}

size_t string_get_num_of_char_repeatings(const char* str, char c) {
    if (!str) {
        return 0;
    }

    int res = 0;
    for (size_t i = 0; str[i]; ++i) {
        if (str[i] == c) {
            ++res;
        }
    }

    return res;
}

char* string_full_trim(const char* str, char char_to_be_erased) {
    if (!str) {
        return NULL;
    }

    const size_t old_len = string_len(str);
    const size_t new_len = old_len - string_get_num_of_char_repeatings(str, char_to_be_erased);

    char* res = (char*)allocate((new_len + 1) * sizeof(char));
    if (!res) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (str[i]) {
        if (str[i] != char_to_be_erased) {
            res[j++] = str[i];
        }
        ++i;
    }

    res[j] = '\0';

    return res;
}

size_t string_get_num_of_words(const char* str, bool(*is_char_a_part_of_word)(char)) {
    if (!str) {
        return 0;
    }

    size_t count = 0;
    bool insideWord = false;
    for (size_t i = 0; str[i]; ++i) {
        if (!is_char_a_part_of_word(str[i])) {
            insideWord = false;
            continue;
        }

        if (!insideWord) {
            ++count;
        }
        insideWord = true;
    }

    return count;
}

size_t string_get_num_of_words_with_context(const char* str, void** context, bool(*is_char_a_part_of_word)(char, void**)) {
    if (!str) {
        return 0;
    }

    size_t count = 0;
    bool insideWord = false;
    for (size_t i = 0; str[i]; ++i) {
        if (!is_char_a_part_of_word(str[i], context)) {
            insideWord = false;
            continue;
        }

        if (!insideWord) {
            ++count;
        }
        insideWord = true;
    }

    return count;
}

static inline bool is_part_of_word_lambda(char c, void** context) {
    char* prohibited_char = (char*)context[0];
    return c != *prohibited_char;
}

char* string_trim(const char* str, char char_to_be_erased) {
    if (!str) {
        return NULL;
    }

    void** context = (void**)allocate(sizeof(void*));
    if (!context) {
        return NULL;
    }

    *context = &char_to_be_erased;

    size_t old_len = string_len(str);
    size_t num_of_repeatings = string_get_num_of_char_repeatings(str, char_to_be_erased);
    size_t num_of_words = string_get_num_of_words_with_context(str, context, is_part_of_word_lambda);
    size_t new_len = old_len - num_of_repeatings + num_of_words - 1;

    deallocate(context);

    char* res = (char*)allocate((new_len + 1) * sizeof(char));
    if (!res) {
        return NULL;
    }

    size_t i = 0;
    while (str[i] && str[i] == char_to_be_erased) {
        ++i;
    }

    size_t j = 0;
    while (str[i]) {
        if (str[i] != char_to_be_erased) {
            res[j++] = str[i++];
            continue;
        }

        if (str[i - 1] != char_to_be_erased) {
            res[j++] = str[i];
        }

        ++i;
    }

    res[new_len] = '\0';

    return res;
}

char* string_append(const char* str, char c) {
    if (!str) {
        return NULL;
    }

    size_t len = string_len(str);

    char* res = (char*)allocate((len + 2) * sizeof(char));
    if (!res) {
        return NULL;
    }

    string_copy_without_alloc(res, str);
    res[len] = c;
    res[len + 1] = '\0';

    return res;
}

static inline void string_mutate_1_arg_char(char** str, char arg, char*(*func)(const char*, char arg)) {
    char* new_str = func(*str, arg);
    deallocate(*str);
    *str = new_str;
}

void string_mutate_full_trim(char** str, char char_to_be_erased) {
    string_mutate_1_arg_char(str, char_to_be_erased, string_full_trim);
}

void string_mutate_trim(char** str, char char_to_be_erased) {
    string_mutate_1_arg_char(str, char_to_be_erased, string_trim);
}

void string_mutate_append(char** str, char c) {
    string_mutate_1_arg_char(str, c, string_append);
}
