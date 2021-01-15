#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "thread_pool.h"
#include "fifo.h"

void* foo(void *args) {
  printf("foo : %ld\n", *(int64_t *)args);
  usleep(0.5);
  printf("=== DONE ===\n");
  return NULL;
}

int
main() {
  struct ThreadPool pool;
  struct queue q;
  int64_t i = 0;
  queue_init(&q);
  pool_init(&pool, 4);

  char s[100];
  while (fgets(s, 100, stdin) != NULL) {
    int64_t *num = malloc(sizeof(int64_t));
    *num = i++;
    pool_exec(&pool, foo, num);
    queue_print(&pool.jobs);
  }
  pool_free(&pool);
  return 0;
}
