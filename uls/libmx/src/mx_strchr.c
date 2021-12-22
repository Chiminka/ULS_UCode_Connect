#include "../inc/libmx.h"

char *mx_strchr(const char *s, int c){
	int i = 0;
	while(s[i] != '\0'){
		if ((char)c != s[i]){
			i++;
			continue;
		}
		else{
			char *temp = (char *)&s[i];
			return temp;
		}
	}
	return NULL;
}
