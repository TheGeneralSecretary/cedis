#include "cedis/request.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cedis/util.h"

cedis_request_t *cedis_request_parse(const char *data)
{
	cedis_request_t *req = malloc(sizeof(cedis_request_t));
	if (!req) {
		perror("malloc");
		return NULL;
	}
	req->command = NULL;

	size_t i = 0, curr_arg = 0;

	while (data[i] != '\0') {
		if (data[i] == '*') {
			int len = atoi(&data[++i]);
			req->command = malloc(sizeof(cedis_command_t));
			req->command->cmd = NULL;
			req->command->args = calloc((len + 1), sizeof(char *));

			while (data[i++] != '\n')
				;

			continue;
		}

		if (data[i] == '$') {
			int len = atoi(&data[++i]);
			while (data[i++] != '\n')
				;

			char *content = calloc((len + 1), sizeof(char));
			strncpy(content, data + i, len);

			if (!req->command->cmd)
				req->command->cmd = toLower(strdup(content));
			else
				req->command->args[curr_arg++] =
					strdup(content);

			free(content);

			while (data[i++] != '\n')
				;

			continue;
		}
		i++;
	}

	return req;
}

void cedis_request_free(cedis_request_t *request)
{
	if (request) {
		cedis_command_free(request->command);
		free(request);
	}
}
