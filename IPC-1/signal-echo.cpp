#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <ucontext.h>


void handler(int, siginfo_t* info, void* ctx){
	if(!info || !ctx){
		std::cerr << "Didn't get siginfo\n";
		return;
	}

	ucontext_t* uc = (ucontext_t *)ctx;

	uid_t uid = info->si_uid;
	struct passwd* pw = getpwuid(uid);
	const char* username = pw->pw_name;
	if(!pw){
		username = "unknown";
	}

	std::cout << "Recieved SIGUSR1 from PID " << info->si_pid << ", UID " <<uid << " (" << username << ")\n";

#if defined(__x86_64__)
	auto& g = uc->uc_mcontext.gregs;

	if(!&g){
		std::cerr << "Couldn't get info about the registers\n";
                return;
	}

	std::cout << "State of the context: EIP = " << g[REG_RIP] << ", EAX = " << g[REG_RAX] << ", EBX = " << g[REG_RBX] << '\n';
	exit(0);
#else 
	std::cout << "Can't get the info from other architectures\n";
	exit(1);
#endif
}


int main(){
	struct sigaction sa = {};

	pid_t pid = getpid();
	if(pid < 0){
		std::cerr << "Couldn't get the PID\n";
		return 1;
	}	

	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;

	if(sigaction(SIGUSR1, &sa, nullptr) == -1){
		std::cerr << "Sigaction failed\n";
                return 1;
	}

	while(true){
		std::cout << "Working\n";
		sleep(2);
	}
}
