#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]){

	if(argc != 4){
		std::cout << "Usage" << argv[0] << " num1 num2 num3\n";
		return 1;
	}
	
	for(int i = 1; i <= 3; i++){
		char* a = argv[i];
	        if(*a == '-'){
			a++;
		}

		if(*a == '\0'){
			std::cout << "not a valid number\n";
			return 2;
		}

		while(*a){
			if(*a > '9' || *a < '0'){
				std::cout << "Arguments must have an integer value\n"; 
			return 3;
			}
			a++;
		}
	}

	int min = atoi(argv[1]), max = min;
	int a = atoi(argv[2]), b = atoi(argv[3]); 
	if(min > a){
		min = a;	
	}

	if(min > b){
		min = b;
	}

	if(max < a){
        max = a;
    }

    if(max < b){
        max = b;
	}


	std::cout << "Min: " << min << ", Max: " << max << "\n";
}
