#pragma once

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>


class parallel_scheduler{
private:
	int capacity;
	bool stop = false;

	pthread_t* tds;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	
	struct Func{
		void (*func)(void*);
		void *arg;
	};	
		
	std::queue<Func> funcs;
		
	static void* worker_start(void* arg);


	void worker_loop();

public:
	explicit parallel_scheduler(int cap);

	void run(void (*func)(void*), void* arg);
	
	~parallel_scheduler();
};

