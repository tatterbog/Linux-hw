#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main(int argc, char* argv[]){
	if(argc < 2){
		const char* err = "insufficient arguments\n";
		write(STDERR_FILENO, err, strlen(err));
		return 1;
	}

	const char* filePath = argv[1];
	int fd = open(filePath, O_RDONLY);
	if(fd < 0){
		const char* err = "Error opening this file: ";
		write(STDERR_FILENO, err, strlen(err));
		write(STDERR_FILENO, filePath, strlen(filePath));
		write(STDERR_FILENO, "\n", 1);
		return 2;
	}

	const size_t BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];

	ssize_t readB;

	while((readB  = read(fd, buffer, BUFFER_SIZE)) > 0){
		ssize_t writtenB = 0;
		while(writtenB < readB){
			ssize_t res = write(STDOUT_FILENO, buffer + writtenB, readB - writtenB);
			if(res < 0){
				const char* err = "outputting error\n";
				write(STDERR_FILENO, err, strlen(err));
				close(fd);
				return 3;
			}
			writtenB += res;
		}
	}

	if(readB < 0){
	 	const char* err = "reading file error\n";
		write(STDERR_FILENO, err, strlen(err));
        close(fd);
		return 4;
	}

	close(fd);
}
