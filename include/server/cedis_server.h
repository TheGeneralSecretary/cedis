#ifndef _SERVER_CEDIS_H_
#define _SERVER_CEDIS_H_

#include "tcp.h"
#include "cedis/threadpool.h"

typedef struct cedis_server {
	tcp_server_t *tcp_server;
	cedis_threadpool_t *threadpool;
} cedis_server_t;

cedis_server_t *cedis_server_init(void);
void cedis_server_free(cedis_server_t *server);
int cedis_server_run(cedis_server_t *server, size_t nthreads);

#endif
