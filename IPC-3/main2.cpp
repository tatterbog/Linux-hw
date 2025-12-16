#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include "shared_array.h"
#include <signal.h>

sem_t* sem = nullptr;
std::string sem_name = "";


void destroyer(){
	if(sem){
		sem_close(sem);
	}

	_exit(0);
}

void destroyer(int){
        if(sem){
                sem_close(sem);
        }

        _exit(0);
}

sem_t* get_semaphore(const std::string& str){
	sem_name = "/" + str + "_sem";
	return sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
}

int main(){
	struct sigaction sa = {};
	sa.sa_handler = destroyer;
	sa.sa_flags = SA_SIGINFO;

	if(sigaction(SIGINT, &sa, nullptr) == -1){
		destroyer();	
		std::cerr << "Sigaction failed/n";
		exit(EXIT_FAILURE);

	}

	shared_array arr("arr", 10);

	sem = get_semaphore("arr");
	if(sem == SEM_FAILED){
		destroyer();
		perror("sem_open");
		exit(EXIT_FAILURE);
	}

	while(true){
		sem_wait(sem);
		arr[0]++;
		arr[1]++;
		sem_post(sem);

		sleep(1);
	}
	
}
