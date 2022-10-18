#include "cedis/cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cedis/encoder.h"

cedis_command_res_t *create_response(int status, void *data)
{
	cedis_command_res_t *response = malloc(sizeof(cedis_command_res_t));
	if (!response) {
		perror("malloc");
		return NULL;
	}
	response->status = status;
	response->data = data;
	return response;
}

cedis_command_res_t *handle_command_command(cedis_command_t *command)
{
	return create_response(0, NULL);
}

cedis_command_res_t *handle_info_command(cedis_command_t *command)
{
	puts("Cedis is an Implementation of Redis Server");
	puts("Author: GeneralSecretary");
	puts("Project: https://github.com/TheGeneralSecretary/cedis");
	return create_response(0, NULL);
}

cedis_command_res_t *handle_ping_command(cedis_command_t *command)
{
	if (!command->args[0])
		return create_response(0, resp_simple_string_encode("PONG"));

	return create_response(0, resp_bulk_string_encode(command->args));
}

void cedis_command_dump(cedis_command_t *command)
{
	printf("COMMAND: %s\n", command->cmd);
	for (size_t i = 0; command->args[i] != NULL; i++)
		printf("ARG: %s\n", command->args[i]);
}

cedis_command_res_t *cedis_command_handle(cedis_command_t *command)
{
	size_t cmd = 0;
	const char *commands[] = { "command", "info", "ping" };

	for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		if (!strcmp(command->cmd, commands[i])) {
			cmd = i + 1;
			break;
		}
	}

	switch (cmd) {
	case 1:
		return handle_command_command(command);
	case 2:
		return handle_info_command(command);
	case 3:
		return handle_ping_command(command);
	default:
		return NULL;
	}

	return NULL;
}

void cedis_command_free(cedis_command_t *command)
{
	if (command) {
		free(command->cmd);
		if (command->args) {
			for (size_t i = 0; command->args[i] != NULL; i++)
				free(command->args[i]);
			free(command->args);
		}
		free(command);
	}
}

void cedis_command_res_free(cedis_command_res_t *command_res)
{
	if (command_res) {
		free(command_res->data);
		free(command_res);
	}
}
