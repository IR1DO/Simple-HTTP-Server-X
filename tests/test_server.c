#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <regex.h>
#include <pthread.h>
#include <stddef.h>
#include <arpa/inet.h>

#include "Unity/src/unity.h"

#define PORT 8080
#define BUFFER_SIZE (5 * 1024 * 1024)

int setup_server_socket(int *server_fd, struct sockaddr_in *server_addr, int implicit);
int run_server(int server_fd);
int client(void);
void *handle_client(void *arg);
int build_http_response(const char *file_name, const char *file_ext, char *response, size_t *response_len, char *pathToFiles);
char *url_decode(const char *src);
const char *get_file_extension(const char *file_name);
const char *get_mime_type(const char *file_ext);

int server_fd;
struct sockaddr_in server_addr;

void setUp(void) {}

void tearDown(void) {}

void test_setup_server_socket(void)
{
    int server_fd;
    struct sockaddr_in server_addr;

    TEST_ASSERT_NOT_EQUAL_MESSAGE(
        -1, setup_server_socket(&server_fd, &server_addr, 1),
        "Failed to set up the server socket");

    close(server_fd);
}

void test_run_server(void)
{
    int sock;
    int server_fd;
    struct sockaddr_in server_addr;

    setup_server_socket(&server_fd, &server_addr, 1);

    sock = client();

    TEST_ASSERT_NOT_EQUAL_MESSAGE(
        -1, run_server(server_fd),
        "Failed to run the server");

    close(sock);
    close(server_fd);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_setup_server_socket);
    RUN_TEST(test_run_server);

    return UNITY_END();
}

int client(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    return sock;
}

void *handle_client(void *arg)
{
    int client_fd = *((int *)arg);
    close(client_fd);
    return NULL;
}

int build_http_response(const char *file_name,
                        const char *file_ext,
                        char *response,
                        size_t *response_len,
                        char *pathToFiles)
{
    return 0;
}