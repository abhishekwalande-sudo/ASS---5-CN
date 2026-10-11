// UDP Chat Client

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
    struct sockaddr_in servaddr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t len = sizeof(servaddr);

    printf("Client started. Type your message:\n");

    while (1) {

        // Enter client message
        printf("Client: ");
        fgets(buffer, MAXLINE, stdin);

        // Remove newline
        buffer[strcspn(buffer, "\n")] = '\0';

        // Send message to server
        sendto(sockfd, buffer, strlen(buffer),
               0, (const struct sockaddr *)&servaddr,
               sizeof(servaddr));

        // Check for bye
        if (strcmp(buffer, "bye") == 0) {
            printf("Chat ended.\n");
            break;
        }

        // Receive server response
        int n = recvfrom(sockfd, buffer, MAXLINE - 1,
                         0, (struct sockaddr *)&servaddr, &len);

        buffer[n] = '\0';

        printf("Server: %s\n", buffer);

        // Check for bye
        if (strcmp(buffer, "bye") == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}
