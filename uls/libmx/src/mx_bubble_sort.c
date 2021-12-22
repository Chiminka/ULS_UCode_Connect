#include "../inc/libmx.h"

void mx_bubble_sort(char **arr, int size){
	for(int i = 0 ; i < size - 1; i++) { 
		for(int j = 0 ; j < size - i - 1 ; j++) {  
			if(mx_strcmp(arr[j], arr[j+1]) > 0) {           
				char *tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp; 
				free(tmp);
			}
		}
	}
}
