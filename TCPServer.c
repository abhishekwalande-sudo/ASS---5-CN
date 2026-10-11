
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
    int sockfd, connfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Binding socket
    if (bind(sockfd, (struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listening for client
    if (listen(sockfd, 5) < 0) {
        perror("listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for client...\n");

    // Accepting client connection
    len = sizeof(cliaddr);
    connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);

    if (connfd < 0) {
        perror("accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receiving message from client
    int n = (int)recv(connfd, buffer, MAXLINE - 1, 0);

    if (n < 0) {
        perror("recv failed");
        close(connfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    // Sending response to client
    send(connfd, hello, strlen(hello), 0);
    printf("Hello message sent.\n");

    close(connfd);
    close(sockfd);

    return 0;
}
