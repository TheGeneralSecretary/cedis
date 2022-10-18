#include "cedis/threadpool.h"
#include <malloc.h>
#include <stdio.h>

/* Threads will execute this function */
static void *thread_function(void *arg)
{
	cedis_threadpool_t *threadpool = (cedis_threadpool_t *)arg;

	while (threadpool->active) {
		cedis_threadtask_t *task;
		pthread_mutex_lock(&threadpool->mutex);

		if (task = cedis_queue_pop(threadpool->worker) == NULL) {
			pthread_cond_wait(&threadpool->cond,
					  &threadpool->mutex);
			task = cedis_queue_pop(threadpool->worker);
		}

		pthread_mutex_unlock(&threadpool->mutex);

		if (task && task->task) {
			task->task(task->arg);
			free(task->arg);
			free(task);
		}
	}

	return NULL;
}

cedis_threadpool_t *cedis_threadpool_init(size_t nthreads)
{
	cedis_threadpool_t *threadpool = malloc(sizeof(cedis_threadpool_t));
	if (!threadpool) {
		perror("malloc");
		return NULL;
	}

	threadpool->nthreads = nthreads;
	threadpool->active = 1;
	threadpool->worker = cedis_queue_init();

	pthread_mutex_init(&threadpool->mutex, NULL);
	pthread_cond_init(&threadpool->cond, NULL);

	threadpool->pool = malloc(nthreads * sizeof(pthread_t));
	if (!threadpool->pool) {
		perror("malloc");
		return NULL;
	}

	for (size_t i = 0; i < nthreads; i++)
		pthread_create(&threadpool->pool[i], NULL, thread_function,
			       threadpool);

	return threadpool;
}

void cedis_threadpool_free(cedis_threadpool_t *threadpool)
{
	if (!threadpool)
		return;

	threadpool->active = 0;

	for (size_t i = 0; i < threadpool->nthreads; i++)
		pthread_cond_signal(&threadpool->cond);

	for (size_t i = 0; i < threadpool->nthreads; i++)
		pthread_join(threadpool->pool[i], NULL);

	if (threadpool->pool)
		free(threadpool->pool);

	cedis_queue_free(threadpool->worker);
	pthread_mutex_destroy(&threadpool->mutex);
	pthread_cond_destroy(&threadpool->cond);
	free(threadpool);
}

cedis_threadtask_t *cedis_threadpool_createtask(void *(*fun)(void *arg),
						void *arg)
{
	cedis_threadtask_t *task = malloc(sizeof(cedis_threadtask_t));
	if (!task) {
		perror("malloc");
		return NULL;
	}
	task->task = fun;
	task->arg = arg;
	return task;
}

void cedis_threadpool_addtask(cedis_threadpool_t *threadpool,
			      cedis_threadtask_t *task)
{
	pthread_mutex_lock(&threadpool->mutex);
	cedis_queue_push(threadpool->worker, task, sizeof(*task));
	pthread_cond_signal(&threadpool->cond);
	pthread_mutex_unlock(&threadpool->mutex);
}
