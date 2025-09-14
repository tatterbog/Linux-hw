#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]){
	if(argc < 2){
		return 0;
	}

	int min = atoi(argv[1]), max = min;

	for(int i = 2; i < argc; i++){
		if(min > atoi(argv[i])){
			min = atoi(argv[i]);
		}

		if(max < atoi(argv[i])){
            max = atoi(argv[i]);
        }
	}

	std::cout << "Min: " << min << ", Max: " << max << "\n";
}
