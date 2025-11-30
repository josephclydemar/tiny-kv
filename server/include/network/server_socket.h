#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct server_socket_t {
  int server_socket_fd, optval;
  struct addrinfo hints;
  struct addrinfo* server_info;
} server_socket_t;

void server_socket_setup(server_socket_t* const server_socket, size_t max_conn_backlog);

#endif // SERVER_SOCKET_H_

