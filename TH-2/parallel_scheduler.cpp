#include "parallel_scheduler.h"


void* parallel_scheduler::worker_start(void* arg){
	parallel_scheduler* ps = (parallel_scheduler*)arg;
    ps->worker_loop();
    return nullptr;
}

void parallel_scheduler::worker_loop(){
	while(true){
		pthread_mutex_lock(&mutex);

		while(funcs.empty() && !stop){
			pthread_cond_wait(&cond, &mutex);
		}

		if(stop && funcs.empty()){
			pthread_mutex_unlock(&mutex);
			return;
		}

		Func f = funcs.front();
		funcs.pop();

		pthread_mutex_unlock(&mutex);

		f.func(f.arg);
	}
}


parallel_scheduler::parallel_scheduler(int cap = 0) : capacity(cap){
	pthread_mutex_init(&mutex, nullptr);
	pthread_cond_init(&cond, nullptr);

	tds = new pthread_t[capacity];

	for(int i = 0; i < capacity; i++){
		pthread_create(&tds[i], nullptr, worker_entry, this);
	}
}

void parallel_scheduler::run(void (*func)(void*), void* arg){
	Func f;
	f.func = func;
	f.arg = arg;
	
	pthread_mutex_lock(&mutex);
	funcs.push(f);
	pthread_mutex_unlock(&mutex);

	pthread_cond_signal(&cond);	
}
	
parallel_scheduler::~parallel_scheduler(){
	pthread_mutex_lock(&mutex);
        stop = true;
        pthread_mutex_unlock(&mutex);

        pthread_cond_broadcast(&cond);
	
	for(int i = 0; i < capacity; i++){
		pthread_join(tds[i], nullptr);
	}

	delete[] tds;

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

}	

