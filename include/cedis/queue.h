#ifndef _CEDIS_QUEUE_H_
#define _CEDIS_QUEUE_H_

#include <stddef.h>

typedef struct cedis_qnode {
	void *data;
	struct cedis_qnode *next;
} cedis_qnode_t;

typedef struct cedis_queue {
	cedis_qnode_t *front;
	cedis_qnode_t *rear;
} cedis_queue_t;

cedis_queue_t *cedis_queue_init(void);
void cedis_queue_push(cedis_queue_t *q, const void *data, size_t len);
void *cedis_queue_pop(cedis_queue_t *q);
void *cedis_queue_peek(cedis_queue_t *q);
void cedis_queue_free(cedis_queue_t *q);

#endif
