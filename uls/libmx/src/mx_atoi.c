#include "../inc/libmx.h"

long long mx_atoi(const char *str){
  	long long result = 0;
    if(mx_isspace(*str)){
    	return 0;
    }
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] == '+') str++;
    int temp = -1;
    for (int i = sign < 0; str[i] >= '0' && str[i] <= '9'; i++) {
        result += str[i] - '0';

        if (str[i + 1] >= '0' && str[i + 1] <= '9') result *= 10;
        if((str[i+1] < '0' || str[i+1]  > '9')
        	&& str[i+1] != '\0'){
        	return 0;
        }
        temp = i - 1;
    }
    int size = 0;
	while(str[size] != '\0'){
		size++;
	}
    return (long long)result * sign;
}
