#include "../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr){
	unsigned long temp = nbr;
	int size_of_string = 0;
	while (temp >= 16)
	{
		size_of_string++;
		temp /= 16;
	}
	size_of_string++;

	char *result = mx_strnew(size_of_string);
	temp = nbr;
	for (int i = size_of_string - 1; i >= 0; i--)
	{
		if (temp % 16 < 10)
		{
			result[i] = (char)((temp % 16) + 48);
			temp /= 16;
			continue;
		}
		if (temp % 16 >= 10){
			result[i] = (char)((temp % 16) + 87);
			temp /= 16;
		}
	}
	return result;
}
