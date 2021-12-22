#include "../inc/libmx.h"

void mx_printstr(const char *s){
	for (int i = 0; i < mx_strlen(s); i++)
		if (s[i] > 31 && s[i] <= 127)
			write(STDOUT_FILENO, &s[i], 1);
}
