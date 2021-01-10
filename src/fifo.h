#ifndef _H_FIFO
#define _H_FIFO

#include <pthread.h>

struct queue_item {
  struct queue_item *prev;
  void *data;
};

struct queue {
  struct queue_item *head;
  struct queue_item *tail;
  pthread_mutex_t mutex;
  size_t nitems;
};

void queue_init(struct queue *q);
int queue_push(struct queue *q, void *data);
void *queue_pop(struct queue *q);
void queue_print(struct queue *q);
int queue_isempty(struct queue *q);
void queue_destroy(struct queue *q);

#endif // _H_FIFO
