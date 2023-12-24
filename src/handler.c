#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <regex.h>
#include <sys/socket.h>
#include <unistd.h>

#include "include/server.h"
#include "include/handler.h"
#include "include/utils.h"

void *handle_client(void *arg)
{
    int client_fd = *((int *)arg);
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    // Receive request data from client and store into buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0)
    {
        // Check if request is GET
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex, buffer, 2, matches, 0) == 0)
        {
            // Extract filename from request and decode URL
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);

            // If file_name is empty, set it to "index.html"
            if (*file_name == '\0')
            {
                strcpy(file_name, "index.html");
            }

            // Get file extension
            char file_ext[32];
            strcpy(file_ext, get_file_extension(file_name));

            // Build HTTP response
            char *response = (char *)malloc(BUFFER_SIZE * sizeof(char));
            size_t response_len;
            build_http_response(file_name, file_ext, response, &response_len, "../../html");

            // Send HTTP response to client
            send(client_fd, response, response_len, 0);

            free(response);
            free(file_name);
        }
        regfree(&regex);
    }
    close(client_fd);
    free(arg);
    free(buffer);
    return NULL;
}

int build_http_response(const char *file_name,
                        const char *file_ext,
                        char *response,
                        size_t *response_len,
                        char *pathToFiles)
{
    // Response is 404 Not Found if file don't exist
    char path[128];
    snprintf(path, sizeof(path), "%s/%s", pathToFiles, file_name);
    int file_fd = open(path, O_RDONLY);
    if (file_fd == -1)
    {
        snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found");
        *response_len = strlen(response);
        return 404;
    }

    // Get file size for Content-Length
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;

    // Build HTTP header
    const char *mime_type = get_mime_type(file_ext);
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n",
             mime_type, (long)file_size);

    // Copy header to response buffer
    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len += strlen(header);

    // Copy file to response buffer
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd,
                              response + *response_len,
                              BUFFER_SIZE - *response_len)) > 0)
    {
        *response_len += bytes_read;
    }
    free(header);
    close(file_fd);
    return 200;
}