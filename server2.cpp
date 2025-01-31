#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <algorithm>

using namespace std;

vector<int> clients;
mutex clients_mutex;

void handle_client(int clientSocket) {
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t data = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (data <= 0) {
            // Client disconnected
            {
                lock_guard<mutex> lock(clients_mutex);
                clients.erase(remove(clients.begin(), clients.end(), clientSocket), clients.end());
            }
            close(clientSocket);
            cout << "Client disconnected\n";
            break;
        }

        string message = "Client: " + string(buffer);
        cout << message << endl;

        // Broadcast message to all clients
        {
            lock_guard<mutex> lock(clients_mutex);
            for (int client : clients) {
                if (client != clientSocket) {
                    send(client, message.c_str(), message.length(), 0);
                }
            }
        }
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Failed to create socket\n";
        return 1;
    }

    // Allow address reuse
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress;
    sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1234);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // **Fixed bind()**
    if (::bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        cerr << "Binding failed\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed\n";
        close(serverSocket);
        return 1;
    }

    cout << "Server is running on port 1234\n";

    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
        if (clientSocket < 0) {
            cerr << "Failed to accept connection\n";
            continue;
        }

        // Store new client
        {
            lock_guard<mutex> lock(clients_mutex);
            clients.push_back(clientSocket);
        }

        cout << "New client connected\n";

        thread(handle_client, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}

