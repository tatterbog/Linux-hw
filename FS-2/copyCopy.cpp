#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main(int argc, char* argv[]){
	if(argc < 3){
		const char* err = "need to input 2 files\n";
		write(STDERR_FILENO, err, strlen(err));
		return 1;
	}

	const char* srcP = argv[1];
	const char* destP = argv[2];
	
	int srcFd = open(srcP, O_RDONLY);
	if(srcFd < 0){
		const char* err = "source file opening error\n";
		write(STDERR_FILENO, err, strlen(err));
		return 2;
	}
	
	int desFd = open(destP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(desFd < 0){
                const char* err = "destination file opening error\n";
                write(STDERR_FILENO, err, strlen(err));
		close(srcFd);
                return 3;
        }

	const size_t BUFF_SIZE = 4096;
	char buffer[BUFF_SIZE];

	ssize_t readB;
	while((readB = read(srcFd, buffer, BUFF_SIZE)) > 0){
		ssize_t writtenB = 0;
		while(writtenB < readB){
			ssize_t res = write(desFd, buffer + writtenB, readB - writtenB);
			if(res < 0){
				const char* err = "error writing to dest file\n";
				write(STDERR_FILENO, err, strlen(err));
				close(srcFd);
				close(desFd);
				return 4;
			}
			writtenB += res;
		}
	}
	
	if(readB < 0){
		const char* err = "error reading from source file\n";
                write(STDERR_FILENO, err, strlen(err));
		close(srcFd);
		close(desFd);
		return 5;
	}
	
	close(srcFd);
        close(desFd);


}
