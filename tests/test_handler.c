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
#include <ctype.h>

#include "../Unity/src/unity.h"

#define PORT 8080
#define BUFFER_SIZE (5 * 1024 * 1024)

int setup_server_socket(int *server_fd, struct sockaddr_in *server_addr, int implicit);
int run_server(void *arg);
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

void test_handle_client(void)
{
    int err;
    int server_fd;
    struct sockaddr_in server_addr;

    setup_server_socket(&server_fd, &server_addr, 1);

    pthread_t server_thread;
    pthread_create(&server_thread, NULL, (void *(*)(void *))run_server, &server_fd);

    err = client();

    pthread_join(server_thread, NULL);

    TEST_ASSERT_NOT_EQUAL_MESSAGE(
        -1, err, "Failed to handle client");

    close(server_fd);
}

void test_build_response(void)
{
    char *response = (char *)malloc(BUFFER_SIZE * sizeof(char));
    size_t response_len;
    char *pathToFiles = "html";

    TEST_ASSERT_EQUAL_INT(200, build_http_response("index.html", "html", response, &response_len, pathToFiles));
    TEST_ASSERT_NOT_EQUAL(0, response_len);

    TEST_ASSERT_EQUAL_INT(404, build_http_response("notexist.jpg", "jpg", response, &response_len, pathToFiles));
    TEST_ASSERT_NOT_EQUAL(0, response_len);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_handle_client);
    RUN_TEST(test_build_response);

    return UNITY_END();
}

int client(void)
{
    int err;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    const char *http_request = "GET /index.html HTTP/1.1\r\n"
                               "Host: localhost\r\n"
                               "\r\n";

    send(sock, http_request, strlen(http_request), 0);

    err = recv(sock, buffer, BUFFER_SIZE, 0);

    close(sock);

    return err;
}

int setup_server_socket(int *server_fd, struct sockaddr_in *server_addr, int implicit)
{
    // Create server socket
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        return -1;
    }

    // Add SO_REUSEADDR to the socket config
    int reuse = 1;
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    {
        perror("Setsockopt failed");
        close(*server_fd);
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
        return -1;
    }

    // Listen for connections
    if (listen(*server_fd, 10) < 0)
    {
        perror("listen failed");
        return -1;
    }

    if (!implicit)
    {
        printf("Server listening on port %d\n", PORT);
    }

    return 0;
}

int run_server(void *arg)
{
    int server_fd = *((int *)arg);
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));

    // Accept client connection
    if ((*client_fd = accept(server_fd,
                             (struct sockaddr *)&client_addr,
                             &client_addr_len)) < 0)
    {

        perror("accept failed");
        return -1;
    }

    // Create a new thread to handle client request
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
    pthread_detach(thread_id);

    return 0;
}

char *url_decode(const char *src)
{
    size_t src_len = strlen(src);
    char *decoded = malloc(src_len + 1);
    size_t decoded_len = 0;

    // Decode %2x to hex
    for (size_t i = 0; i < src_len; i++)
    {
        if (src[i] == '%' && i + 2 < src_len)
        {
            int hex_val;
            // Parse the two characters after % as hexadecimal numbers.
            sscanf(src + i + 1, "%2x", &hex_val);
            decoded[decoded_len] = hex_val;
            decoded_len++;
            i += 2;
        }
        else
        {
            decoded[decoded_len] = src[i];
            decoded_len++;
        }
    }

    // Add null terminator
    decoded[decoded_len] = '\0';
    return decoded;
}

const char *get_file_extension(const char *file_name)
{
    const char *dot = strrchr(file_name, '.');
    if (!dot || dot == file_name)
    {
        return "";
    }
    return dot + 1;
}

const char *get_mime_type(const char *file_ext)
{
    if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0)
    {
        return "text/html";
    }
    else if (strcasecmp(file_ext, "txt") == 0)
    {
        return "text/plain";
    }
    else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcasecmp(file_ext, "png") == 0)
    {
        return "image/png";
    }
    else if (strcasecmp(file_ext, "css") == 0)
    {
        return "text/css";
    }
    else if (strcasecmp(file_ext, "js") == 0)
    {
        return "application/javascript";
    }
    else
    {
        return "application/octet-stream";
    }
}