#include "../inc/libmx.h"

int mx_strcmp(const char *s1, const char *s2){
	int i = 0;
	int flag = 0;
	while(s1[i] != '\0' || s2[i] != '\0'){
		if(s1[i] == s2[i]){
			if (s1[i + 1] == '\0'){
				flag = 0;
				break;
			}
			i++;
			continue;
		}
		if(s1[i] > s2[i]){
			flag = s1[i] - s2[i];
			break;
		}
		if(s1[i] <  s2[i]){
			flag = s1[i] - s2[i];
			break;
		}
	}
	return flag;
}
