#include "server/cedis_server.h"
#include <malloc.h>
#include <clog/clog.h>
#include <unistd.h>
#include "cedis/request.h"
#include "cedis/cmd.h"
#include <string.h>
#include "cedis/encoder.h"
#include "cedis/reply.h"

typedef struct cedis_handle_args {
	int clifd;
} cedis_handle_args_t;

void *cedis_handle_concurrent(void *data)
{
	cedis_handle_args_t *args = (cedis_handle_args_t *)data;
	int clifd = args->clifd;
	printf("THREAD CLIENT: %d\n", clifd);

	char buf[1024] = { 0 };
	ssize_t r = read(clifd, buf, sizeof(buf));
	if (r == -1) {
		perror("read");
		close(clifd);
		return NULL;
	}

	cedis_request_t *request = cedis_request_parse(buf);
	if (!request) {
		CLOG_ERROR("failed to parse cedis request");
		close(clifd);
		return NULL;
	}

	if (!request->command || !request->command->cmd) {
		// CLOG_ERROR("no command provided");
		close(clifd);
		return NULL;
	}

	// cedis_command_dump(request->command);

	cedis_command_res_t *res = cedis_command_handle(request->command);
	if (!res) {
		char *msg = reply_unknown_command(request->command->cmd);
		char *encoded = resp_error_encode(msg);

		if (write(clifd, encoded, strlen(encoded)) == -1)
			perror("write");

		free(msg);
		free(encoded);
	} else if (res->status != 0) {
		char *msg = reply_custom_message(request->command->cmd,
						 "execution failed");
		char *encoded = resp_error_encode(msg);

		if (write(clifd, encoded, strlen(encoded)) == -1)
			perror("write");

		free(msg);
		free(encoded);
	}

	if (res && res->status == 0 && res->data) {
		if (write(clifd, res->data, strlen(res->data)) == -1)
			perror("write");
	}

	close(clifd);
	cedis_request_free(request);
	cedis_command_res_free(res);
	return NULL;
}

cedis_server_t *cedis_server_init(void)
{
	cedis_server_t *cedis = malloc(sizeof(cedis_server_t));
	if (!cedis) {
		perror("malloc");
		return NULL;
	}

	cedis_socket_t sockconf = { .domain = AF_INET,
				    .type = SOCK_STREAM,
				    .protocol = 0,
				    .host = INADDR_ANY,
				    .port = 6379,
				    .backlog = 255 };

	cedis->tcp_server = tcp_server_init(sockconf);
	if (!cedis->tcp_server) {
		CLOG_FATAL("failed to init tcp server");
		return NULL;
	}

	cedis->threadpool = NULL;
	return cedis;
}

void cedis_server_free(cedis_server_t *server)
{
	if (server) {
		tcp_server_free(server->tcp_server);
		free(server);
	}
}

int cedis_server_run(cedis_server_t *server, size_t nthreads)
{
	server->threadpool = cedis_threadpool_init(nthreads);
	if (!server->threadpool) {
		printf("failed to init threadpool");
		return -1;
	}

	struct sockaddr *sa = (struct sockaddr *)server->tcp_server->sa;
	socklen_t addr_len = (socklen_t)sizeof(server->tcp_server->sa);

	CLOG_INFO("Cedis Running on Port: %d", server->tcp_server->sock->port);

	while (1) {
		int clifd = accept(server->tcp_server->sockfd, sa, &addr_len);
		if (clifd == -1) {
			perror("accept");
			continue;
		}

		cedis_handle_args_t *args = malloc(sizeof(cedis_handle_args_t));
		if (!args) {
			perror("malloc");
			continue;
		}
		args->clifd = clifd;

		cedis_threadtask_t *task = cedis_threadpool_createtask(
			cedis_handle_concurrent, (void *)args);
		cedis_threadpool_addtask(server->threadpool, task);
		free(task);
	}

	if (shutdown(server->tcp_server->sockfd, SHUT_RDWR) == -1)
		perror("shutdown");

	return 0;
}
