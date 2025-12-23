#include <iostream>
#include <unistd.h>
#include "parallel_scheduler.h"


void increment(void* arg){
	int x = *(int *)arg;
	std::cout << "Thread: " << pthread_self()  << ' ' << ++x << '\n';
	usleep(2000);
	delete (int*)arg;
}

int main(){
	parallel_scheduler ps(3);
	
	for(int i = 0; i < 5; i++){
		int* p = new int(i);
		ps.run(increment, p);
	}
	//sleep(2);
	
}
