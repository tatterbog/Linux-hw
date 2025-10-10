#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]){
	if(argc != 2){
		const char* err = "False amount of arguments\n";
		write(2, err, strlen(err));
		return 1;
	}

	int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(fd == -1){
                const char* err = "Couldn't open\n";
                write(2, err, strlen(err));
                return 2;
        }

	int fd2 = dup(fd);
	if(fd2 == -1){
		const char* err = "Couldn't duplicate\n";
                write(2, err, strlen(err));
		close(fd);
                return 3;
	}

	int w1 = write(fd, "First line\n", 11);
	if(w1 == -1){
                const char* err = "Couldn't write to fd\n";
                write(2, err, strlen(err));
                close(fd);
		close(fd2);
                return 4;
        }


	int w2 = write(fd2, "Second line\n", 12);
	if(w2 == -1){
                const char* err = "Couldn't write to fd2\n";
                write(2, err, strlen(err));
                close(fd);
                close(fd2);
                return 5;
        }

	close(fd);
	close(fd2);

}

