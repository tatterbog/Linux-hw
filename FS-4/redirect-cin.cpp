#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

void initialize(int argc, char* argv[]){
	if(argc != 2){
		const char* err = "Wrong arguments\n";
		write(2, err, strlen(err));
		exit(1);
	}

	char* filePath = argv[1];
	int fd = open(filePath, O_RDONLY);
	if(fd == -1){
		perror("open");
		exit(2);
	}

	int dp = dup2(fd, 0);
	if(dp == -1){
		perror("dup2");
		close(fd);
		exit(3);	
	}
	close(fd);
}


int main(int argc, char* argv[]){
	
	initialize(argc, argv);

	std::string input; 
	std::cin >> input;

	std::string reversed;
	
	size_t i = 0;
	while(i < input.size()){
		reversed += input[input.size() - i - 1];
		i++;
	}
	
	std::cout << reversed << '\n';

}

