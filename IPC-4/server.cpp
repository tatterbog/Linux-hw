#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>

#define PORT 12345
#define BUF_SIZE 1024

struct Client{
    int sock;
    std::string name;
};

std::vector<Client> clients;
pthread_mutex_t clients_mutex;


void broadcast(const std::string& msg) {
    pthread_mutex_lock(&clients_mutex);
    for (auto& c : clients){
        send(c.sock, msg.c_str(), msg.size(), 0);
    }
    pthread_mutex_unlock(&clients_mutex);
}


void send_list(int sock) {
    pthread_mutex_lock(&clients_mutex);
    std::string list = "Users:\n";
    for (auto& c : clients){
        list += "- " + c.name + "\n";
    }
    pthread_mutex_unlock(&clients_mutex);

    send(sock, list.c_str(), list.size(), 0);
}

void remove_client(int sock) {
    pthread_mutex_lock(&clients_mutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->sock == sock) {
            std::cout << it->name << " disconnected\n";
            clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void* client_thread(void* arg){
    int sock = *(int*)arg;
    char buf[BUF_SIZE];

    recv(sock, buf, BUF_SIZE, 0);
    std::string name(buf);

    pthread_mutex_lock(&clients_mutex);
    clients.push_back({sock, name});
    pthread_mutex_unlock(&clients_mutex);

    broadcast(name + "joined the chat\n");

    while (true){
        int bytes = recv(sock, buf, BUF_SIZE, 0);
        if (bytes <= 0){
            break;
        }
        
        buf[bytes] = 0;
       
	std::string msg(buf);

        if(!name.empty() && name.back() == '\n'){
	    name.pop_back();
	}
	
	if (msg == "/exit\n"){
            break;
        } 
        else if (msg == "/list\n"){
            send_list(sock);
        } 
        else{
            broadcast(name + ": " + msg);
        }
    }

    remove_client(sock);
    broadcast(name + "left the chat\n");
    close(sock);
    return nullptr;
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    pthread_mutex_init(&clients_mutex, nullptr);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (sockaddr*)&addr, sizeof(addr));
    listen(server_sock, 10);

    std::cout << "Server started on port " << PORT << "\n";

    while(true){
        int client_sock = accept(server_sock, nullptr, nullptr);
        pthread_t tid;
        pthread_create(&tid, nullptr, client_thread, &client_sock);
        pthread_detach(tid);
    }

    pthread_mutex_destroy(&clients_mutex);
}
