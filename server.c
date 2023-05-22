#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int sockfd;
    int clt_sock;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket() failed\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "socket_path", sizeof(server_addr.sun_path) - 1);
    strcpy(server_addr.sun_path, "./dsock_file");

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind() failed\n");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen() failed\n");
        exit(EXIT_FAILURE);
    }

    printf("UNIX Domain Server started.\n");

    while (1) {
        client_addr_len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(struct sockaddr_un));
        clt_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (clt_sock == -1) {
            perror("accept() failed\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        int pid = fork();

        if (pid == -1) {
            perror("fork() failed\n");
            close(clt_sock);
            close(sockfd);
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            break;
        }

        close(clt_sock);
    }

    close(sockfd);

    while (1) {
        int ret;
        char buff[128];

        ret = read(clt_sock, buff, sizeof(buff));
        if (ret == -1 || ret == 0) {
            printf("stopping\n");
            break;
        }

        ret = write(clt_sock, buff, ret);
        if (ret == -1 || ret == 0) {
            printf("stopping\n");
            break;
        }
    }

    close(clt_sock);
    unlink(server_addr.sun_path);

    return 0;
}
