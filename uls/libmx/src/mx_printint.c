#include "../inc/libmx.h"

void mx_printint(int n){
	int size = 0;
	int temp = n;
	if(temp == 0){
		size++;
	}
	while(temp != 0){
		size++;
		temp /= 10;
	}
	temp = n;
	int num_for_div = 1;
	for (int i = 1; i < size; i++){
		num_for_div *= 10;
	}
	bool flag = false;
	if (n < 0){
		mx_printchar('-');
		if (n == -2147483648)
		{
			flag = true;
			n++;
			n *= -1;
		}
	}
	
	for (int i = 0; i < size; i++){
		temp = n;
		if (i == size - 1){
			if(flag){
				mx_printchar((temp / num_for_div) + 49);
			}else{
				mx_printchar((temp / num_for_div) + 48);
				n = temp - (temp / num_for_div) * num_for_div;
				num_for_div /= 10;	
			}
		}else{
			mx_printchar((temp / num_for_div) + 48);
			n = temp - (temp / num_for_div) * num_for_div;
			num_for_div /= 10;
		}			
	}
}
