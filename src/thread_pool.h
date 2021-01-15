#ifndef _H_THREAD_POOL
#define _H_THREAD_POOL

#include <pthread.h>
#include <unistd.h>
#include "fifo.h"

enum PoolSignals {
  TSIG_NEW,  /* Signal to indicate task in the queue. */
  TSIG_TERM  /* Signal to terminate whichever thread was first to get this signal. */
};

struct ThreadPool {
  pthread_t *threads;
  size_t nthreads;
  struct queue jobs; /* FIFO queue to store tasks for threads */
};

int pool_init(struct ThreadPool *pool, size_t nthreads);
void pool_free(struct ThreadPool *pool);
void pool_exec(struct ThreadPool *pool, void *(*func)(void *), void *args);

#endif // _H_THREAD_POOL
