#include "../inc/libmx.h"

char *mx_strjoin(char const *s1, char const *s2){
	int size = 0;
	if (s1 != NULL){
		size += mx_strlen(s1);
	}
	if (s2 != NULL){
		size += mx_strlen(s2);
	}
	if (s1 == NULL && s2 == NULL){
		return NULL;
	}
	size++;
	char *newstr = mx_strnew(size);
	if (s1 != NULL)
		mx_strcpy(newstr, s1);
	if (s2 != NULL)
		mx_strcat(newstr, s2);	
	newstr[size] = '\0';
	return newstr;
}
