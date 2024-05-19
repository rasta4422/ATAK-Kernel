#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <kernel/vga.h>


static bool check(const char* str, size_t length) {
    const unsigned char* u_str = (const unsigned char*) str;
    for (size_t i = 0; i < length; i++){
        term_print(u_str[i]);
        if ((int)u_str[i] == EOF)
            return false;
    }
    return true;
}

int printf(const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t max = INT_MAX - written;

        /* check if each string is printable to VGA */
        if (format[0] != '%' || format[1] == '%') {
            /* escaping % */
            if (format[0] == '%')
                format++;

            size_t idx = 1;
            while (format[idx] && format[idx] != '%')
                idx++;

            if (max < idx)
                return -1;

            if (!check(format, idx))
                return -1;

            format += idx;
            written += idx;
            continue;
        }

        const char* format_begin = format++;

        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int);
            if (!max)
                return -1;

            if (!check(&c, sizeof(c)))
                return -1;

            written++;
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (max < len)
                return -1;

            if (!check(str, len))
                return -1;

            written += len;
        } else {
            format = format_begin;
            size_t len = strlen(format);
            if (max < len)
                return -1;

            if (!check(format, len))
                return -1;

            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}

