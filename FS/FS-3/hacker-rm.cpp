#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

int main(int argc, char* argv[]){
	if(argc != 2){
		const char* err = "Not enough arguments\n";
		write(2, err, strlen(err));
		return 1;
	}
	

	char* filePath = argv[1];
	
	struct stat st;
	if(stat(filePath, &st) == -1){
		const char* err = "Stat failed\n";
                write(2, err, strlen(err));
                return 2;
	}

	if(!S_ISREG(st.st_mode)){
		const char* err = "Not a regular file\n";
                write(2, err, strlen(err));
                return 3;
	}

	int fd = open(filePath, O_WRONLY);
	if(fd == -1){
		const char* err = "Couldn't open a file\n";
                write(2, err, strlen(err));
                return 4;
	}


	off_t size = lseek(fd, 0, SEEK_END);
	if(size == -1){
		const char* err = "Couldn't reach the end\n";
                write(2, err, strlen(err));
		close(fd);
                return 5;
	}	
	
	const int buffSize = 4096;
	char buff[buffSize];	
	memset(buff, '\0', buffSize);
	
	if(lseek(fd, 0, SEEK_SET) == -1){
		const char* err = "Couldn't get back to start\n";
                write(2, err, strlen(err));
		close(fd);
                return 6;
	}

	off_t bytesToWrite = size;

	while(bytesToWrite > 0){
		ssize_t writeB = buffSize;
		if(buffSize > bytesToWrite){
			writeB = bytesToWrite;
		}

		ssize_t written = write(fd, buff, writeB);
		if(written == -1){
			const char* err = "Couldn't fill with nulls\n";
                	write(2, err, strlen(err));
			close(fd);
        	        return 7;
		}
		bytesToWrite -= written;
	}	


	
	close(fd);
	if(unlink(filePath) == -1){
		const char* err = "Couldn't unlink\n";
                write(2, err, strlen(err));
                return 8;
	}
	
}
