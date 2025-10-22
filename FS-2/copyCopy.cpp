#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main(int argc, char* argv[]){
	if(argc != 3){
		const char* err = "Need to input 2 files\n";
		write(2, err, strlen(err));
		return 1;
	}

	const char* srcP = argv[1];
	const char* destP = argv[2];
	
	int srcFd = open(srcP, O_RDONLY);
	if(srcFd == -1){
		const char* err = "source file opening error\n";
		write(2, err, strlen(err));
		return 2;
	}
	
	int desFd = open(destP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(desFd == -1){
            const char* err = "destination file opening error\n";
            write(2, err, strlen(err));
		    close(srcFd);
            return 3;
    }

	const size_t buffSize = 4096;
	char buff[buffSize];

	ssize_t readB;
	while((readB = read(srcFd, buff, buffSize)) > 0){
		ssize_t writtenB = write(desFd, buff, readB);
        if (writtenB != readB) {
            const char* err = "error writting to the destination file\n";
        	write(2, err, strlen(err));
			close(srcFd);
			close(desFd);
			return 4;
        }
	}
	
	if(readB == -1){
		const char* err = "error reading from the source file\n";
        write(2, err, strlen(err));
		close(srcFd);
		close(desFd);
		return 5;
	}
	
	close(srcFd);
    close(desFd);

}
