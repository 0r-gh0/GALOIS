#include <string.h>
#include "final.h"
//#include "gcm.h"
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX 1024
#define SERVER_IP "192.168.155.86"
int client_fd;

//For error handling
void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void *receive_messages(void *arg) {
    char buffer[MAX];
    int valread;
    while (1) {
        memset(buffer, 0, MAX);
        // Read message from server
        valread = read(client_fd, buffer, MAX);
        if (valread <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        printf("Server: %s", buffer);
    }
    pthread_exit(NULL);
}

void *send_messages(void *arg) {
    char message[MAX];
    int valsend;
    while (1) {
        fgets(message, MAX, stdin); // Get client message to send to the server
        valsend = send(client_fd, message, strlen(message), 0);// Send message to server
        if (valsend <= 0) break;
    }
    pthread_exit(NULL);
}

int main() {
    struct sockaddr_in server_addr;
    pthread_t send_thread, receive_thread;

    // Socket creation
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("Unable to create socket");

    // Configuring server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if(connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
        error("Connection failed");
    
    printf("Connected to the server.\n");
    printf("====== Welcome ======\n");

    char s_key[65], p_key[65];
    public_key_generation(s_key, p_key);
    char server_p_key[65] = {0};
    read(client_fd, server_p_key, 64);
    send(client_fd, p_key, strlen(p_key), 0);
    u8 aes_key[16];
    shared_key_generation(s_key, server_p_key, aes_key);

    // Create threads for sending and receiving messages
    pthread_create(&send_thread, NULL, send_messages, NULL);
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    // Wait for both threads to finish
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    // Close the socket
    close(client_fd);
    printf("Disconnected from server\n");
    return 0;
}
