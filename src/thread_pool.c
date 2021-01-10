#include <stdlib.h>
#include "thread_pool.h"

struct fn_wrapper {
  void *(*func)(void *);
  void *args;
};

static void *worker(void *args) {
  struct queue *fifo = (struct queue *)args;
  for (;;) {
    if (!queue_isempty(fifo)) {
      struct fn_wrapper *wrap = (struct fn_wrapper *)queue_pop(fifo);
      wrap->func(wrap->args);
      free(wrap);
    }
    sleep(1);
  }
  return NULL;
}

int pool_init(struct ThreadPool *pool, size_t nthreads) {
  pool->threads = malloc(nthreads * sizeof(pthread_t));
  pool->nthreads = nthreads;
  queue_init(&pool->jobs);
  for (int i = 0; i < nthreads; ++i) {
    pthread_create(&pool->threads[i], NULL, worker, (void *)&pool->jobs);
  }
  return 0;
}

void pool_exec(struct ThreadPool *pool, void *(*func)(void *), void *args) {
  struct fn_wrapper *wrap = malloc(sizeof(struct fn_wrapper));
  wrap->func = func;
  wrap->args = args;
  queue_push(&pool->jobs, wrap);
}
