#include <stdlib.h>
#include "thread_pool.h"

struct pool_signal {
  int type;
  union {
    struct {
      void *(*func)(void *);
      void *args;
    };
  };
};

void *worker(void *args) {
  struct queue *fifo = (struct queue *)args;
  for (;;) {
    if (!queue_isempty(fifo)) {
      struct pool_signal *wrap = (struct pool_signal *)queue_pop(fifo);
      queue_print(fifo);
      if (wrap->type == TSIG_NEW) {
        wrap->func(wrap->args);
      } else if (wrap->type == TSIG_TERM) {
        free(wrap);
        break;
      }
      if (wrap->args) {
        free(wrap->args);
      }
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

void pool_free(struct ThreadPool *pool) {
  int i;

  /* Iterate over all threads to make sure each one receives it's own
   * TSIG_TERM signal. Otherwise permanent lock is possible in the
   * pthread_join call */
  for (i = 0; i < pool->nthreads; ++i) {
    struct pool_signal *wrap = malloc(sizeof(struct pool_signal));
    wrap->type = TSIG_TERM;
    queue_push(&pool->jobs, wrap);
  }

  for (i = 0; i < pool->nthreads; ++i) {
    pthread_join(pool->threads[i], NULL);
  }
  free(pool->threads);
}

/*
 * Put new task to be executed in the pool
 */
void pool_exec(struct ThreadPool *pool, void *(*func)(void *), void *args) {
  struct pool_signal *wrap = malloc(sizeof(struct pool_signal));
  wrap->type = TSIG_NEW;
  wrap->func = func;
  wrap->args = args;
  queue_push(&pool->jobs, wrap);
}
