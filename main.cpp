#include <iostream>
#include "Sort.h"
#include "Complex.h"


int main(){

	int n = 5;
	Complex* arr = new Complex[n];

	for(int i = 0; i < n; i++){
		arr[n - i - 1] = Complex(i + 1, (i + 1) * (i + 1));
	}

	Sorting(arr, n);
	
	for(int i = 0; i < n; i++){
		std::cout << arr[i] << ' ';
	}
	std::cout << '\n';

	delete[] arr;

}
