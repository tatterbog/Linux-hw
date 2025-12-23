#include <iostream> 
#include <semaphore.h>
#include <unistd.h> 
#include "shared_array.h" 
#include <signal.h> 


bool stop = false;
void stopper(int){
	stop = true;
}

sem_t* get_semaphore(const std::string& str){
	  std::string sem_name = "/" + str + "_sem";
    return sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
} 

int main(){ 
	struct sigaction sa = {};
  sa.sa_handler = stopper; 
	
	if(sigaction(SIGINT, &sa, nullptr) == -1){
	    std::cerr << "Sigaction failed/n"; 
		  exit(EXIT_FAILURE);
  }
       
	std::string name = "arr";
	shared_array arr(name, 10); 
	sem_t* sem = get_semaphore(name);

  if(sem == SEM_FAILED){
	    arr.unlink();	
		  perror("sem_open");
	    exit(EXIT_FAILURE); 
	}

  while(!stop){ 
		sem_wait(sem); 
		std::cout << arr[0] << ' ' << arr[1] << '\n';
		sem_post(sem); 

		sleep(1); 
	} 
	
	std::string sem_name = "/" + name + "_sem";
	sem_close(sem);
	sem_unlink(sem_name.c_str());
	arr.unlink();
}
