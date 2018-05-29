#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PriorityQueue.h"

/*Private Functions*/
static void priorityQueueRealloc(priorityqueue_t *pq);
static void priorityQueueAdjustHead(priorityqueue_t *pq);
static void priorityQueueAdjustTail(priorityqueue_t *pq);
static int priorityQueueCompare(priorityqueue_t *pq, int pos1, int pos2);
static void priorityQueueSwap(map_t **nodes, int pos1, int pos2);

/*Functions of KeyValue Struct*/
map_t *newMap(int key, void *value)
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    map->key = key;
    map->value = value;
    return map;
}
void mapValueFree(map_t *map, void (*freevalue)(void *))
{
    if (map)
    {
        if (freevalue)
        {
            freevalue(map->value);
        }
        free(map);
    }
}

/*Functions of PriorityQueue Struct*/
priorityqueue_t *priorityQueueNew(int priority)
{
    priorityqueue_t *pq = (priorityqueue_t *)malloc(sizeof(priorityqueue_t));
    pq->capacity = 11; /*default initial value*/
    pq->size = 0;
    pq->priority = priority;

    pq->nodesp = (map_t **)malloc(sizeof(map_t *) * pq->capacity);
    return pq;
}

void priorityQueueFree(priorityqueue_t *pq, void (*freevalue)(void *))
{
    int i;
    if (pq)
    {
        for (i = 0; i < pq->size; ++i)
        {
            mapValueFree(pq->nodesp[i], freevalue);
        }
        free(pq->nodesp);
        free(pq);
    }
}

const map_t *priorityQueueTop(priorityqueue_t *priorityQueuep)
{
    if (priorityQueuep->size > 0)
    {
        return priorityQueuep->nodesp[0];
    }
    return NULL;
}

map_t *priorityQueueDequeue(priorityqueue_t *priorityQueuep)
{
    map_t *mapp = NULL;
    if (priorityQueuep->size > 0)
    {
        mapp = priorityQueuep->nodesp[0];
        priorityQueueAdjustHead(priorityQueuep);
    }
    return mapp;
}

void priorityQueueEnqueue(priorityqueue_t *pqp, map_t *mapp)
{
    printf("add key:%d\n", mapp->key);
    pqp->nodesp[pqp->size] = mapp;
    priorityQueueAdjustTail(pqp);
    if (pqp->size >= pqp->capacity)
    {
        priorityQueueRealloc(pqp);
    }
}

int priorityQueueSize(priorityqueue_t *pqp)
{
    return pqp->size;
}

int priorityQueueEmpty(priorityqueue_t *pqp)
{
    return pqp->size <= 0;
}

void priorityQueuePrint(priorityqueue_t *pqp)
{
    int i;
    map_t *mapp;
    printf("data in the pq->_nodes\n");
    for (i = 0; i < pqp->size; ++i)
    {
        printf("%d ", pqp->nodesp[i]->key);
    }
    printf("\n");

    printf("dequeue all data\n");
    while (!priorityQueueEmpty(pqp))
    {
        mapp = priorityQueueDequeue(pqp);
        printf("%d ", mapp->key);
    }
    printf("\n");
}

static void priorityQueueRealloc(priorityqueue_t *pqp)
{
    pqp->capacity = pqp->capacity * 2;
    pqp->nodesp = realloc(pqp->nodesp, sizeof(map_t *) * pqp->capacity);
}

static void priorityQueueAdjustHead(priorityqueue_t *pqp)
{
    int i, j, parent, left, right;

    i = 0, j = 0;
    parent = left = right = 0;
    priorityQueueSwap(pqp->nodesp, 0, pqp->size - 1);
    pqp->size--;
    while (i < (pqp->size - 1) / 2)
    {
        parent = i;

        left = i * 2 + 1;
        right = left + 1;
        j = left;
        if (priorityQueueCompare(pqp, left, right) > 0)
        {
            j++;
        }
        if (priorityQueueCompare(pqp, parent, j) > 0)
        {
            priorityQueueSwap(pqp->nodesp, i, j);
            i = j;
        }
        else
        {
            break;
        }
    }
}

static void priorityQueueAdjustTail(priorityqueue_t *pqp)
{
    int i, parent, child;

    i = pqp->size - 1;
    pqp->size++;
    while (i > 0)
    {
        child = i;
        parent = (child - 1) / 2;

        if (priorityQueueCompare(pqp, parent, child) > 0)
        {
            priorityQueueSwap(pqp->nodesp, child, parent);
            i = parent;
        }
        else
        {
            break;
        }
    }
}

static int priorityQueueCompare(priorityqueue_t *pqp, int pos1, int pos2)
{
    int adjust = -1;
    int r = pqp->nodesp[pos1]->key - pqp->nodesp[pos2]->key;
    if (pqp->priority == PRIORITY_MAX)
    {
        r *= adjust;
    }
    return r;
}

static void priorityQueueSwap(map_t **nodesp, int pos1, int pos2)
{
    map_t *temp = nodesp[pos1];
    nodesp[pos1] = nodesp[pos2];
    nodesp[pos2] = temp;
}
