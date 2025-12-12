#pragma once

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>

class shared_array {
	private:
		int fd;
		int* arr;
		size_t size;
		std::string name;

	public:
		shared_array(const std::string& name, size_t size): size(size), name('/' + name){
			fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
			if(fd == -1){
				perror("Shmopen");
				exit(EXIT_FAILURE);
			}

			if(ftruncate(fd, size * sizeof(int)) == -1){
				perror("Ftruncate");
				exit(EXIT_FAILURE);
			}

			arr = (int *)mmap(nullptr,
					size * sizeof(int),
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fd,
					0);

			if(arr == MAP_FAILED){
				perror("mmap");
				close(fd);
			}
		}

		int& operator[](size_t index){
			if(index < 0 && index >= size){
				throw std::out_of_range("Indexing out of bounds");
			}
			return arr[index];
		}
	
		~shared_array(){
			munmap(arr, size * sizeof(int));
			close(fd);
		}
};
