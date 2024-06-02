#include <string.h>

unsigned int strlen(const char *s) {
    unsigned int len = 0;
    while (s[len])
        len++;
    return len;
}
