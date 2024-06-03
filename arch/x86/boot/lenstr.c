#include <string.h>

unsigned int lenstr(const char *s) {
    unsigned int len = 0;
    while (s[len])
        len++;
    return len;
}
