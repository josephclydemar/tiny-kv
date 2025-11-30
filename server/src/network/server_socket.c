#include "network/server_socket.h"
#include "util/debugging.h"
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>

#define PORT                  "5800"

void server_socket_setup(server_socket_t* const server_socket, size_t max_conn_backlog)
{
  server_socket->server_info = NULL;

  memset(&(server_socket->hints), 0, sizeof(struct addrinfo));
  server_socket->hints.ai_family = AF_UNSPEC;
  server_socket->hints.ai_socktype = SOCK_STREAM;
  server_socket->hints.ai_flags = AI_PASSIVE;


  ASSERT("GETADDRINFO",
    getaddrinfo(
      NULL,
      PORT,
      &server_socket->hints,
      &server_socket->server_info
    ) == 0
  );

  ASSERT("SERVER_INFO", server_socket->server_info != NULL);
  server_socket->server_socket_fd = socket(
    server_socket->server_info->ai_family,
    server_socket->server_info->ai_socktype,
    server_socket->server_info->ai_protocol
  );
  ASSERT("SOCKET", server_socket->server_socket_fd > 0);

  ASSERT("SETSOCKOPT",
    setsockopt(
      server_socket->server_socket_fd,
      SOL_SOCKET,
      SO_REUSEADDR,
      &server_socket->optval,
      sizeof(int)
    ) == 0
  );
  ASSERT("BIND",
    bind(
      server_socket->server_socket_fd,
      server_socket->server_info->ai_addr,
      server_socket->server_info->ai_addrlen
    ) == 0
  );

  ASSERT("LISTEN",
    listen(server_socket->server_socket_fd, max_conn_backlog) == 0
  );

}
