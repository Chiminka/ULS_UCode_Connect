#include "../inc/libmx.h"

double mx_pow(double n, unsigned int pow){
	double temp = 1;
	for (unsigned int i = 0; i < pow; i++){
		temp *= n;
	}
	if (pow == 0){
		return 1;
	}
	return temp;
}
