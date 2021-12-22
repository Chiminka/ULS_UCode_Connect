#include "../inc/libmx.h"

char *mx_file_to_str(const char *file) {
    int fl = open(file, O_RDONLY);
    int sz = 0;
    int size = 0;
    if (fl == -1) {
        close(fl);
        return NULL;
    }
    size = file_length(file);
    if (size == 0) {
        return NULL;
    }
    char *newstr = mx_strnew(size);
    sz = read(fl, newstr, size);
    close(fl);
    return newstr;
}
