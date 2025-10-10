#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>

int main(int argc, char* argv[]){
	if(argc != 3){
		write(2, "Error with the arguments\n", 25);
		return 1;
	}
	
	char* srcP = argv[1];
	char* dstP = argv[2];
	
	int src = open(srcP, O_RDONLY);
	if(src == -1){
		perror("Source open error");
        return 2;
	}


	int dst = open(dstP, O_CREAT | O_TRUNC |  O_WRONLY, 0644);
        if(dst == -1){
        	perror("Destination open error");
			close(src);
        	return 3;
        }


	off_t size = lseek(src, 0, SEEK_END);
	if(size == -1){
		perror("lseek error");
        close(src);
		close(dst);
        return 4;
	}
	

    if(lseek(src, 0, SEEK_SET) == -1){
		perror("lseek error 2.0");
        close(src);
        close(dst);
        return 5;
	}

	off_t data = 0;
	off_t pos = 0;	
	off_t holes = 0;

	char buf[4096];

	while(pos < size){
		off_t data1 = lseek(src, pos, SEEK_DATA);
		if(data1 == -1){
			data1 = pos;
		}		

		if(data1 > pos){
			holes += data1 - pos;
		}

		off_t hole = lseek(src, data1, SEEK_HOLE);
                if(hole == -1){
                    hole = size;
                }

		lseek(src, data1, SEEK_SET);
		lseek(dst, data1, SEEK_SET);
		
		off_t cp = hole - data1;
		while(cp > 0){
			ssize_t rd;
			if(cp < 4096){
				rd = read(src, buf, cp);
			}
			else{
				rd = read(src, buf, 4096);
			}

			if(rd == -1){
				perror("reading error");
                		close(src);
                		close(dst);
        	        	return 6;
			}

			ssize_t w = write(dst, buf, rd);
			if(w != rd){
				perror("writing error");
                close(src);
                close(dst);
                return 7;
			}
			cp -= w;
			data += w;
		}
		pos = hole;
	}
		
	if(ftruncate(dst, size) == -1){
		perror("writing error");
        close(src);
        close(dst);
        return 8;
	}
	
	holes = size - data;
	std::cout << "Succesfully copied " << size << " bytes (Data: " << 
		data << ", Hole: " << holes << ")\n"; 	

	close(src);
	close(dst);

}
