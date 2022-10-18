#include "cedis/queue.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

static cedis_qnode_t *cedis_create_node(void *data)
{
	cedis_qnode_t *node = malloc(sizeof(cedis_qnode_t));
	if (!node) {
		perror("malloc");
		return NULL;
	}
	node->data = data;
	node->next = NULL;
	return node;
}

cedis_queue_t *cedis_queue_init(void)
{
	cedis_queue_t *q = malloc(sizeof(cedis_queue_t));
	if (!q) {
		perror("malloc");
		return NULL;
	}
	q->front = q->rear = NULL;
	return q;
}

void cedis_queue_push(cedis_queue_t *q, const void *data, size_t len)
{
	void *tmp = malloc(len);
	if (!tmp) {
		perror("malloc");
		return;
	}
	memcpy(tmp, data, len);

	cedis_qnode_t *node = cedis_create_node(tmp);

	if (!q->rear) {
		q->front = q->rear = node;
		return;
	}

	q->rear->next = node;
	q->rear = node;
}

void *cedis_queue_pop(cedis_queue_t *q)
{
	if (!q->front)
		return NULL;

	cedis_qnode_t *node = q->front;
	q->front = q->front->next;

	if (!q->front)
		q->rear = NULL;

	void *data = node->data;
	free(node);
	return data;
}

void *cedis_queue_peek(cedis_queue_t *q)
{
	if (!q->front)
		return NULL;

	return q->front->data;
}

void cedis_queue_free(cedis_queue_t *q)
{
	if (q) {
		if (!q->front) {
			free(q);
			return;
		}

		void *data = cedis_queue_pop(q);
		while (data) {
			free(data);
			data = cedis_queue_pop(q);
		}

		free(q);
	}
}
