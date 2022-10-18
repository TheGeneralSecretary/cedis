#ifndef _SERVER_TCP_H_
#define _SERVER_TCP_H_

#include "socket.h"
#include <netinet/in.h>

typedef struct cedis_tcp_server {
	cedis_socket_t *sock;
	struct sockaddr_in *sa;
	int sockfd;
} tcp_server_t;

tcp_server_t *tcp_server_init(cedis_socket_t sockconf);
void tcp_server_free(tcp_server_t *server);

#endif
