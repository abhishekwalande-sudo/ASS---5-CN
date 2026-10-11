// UDP Chat Server

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(cliaddr);

    printf("Server is waiting for messages...\n");

    while (1) {

        // Receive message from client
        int n = recvfrom(sockfd, buffer, MAXLINE - 1,
                         0, (struct sockaddr *)&cliaddr, &len);

        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        // Check for bye
        if (strcmp(buffer, "bye") == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Enter server reply
        printf("Server: ");
        fgets(buffer, MAXLINE, stdin);

        // Remove newline
        buffer[strcspn(buffer, "\n")] = '\0';

        // Send reply
        sendto(sockfd, buffer, strlen(buffer),
               0, (const struct sockaddr *)&cliaddr, len);

        // Check for bye
        if (strcmp(buffer, "bye") == 0) {
            printf("Chat ended.\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}