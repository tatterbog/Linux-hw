#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main(int argc, char* argv[]){
	if(argc != 2){
		const char* err = "Wrong amount of elements\n";
		write(2, err, strlen(err));
		return 1;
	}

	const char* filePath = argv[1];
	int fd = open(filePath, O_RDONLY);
	if(fd == -1){
		const char* err = "Error opening this file: ";
		write(2, err, strlen(err));
		write(2, filePath, strlen(filePath));
		write(2, "\n", 1);
		return 2;
	}

	const size_t buffSize = 4096;
	char buff[buffSize];

	ssize_t readB;
    while ((readB = read(fd, buff, buffSize)) > 0) {
		ssize_t written = write(1, buff, readB);
        if (written == -1) {
            const char* err = "outputting error\n";
            write(2, err, strlen(err));
            close(fd);
            return 3;
        }
    }

	if(readB == -1){
	 	const char* err = "reading file error\n";
		write(2, err, strlen(err));
        close(fd);
		return 4;
	}

	close(fd);
}
