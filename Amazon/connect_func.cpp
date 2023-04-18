#include "connect_func.h"

int setup_server(const char * port) {

  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo * host_info_list;
  const char * hostname = NULL;

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags = AI_PASSIVE;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }
/* 應該可以刪掉
  if (strcmp(port, "") == 0) {
    struct sockaddr_in * addr_in = (struct sockaddr_in *)(host_info_list->ai_addr);
    addr_in->sin_port = 0;
  }
*/
  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }

  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }

  status = listen(socket_fd, 100);
  if (status == -1) {
    cerr << "Error: cannot listen on socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }

  return socket_fd;
}

int setup_client(const char * hostname, const char * port) {
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo * host_info_list;
  
  

  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;

    return -1;
  }

  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }

  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }
  freeaddrinfo(host_info_list);
  return socket_fd;
}

int accept_server(int socket_fd) {
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  int client_connect_fd;

  client_connect_fd =
      accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if (client_connect_fd == -1) {
    cerr << "Error: cannot accept connection on socket" << endl;
    return -1;
  }
  return client_connect_fd;
}