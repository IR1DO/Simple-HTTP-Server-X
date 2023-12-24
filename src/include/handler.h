#ifndef HANDLER_H
#define HANDLER_H

#include <stddef.h>

void *handle_client(void *arg);
int build_http_response(const char *file_name, const char *file_ext, char *response, size_t *response_len, char *pathToFiles);

#endif
