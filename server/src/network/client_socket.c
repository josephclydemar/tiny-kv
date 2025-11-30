#include "network/client_socket.h"

void client_socket_setup(client_socket_t* const client_socket)
{
  client_socket->client_socket_fd = 0;
  client_socket->client_addr_size = sizeof(struct sockaddr_storage);
}

