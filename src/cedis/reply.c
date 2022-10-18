#include "cedis/reply.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <clog/clog.h>

char *reply_custom_message(const char *command, const char *message)
{
	const char *template = "command '': ";

	int len = (strlen(command) + strlen(message) + strlen(template) + 1);
	char *msg = malloc(len * sizeof(char));
	if (!msg) {
		perror("malloc");
		return NULL;
	}

	int written = snprintf(msg, len, "command '%s': %s", command, message);
	if (written <= 0 || written != len - 1) {
		CLOG_DEBUG(
			"reply_unknown_command snprintf: Written = %d, len = %ld",
			written, len - 1);
		return NULL;
	}

	return msg;
}

char *reply_unknown_command(const char *command)
{
	const char *template = "unknown command ''";

	int len = (strlen(command) + strlen(template) + 1);
	char *msg = malloc(len * sizeof(char));
	if (!msg) {
		perror("malloc");
		return NULL;
	}

	int written = snprintf(msg, len, "unknown command '%s'", command);
	if (written <= 0 || written != len - 1) {
		CLOG_DEBUG(
			"reply_unknown_command snprintf: Written = %d, len = %ld",
			written, len - 1);
		return NULL;
	}

	return msg;
}
