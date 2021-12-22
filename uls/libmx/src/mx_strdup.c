#include "../inc/libmx.h"

char *mx_strdup(const char *s1) {
    char *str = mx_strnew(mx_strlen(s1));
    mx_strcpy(str, s1);
    return str;
}
