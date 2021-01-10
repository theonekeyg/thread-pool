#include <stdio.h>
#include <unistd.h>
#include "thread_pool.h"
#include "fifo.h"

void* foo(void *args) {
  printf("foo\n");
  sleep(2);
  printf("=== DONE ===\n");
}

int
main() {
  struct ThreadPool pool;
  pool_init(&pool, 4);

  char s[100];
  while (fgets(s, 100, stdin) != NULL) {
    pool_exec(&pool, foo, NULL);
    queue_print(&pool.jobs);
  }
  return 0;
}
