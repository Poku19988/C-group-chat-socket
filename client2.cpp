#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234
#define BUFFER_SIZE 1024

void receive_messages(int socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        long int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            std::cout << "Disconnected from server.\n";
            close(socket);
            exit(0);
        }

        std::cout << "\n" << buffer << "\n> ";
        fflush(stdout);
    }
}

int main() {
    int client_socket;
    sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to server
    sleep(1);
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }


    std::cout << "Connected to the server!\n";
    std::thread receiver(receive_messages, client_socket);
    receiver.detach();

    while (true) {
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message);

        if (message == "exit") {
            close(client_socket);
            break;
        }

        send(client_socket, message.c_str(), message.length(), 0);
    }

    return 0;
}

