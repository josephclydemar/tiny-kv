#ifndef CLIENT_SOCKET_H_
#define CLIENT_SOCKET_H_

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct client_socket_t {
  int client_socket_fd;
  socklen_t client_addr_size;
  struct sockaddr_storage client_addr;
  union {
    char v4_buf[INET_ADDRSTRLEN];
    char v6_buf[INET6_ADDRSTRLEN];
  } ip;
} client_socket_t;

void client_socket_setup(client_socket_t* const client_socket);

#endif // CLIENT_SOCKET_H_

