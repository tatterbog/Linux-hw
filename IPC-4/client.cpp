#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define PORT 12345
#define BUF_SIZE 1024

int sock;

void* recv_thread(void*) {
    char buf[BUF_SIZE];
    while(true){
        int bytes = recv(sock, buf, BUF_SIZE, 0);
        if(bytes <= 0){
            break;
        }
        buf[bytes] = 0;
        std::cout << buf;
    }
    exit(0);
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (sockaddr*)&addr, sizeof(addr));

    std::string name;
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    name += "\n";
    send(sock, name.c_str(), name.size(), 0);

    pthread_t tid;
    pthread_create(&tid, nullptr, recv_thread, nullptr);

    std::string msg;
    while(std::getline(std::cin, msg)){
        msg += "\n";
        send(sock, msg.c_str(), msg.size(), 0);
        if (msg == "/exit\n"){
            break;
        }
    }

    close(sock);
}
