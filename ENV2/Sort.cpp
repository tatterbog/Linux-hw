#include "Sort.h"

void Sorting(Complex* arr, int len){
	for(int i = 1; i < len; i++){
		Complex tmp = arr[i];
		int j = i - 1;
		while(j >= 0 && arr[j] > tmp){
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;
	}
}
