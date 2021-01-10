#ifndef _H_THREAD_POOL
#define _H_THREAD_POOL

#include <pthread.h>
#include <unistd.h>
#include "fifo.h"

struct ThreadPool {
  pthread_t *threads;
  size_t nthreads;
  struct queue jobs;
};

int pool_init(struct ThreadPool *pool, size_t nthreads);
void pool_exec(struct ThreadPool *pool, void *(*func)(void *), void *args);

#endif // _H_THREAD_POOL
