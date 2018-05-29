#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

/* =============KeyValue Struct================================== */
typedef struct Map
{
    int key;
    void *value;
} map_t;

/* =============PriorityQueue Struct============================== */
#define PRIORITY_MAX 1
#define PRIORITY_MIN 2
typedef struct PriorityQueue
{
    map_t **nodesp;
    int size;
    int capacity;
    int priority;
} priorityqueue_t;

map_t *newMap(int key, void *value);
void mapValueFree(map_t *kv, void (*freevalue)(void *));

priorityqueue_t *priorityQueueNew(int priority);
void priorityQueueFree(priorityqueue_t *pq, void (*freevalue)(void *));
const map_t *priorityQueueTop(priorityqueue_t *pq);
map_t *priorityQueueDequeue(priorityqueue_t *pq);
void priorityQueueEnqueue(priorityqueue_t *pq, map_t *kv);
int priorityQueueSize(priorityqueue_t *pq);
int priorityQueueEmpty(priorityqueue_t *pq);
void priorityQueuePrint(priorityqueue_t *pq);

#endif
