#include "server/tcp.h"
#include <malloc.h>
#include <string.h>
#include <unistd.h>

tcp_server_t *tcp_server_init(cedis_socket_t sockconf)
{
	tcp_server_t *server = malloc(sizeof(tcp_server_t));
	if (!server) {
		perror("malloc");
		return NULL;
	}

	server->sock = malloc(sizeof(cedis_socket_t));
	if (!server->sock) {
		perror("malloc");
		return NULL;
	}

	memcpy(server->sock, &sockconf, sizeof(cedis_socket_t));

	server->sa = malloc(sizeof(struct sockaddr_in));
	if (!server->sa) {
		perror("malloc");
		return NULL;
	}

	server->sa->sin_family = sockconf.domain;
	server->sa->sin_port = htons(sockconf.port);
	server->sa->sin_addr.s_addr = htonl(sockconf.host);

	server->sockfd =
		socket(sockconf.domain, sockconf.type, sockconf.protocol);
	if (server->sockfd == -1) {
		perror("socket");
		return NULL;
	}

	int opt = 1;
	if (setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &(opt),
		       sizeof(opt)) == -1) {
		perror("setsockopt");
		return NULL;
	}

	if (bind(server->sockfd, (struct sockaddr *)server->sa,
		 sizeof(*server->sa)) < 0) {
		perror("bind");
		return NULL;
	}

	if (listen(server->sockfd, sockconf.backlog) == -1) {
		perror("listener");
		return NULL;
	}

	return server;
}

void tcp_server_free(tcp_server_t *server)
{
	if (server) {
		close(server->sockfd);
		free(server->sock);
		free(server->sa);
		free(server);
	}
}
