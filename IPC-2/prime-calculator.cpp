#include <iostream>
#include <unistd.h>

bool isPrime(int x){
	if(x == 2){
		return true;
	}

	if(x < 2 || (x & 1) == 0){
		return false;
	}
	
	for(int i = 3; i * i <= x; i += 2){
		if(x % i == 0){
			return false;
		}
	}

	return true;
}

int NthNumber(int n){
	int i = 1;
	while(n > 0){
		i++;
		if(isPrime(i)){
			n--;
		}	
	}
	return i;
}


int main(){
	int pfdWR[2];
	int pfdRW[2];

	if(pipe(pfdWR) == -1 || pipe(pfdRW) == -1){
		std::cerr << "Pipe failed\n";
		return 1;
	}

	pid_t pid = fork();
	
	if(pid > 0){
		close(pfdWR[0]);
		close(pfdRW[1]);

		std::string str;
		while(true){
			std::cout << "[Parent] Enter a number: ";
			std::cin >> str;

			if(str == "exit"){
				break;
			}

			for(int i = 0; i < str.size(); i++){
				if(str[i] < '0' || str[i] > '9'){
					std::cerr << "Not a valid number\n";
					return 1;
				}
			}

			int x = atoi(str.c_str());
			if(x <= 0){
				std::cerr << "Non positive number\n";
				return 1;
			}

			std::cout << "[Parent] Sending " << x << "to the child process\n";
			int wb = write(pfdWR[1], &x, sizeof(x));
			if(wb != sizeof(x)){
				std::cerr << "Write failed\n";
				return 1;
			}

			std::cout << "[Parent] Waiting for the child\n";
			int prime;
			int rd = read(pfdRW[0], &prime, sizeof(prime));
			if(rd == -1){
				std::cerr << "Read failed\n";
				return 1;
			}
			std::cout << "[Parent] Recieved calculation result of prime(" << x << ") = " << prime << '\n';  

		}
		close(pfdWR[1]);
		close(pfdRW[0]);
	}

	else if(pid == 0){
		close(pfdWR[1]);
        close(pfdRW[0]);

		while(true){
			int x;	
			int rd = read(pfdWR[0], &x, sizeof(x));
            if(rd == 0){
				return 0;
			}
			
			if(rd == -1){
                std::cerr << "Read failed\n";
                return 1;
            }

			std::cout << "[Child] Calculating the " <<x << "-th prime number\n";
			int prime = NthNumber(x);

			std::cout << "[Child] Sending the result\n";

			int wb = write(pfdRW[1], &prime, sizeof(prime));
            if(wb != sizeof(prime)){
            	std::cerr << "Write failed\n";
            	return 1;
            }
		}

		close(pfdWR[0]);
    	close(pfdRW[1]);

	}

	else{
		std::cerr << "Fork failed\n";
    	return 1;
	}

}

