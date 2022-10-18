#ifndef _CEDIS_THREADPOOL_H_
#define _CEDIS_THREADPOOL_H_

#include <pthread.h>
#include "queue.h"

typedef struct cedis_threadpool {
	size_t nthreads;
	int active;
	pthread_t *pool;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	cedis_queue_t *worker;
} cedis_threadpool_t;

typedef struct cedis_threadtask {
	void *(*task)(void *arg);
	void *arg;
} cedis_threadtask_t;

cedis_threadpool_t *cedis_threadpool_init(size_t nthreads);
void cedis_threadpool_free(cedis_threadpool_t *threadpool);
cedis_threadtask_t *cedis_threadpool_createtask(void *(*fun)(void *arg),
						void *arg);
void cedis_threadpool_addtask(cedis_threadpool_t *threadpool,
			      cedis_threadtask_t *task);

#endif
