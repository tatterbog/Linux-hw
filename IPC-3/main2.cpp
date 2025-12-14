#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include "shared_array.h"


sem_t* get_semaphore(const std::string& str){ 
	std::string name = '/' + str + "_sem";
	return sem_open(name.c_str(), O_CREAT, 0666, 1);
}


int main(){
	shared_array arr("arr", 10);

	sem_t* sem = get_semaphore("arr");
	if(sem == SEM_FAILED){
        perror("Semaphore");
        exit(EXIT_FAILURE);
  }
	
	
	while(true){
		sem_wait(sem);
		std::cout << arr[0] << ' ' << arr[1] << '\n';
		sem_post(sem);

		sleep(1);
	}
	
}
