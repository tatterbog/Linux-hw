#pragma once

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>

class shared_array {
	private:
		int fd = -1;
		int* arr = nullptr;
		size_t size;
		std::string name;

		void destroyer(){
			if(arr){
				munmap(arr, size * sizeof(int));
				arr = nullptr;
                 	}

			if(fd != -1){
                        	close(fd);
				fd = -1;
			}
		}

	public:
		shared_array(const std::string& name = "", size_t size = 0) : size(size), name('/' + name){

			if(size == 0){
				std::cerr << "Size is 0\n";
				exit(EXIT_FAILURE);
			}

			fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
			if(fd == -1){
				perror("Shmopen");
				exit(EXIT_FAILURE);
			}


			struct stat st;
			if(fstat(fd, &st) < 0){
				perror("fstat");
				close(fd);
				exit(EXIT_FAILURE);;
			}


			if(st.st_size == 0){
				if(ftruncate(fd, size * sizeof(int)) == -1){
					shm_unlink(name.c_str());
					perror("Ftruncate");
					close(fd);
					exit(EXIT_FAILURE);
				}
			}

			else if(st.st_size != (size * sizeof(int))){
				std::cerr << "Sizes are not equal\n";
				close(fd);
				exit(EXIT_FAILURE);
			}
			arr = (int *)mmap(nullptr,
					size * sizeof(int),
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fd,
					0);

			if(arr == MAP_FAILED){
				shm_unlink(name.c_str());
				perror("mmap");
				close(fd);
			}
		}

		shared_array(const shared_array&) = delete;
		shared_array& operator=(const shared_array&) = delete;

		shared_array(shared_array&& oth){
			fd = oth.fd;
			arr = oth.arr;
			size = oth.size;
			name = std::move(oth.name);

			oth.fd = -1;
			oth.arr = nullptr;
			oth.size = 0;
		}


		shared_array& operator=(shared_array&& oth){
                        if(this != &oth){
				destroyer();

				fd = oth.fd;
                	        arr = oth.arr;
                        	size = oth.size;
                        	name = std::move(oth.name);
	
        	                oth.fd = -1;
                	        oth.arr = nullptr;
                       		oth.size = 0;
                	} 

			return *this;
		}


		int& operator[](size_t index){
			if(index >= size){
				throw std::out_of_range("Indexing out of bounds");
			}
			return arr[index];
		}
	
		~shared_array(){
			destroyer();
		}
};
