#include "../inc/uls.h"

int mx_empty_dir(char *dptr)
{
    int files_found = 0;
    if (mx_strcmp(dptr, ".") == 0 || mx_strcmp(dptr, "..") == 0)
    {
        files_found = 1;
    }
    return files_found;
}

