#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <sys/wait.h>
#include <sstream>

int main(int argc, char** argv){
	std::string str;
	while(true){
		if(!std::getline(std::cin, str)){
			return 0;
		}

		if(str.empty()){
			continue;
		}

		if(str == "exit"){
			return 0;
		}
		
		std::istringstream line(str);
		std::string word;
		std::vector<std::string> words;

		while(line >> word){
			words.push_back(word);
		}
		

		if(words.empty()){
			continue;
		}

		for(int i = 0; i < words.size(); ){
			std::vector<char *> args;
			int fd = -1;
			int j = i;
			for(; j < words.size() && words[j] != ";" && words[j] != "&&" && words[j] != "||"; j++){
				if((words[j] == ">" || words[j] == ">>") && j + 1 < words.size()){
					int flags = O_CREAT | O_WRONLY;
					if(words[j] == ">"){
						flags |= O_TRUNC;
					}
					else{
						flags |= O_APPEND;
					}
					fd = open(words[j + 1].c_str(), flags, 0644);
					if(fd == -1){
						std::cerr << "Open error\n";
						return 1;
					}
					j++;
				}
				else{
					args.push_back(&words[j][0]);
				}
			}
			
			args.push_back(nullptr);
	 		if(args[0] == nullptr){
				i = j + 1;
				break;
			}	

			pid_t pid = fork();
			if(pid == -1){
				std::cerr << "Fork error\n";
        	                return 1;
			}

			if(pid == 0){
				int index = 0;
				if(std::string(args[0]) == "silent"){
					index = 1;
					if(!args[1]){
						std::cerr << "nothing to run\n";
						exit(1);
					}

					std::string file = std::to_string(getpid()) + ".log";
					int fd2 = creat(file.c_str(), 0644);
					if(fd2 == -1){
						std::cerr << "Creating error\n";
						exit(1);
					}

					dup2(fd2, 1);
					dup2(fd2, 2);
					close(fd2);
				}

				else if(fd != -1){
					dup2(fd, 1);
					close(fd);
				}

				const char* old = getenv("PATH");
				if(!old){
					old = "";
				}
          	             	std::string newP = std::string(".:") + old;
                	        setenv("PATH", newP.c_str(), 1);
			
				execvp(args[index], &args[index]);
				std::cerr << "Exec error\n";
                        	exit(1);
			}

			else{
				int status;
				if(waitpid(pid, &status, 0) == -1){
					std::cerr << "WaitPid error\n";
	        	                return 1;
				}
		 		
				if(fd == -1){
					close(fd);	
				}	

				int exit = WIFEXITED(status);
				std::cout << "Exit status: " << exit << '\n';
				
				
				if(j < words.size()){
					if(words[j] == "&&" && exit != 0){
						j++;
						while(j < words.size() && words[j] != ";" && words[j] != "&&" && words[j] !="||"){
							j++;
						}	
					}
				
					else if(words[j] == "||" && exit == 0){
						j++;
						while(j < words.size() && words[j] != ";" && words[j] != "&&" && words[j] !="||"){                                                	        j++;
                                         	}
	
					}

				}
			}
			i = j;
			if(j < words.size() && words[j] == ";"){
				i = j + 1;	
			}
		}	
	}	
}
