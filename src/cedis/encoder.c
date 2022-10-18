#include "cedis/encoder.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <clog/clog.h>

char *_getarraycontent(const char **data)
{
	size_t len = 1;
	for (int i = 0; data[i] != NULL; i++) {
		len += strlen(data[i]);
		if (data[i + 1] != NULL)
			len++;
	}

	char *content = calloc(len, sizeof(char));
	if (!content) {
		perror("calloc");
		return NULL;
	}

	int written = 0;
	for (int i = 0; data[i] != NULL; i++) {
		char *fmt = (data[i + 1] != NULL ? "%s " : "%s");
		written +=
			snprintf(content + strlen(content), len, fmt, data[i]);
	}

	if (written <= 0 || written != len - 1) {
		CLOG_DEBUG("_getarraycontent snprintf: Written = %d, len = %ld",
			   written, len - 1);
		return NULL;
	}

	return content;
}

char *resp_simple_string_encode(const char *data)
{
	size_t len = strlen(data) + 4;
	char *encoded = malloc(len * sizeof(char));
	if (!encoded) {
		perror("malloc");
		return NULL;
	}

	int written = snprintf(encoded, len, "+%s\r\n", data);
	if (written <= 0 || written != len - 1) {
		CLOG_DEBUG(
			"resp_simple_string_encode snprintf: Written = %d, len = %ld",
			written, len - 1);
		return NULL;
	}

	return encoded;
}

char *resp_bulk_string_encode(const char **data)
{
	char *content = _getarraycontent(data);
	if (!content)
		return NULL;

	int content_lenstr_len = snprintf(NULL, 0, "%ld", strlen(content));
	int len = strlen(content) + content_lenstr_len + 6;

	char *encoded = malloc(len * sizeof(char));
	if (!encoded) {
		perror("malloc");
		return NULL;
	}

	int written = snprintf(encoded, len, "$%ld\r\n%s\r\n", strlen(content),
			       content);
	if (written <= 0 || written != len - 1) {
		CLOG_DEBUG(
			"resp_bulk_string_encode snprintf: Written = %d, len = %ld",
			written, len - 1);
		return NULL;
	}

	free(content);
	return encoded;
}

char *resp_error_encode(const char *data)
{
	size_t len = strlen(data) + 4;
	char *encoded = malloc(len * sizeof(char));
	if (!encoded) {
		perror("malloc");
		return NULL;
	}

	int written = snprintf(encoded, len, "-%s\r\n", data);
	if (written <= 0 || written != len - 1) {
		CLOG_DEBUG(
			"resp_simple_string_encode snprintf: Written = %d, len = %ld",
			written, len - 1);
		return NULL;
	}

	return encoded;
}
