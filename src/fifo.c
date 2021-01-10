#include <stdlib.h>
#include <stdio.h>
#include "fifo.h"

void queue_init(struct queue *q) {
  q->head = NULL;
  q->tail = NULL;
  pthread_mutex_init(&q->mutex, NULL);
}

void queue_destroy(struct queue *q) {
  pthread_mutex_destroy(&q->mutex);
}

int queue_push(struct queue *q, void *data) {
  pthread_mutex_lock(&q->mutex);
  struct queue_item *item = calloc(1, sizeof(struct queue_item));
  if (!item) {
    pthread_mutex_unlock(&q->mutex);
    return 1;
  }
  item->data = data;
  if (!q->head) {
    q->head = item;
    q->tail = item;
  } else {
    q->head->prev = item;
    q->head = item;
  }
  pthread_mutex_unlock(&q->mutex);
  return 0;
}

void *queue_pop(struct queue *q) {
  pthread_mutex_lock(&q->mutex);
  struct queue_item *tail = q->tail;
  if (!tail) {
    pthread_mutex_unlock(&q->mutex);
    return NULL;
  }
  q->tail = tail->prev;
  void *data = tail->data;
  free(tail);
  --q->nitems;
  pthread_mutex_unlock(&q->mutex);
  return data;
}

int queue_isempty(struct queue *q) {
  pthread_mutex_lock(&q->mutex);
  int isempty = q->tail == NULL;
  pthread_mutex_unlock(&q->mutex);
  return isempty;
}

void queue_print(struct queue *q) {
  struct queue_item *item = q->tail;
  while (item) {
    printf("\033[32mitem\033[m   ");
    item = item->prev;
  }
  putchar('\n');
}
