#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "include/server.h"

int main(int argc, char *argv[])
{
    int server_fd;
    struct sockaddr_in server_addr;

    // Create and configure the server socket.
    setup_server_socket(&server_fd, &server_addr, 0);

    while (1)
    {
        run_server(server_fd);
    }

    close(server_fd);

    return 0;
}