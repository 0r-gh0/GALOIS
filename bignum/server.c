#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "final.h"

#define PORT 8888
#define MAX 1024
int new_socket;

void *receive_messages(void *arg) {
    char buffer[MAX];
    int valread;
    while (1) {
        memset(buffer, 0, MAX);
        // Read message from client
        valread = read(new_socket, buffer, MAX);
        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s", buffer);
    }
    pthread_exit(NULL);
}

void *send_messages(void *arg) {
    char message[MAX];
    while (1) {
        // Get server message to send to the client
        fgets(message, MAX, stdin);
        // Send message to client
        send(new_socket, message, strlen(message), 0);
    }
    pthread_exit(NULL);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t send_thread, receive_thread;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Define address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");
    char s_key[65], p_key[65];
    public_key_generation(s_key, p_key);

    send(new_socket, p_key, strlen(p_key), 0);

    char client_p_key[65] = {0};
    read(new_socket, client_p_key, 64);
    u8 aes_key[16];
    shared_key_generation(s_key, client_p_key, aes_key);

// Create threads for sending and receiving messages
    pthread_create(&send_thread, NULL, send_messages, NULL);
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    // Wait for both threads to finish
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    // Close the socket
    close(new_socket);
    close(server_fd);

    return 0;
}
