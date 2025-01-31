# C++ Chat Group

## Description
This project is a multi-threaded chat server and client application written in C++. The server can handle multiple clients, each of whom can send messages to all other clients. The client connects to the server, sends and receives messages.

## Algorithms and Steps

### Server-Side Algorithm
1. **Create Socket**
    - Create a socket  `socket(AF_INET, SOCK_STREAM, 0)`.
2. **Allow Address Reuse**
    - Enable address reuse using `setsockopt`.
3. **Bind Socket**
    - Bind the socket to a port using `bind`.
4. **Listen for Connections**
    - Listen for incoming connections using `listen`.
5. **Accept Connections**
    - Accept client connections `accept`.
6. **Handle Client**
    - Create a new thread to handle each client connection.
    - Receive messages from the client.
    - Broadcast received messages to all other clients.
    - Remove the client from the list of connected clients when they disconnect.

### Client-Side Algorithm
1. **Create Socket**
    - Create a socket using `socket(AF_INET, SOCK_STREAM, 0)`.
2. **Connect to Server**
    - Connect to the server using `connect`.
3. **Receive Messages**
    - Create a separate thread to receive messages from the server.
    - Print received messages to the console.
4. **Send Messages**
    - Read user input and send messages to the server.
    - Close the socket when the user types `exit`.

## Threading
Threading is used to handle multiple clients simultaneously in the server and to receive messages . Each client connection in the server is handled in a separate thread, ensuring that one client's actions don't block others.

## Socket Binding and Accepting Connections
- **Bind**: The `bind` function assigns the specified IP address and port number to the server socket, allowing it to listen for incoming connections.

- **Accept**: The `accept` function waits for incoming client connections and returns a new socket object representing the client connection and the address of the client.
    ```cpp
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
    ```







