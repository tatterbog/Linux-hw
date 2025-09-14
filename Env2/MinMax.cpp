#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]){
	if(argc != 4){
		return 0;
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
