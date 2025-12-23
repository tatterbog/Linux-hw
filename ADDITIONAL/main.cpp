#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>


#define PORT 80
#define BUF_SIZE 4096

int main() {
    const char* host = "httpforever.com";

    
    hostent* server = gethostbyname(host);
    if (!server) {
        std::cerr << "Couldn't resolve errors\n";
        return 1;
    }

    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

   
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);

    
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0){
        perror("connect");
        close(sock);
        return 1;
    }

   
    const char* request =
        "GET / HTTP/1.1\r\n"
        "Host: httpforever.com\r\n"
        "Connection: close\r\n"
        "\r\n";

    send(sock, request, strlen(request), 0);

    
    std::ofstream file("httpforever.html", std::ios::binary);

    char buffer[BUF_SIZE];
    int bytes;

    while ((bytes = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
        file.write(buffer, bytes);
    }

    file.close();
    close(sock);

    std::cout << "Saved to httpforever.html\n";
    
}
