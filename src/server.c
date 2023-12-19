#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <regex.h>
#include <pthread.h>

#include "include/server.h"
#include "include/handler.h"
#include "include/utils.h"

void setup_server_socket(int *server_fd, struct sockaddr_in *server_addr)
{
    // Create server socket
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Config socket
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);

    // Bind socket to port
    if (bind(*server_fd,
             (struct sockaddr *)server_addr,
             sizeof(*server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(*server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
}

void run_server(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));

    // Accept client connection
    if ((*client_fd = accept(server_fd,
                             (struct sockaddr *)&client_addr,
                             &client_addr_len)) < 0)
    {

        perror("accept failed");
        return;
    }

    // Create a new thread to handle client request
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
    pthread_detach(thread_id);
}