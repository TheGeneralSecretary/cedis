#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

typedef struct cedis_socket {
	unsigned short domain;
	int type;
	int protocol;
	unsigned int host;
	unsigned short port;
	int backlog;
} cedis_socket_t;

#endif
