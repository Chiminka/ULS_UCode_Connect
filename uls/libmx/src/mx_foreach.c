#include "../inc/libmx.h"

void mx_foreach(int *arr, int size, void (*f) (int)){
    int i = 0;
    while(i < size){
        f(arr[i]);
        i++;
    }
}
