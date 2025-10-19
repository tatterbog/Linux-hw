#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <chrono>

void do_command(char** argv){
	auto start = std::chrono::high_resolution_clock::now();
	pid_t pid = fork();
	

	if(pid == -1){
		write(2, "Fork failed\n", 11);
		exit(1);
	}

	if(pid == 0){
		execvp(argv[0], argv);
		write(2, "Exec failed\n", 11);
		exit(1);
	}

	else{
		int status;
		if(waitpid(pid, &status, 0) == -1){
			write(2, "WaitPid failed\n", 15);
			exit(1);
		}
		auto end =  std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time = end - start;
		
		if(WIFEXITED(status)){
			std::cout << "Exit status: " << WEXITSTATUS(status) << '\n';
		} 
		else if(WIFSIGNALED(status)){
			std::cout << "Signal termination: " << WIFSIGNALED(status) << '\n';
		}
		
		
		std::cout << "Time: " << time.count() << '\n';
	}
}


int main(int argc, char** argv){
	if(argc < 2){
		std::cout << "Not enough arguments\n";
		return 1;
	}

	
	do_command(argv + 1);
}
