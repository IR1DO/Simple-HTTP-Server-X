#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE (5 * 1024 * 1024)

int setup_server_socket(int *server_fd, struct sockaddr_in *server_addr, int implicit);
int run_server(int server_fd);

#endif