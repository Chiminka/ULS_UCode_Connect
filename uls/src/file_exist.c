#include "../inc/uls.h"

bool file_exist(char *name)
{
    struct stat buf;
    return (stat(name, &buf) == 0);
}
