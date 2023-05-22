//
// Created by я on 21.05.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int sockfd;
    struct sockaddr_un server_addr;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket() failed\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "./dsock_file", sizeof(server_addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect() failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the UNIX Domain Server. Enter messages:\n");

    char input[128];
    ssize_t ret;

    while (1) {
        memset(input, 0, sizeof(input));
        fgets(input, sizeof(input), stdin);

        ret = write(sockfd, input, strlen(input));
        if (ret == -1 || ret == 0) {
            perror("write() failed\n");
            break;
        }

        ret = read(sockfd, input, sizeof(input));
        if (ret == -1 || ret == 0) {
            perror("read() failed\n");
            break;
        }

        printf("Server response: %s\n", input);
    }

    close(sockfd);

    return 0;
}

