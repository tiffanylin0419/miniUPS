#include "head.h"

int setup_server(const char * port);
int setup_client(const char * hostname, const char * port);
int accept_server(int socket_fd);