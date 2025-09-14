#include <iostream>

int main(int argc, char* argv[]){

	for(int i = 0; i < argc/2; i++){
		auto t = argv[i];
	    argv[i] = argv[argc - i - 1];
		argv[argc - i - 1] = t;	
	}

	for(int i = 0; i < argc - 1; i++){
		std::cout << argv[i] << ' ';
	}
	std::cout << "\n";
}
