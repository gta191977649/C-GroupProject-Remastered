#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PriorityQueue.h"
#include "Common.h"
/*Private Functions*/
static void priorityQueueRealloc(priorityqueue_t *pq);
static void priorityQueueAdjustHead(priorityqueue_t *pq);
static void priorityQueueAdjustTail(priorityqueue_t *pq);
static int priorityQueueCompare(priorityqueue_t *pq, int pos1, int pos2);
static void priorityQueueSwap(map_t **nodes, int pos1, int pos2);


/*****************************************************************************
 newMap
 This function is creating a new map 
 inputs:
  - key: the unique key value function like id
  - value: a hashmap to pair with key
 output:
  - a map with key and value
 *****************************************************************************/
map_t *newMap(int key, void *value)
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    map->key = key;
    map->value = value;
    return map;
}
/*****************************************************************************
 mapValueFree
 This function is to free the map from memory
 inputs:
  - map: a potiner to the map struct contain key, and keyvalue
  - freevalue: function pointer to free from memory
 output:
  - none
 *****************************************************************************/
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
/******************************************************************************
 priorityQueueNew
 This function is create new priority queue
 inputs:
  - priority: int datetype use to show priority 
 output:
  - priorityqueue with nodesp, size, capacity and priority
 *****************************************************************************/
priorityqueue_t *priorityQueueNew(int priority)
{
    priorityqueue_t *pq = (priorityqueue_t *)malloc(sizeof(priorityqueue_t));
    pq->capacity = 11; /*default initial value*/
    pq->size = 0;
    pq->priority = priority;

    pq->nodesp = (map_t **)malloc(sizeof(map_t *) * pq->capacity);
    return pq;
}
/******************************************************************************
 priorityQueueFree
 This function is free priority queue from memory
 inputs:
  - pq: priorityqueue struct with nodesp, size, capacity and priority
  - freevalue: function pointer to free from memory
 output:
  - none
 *****************************************************************************/
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
/******************************************************************************
 priorityQueueTop
 This function is pop first node in priority queue
 inputs:
  - priorityQueuep: pointer point to priorityqueue struct with nodesp, size, 
                    capacity and priority
 output:
  - a map with key and value
 *****************************************************************************/
const map_t *priorityQueueTop(priorityqueue_t *priorityQueuep)
{
    if (priorityQueuep->size > 0)
    {
        return priorityQueuep->nodesp[0];
    }
    return NULL;
}
/******************************************************************************
 priorityQueueDequeue
 This function is remove top node from queue
 inputs:
  - priorityQueuep: pointer point to priorityqueue struct with nodesp, size, 
                    capacity and priority
 output:
  - a map with key and value
 *****************************************************************************/
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
/******************************************************************************
 priorityQueueEnqueue
 This function is add node into queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
  - mapp: pointer to map with key and value
 output:
  - none
 *****************************************************************************/
void priorityQueueEnqueue(priorityqueue_t *pqp, map_t *mapp)
{
    if(debugMode)
        printf("add key:%d\n", mapp->key);
    pqp->nodesp[pqp->size] = mapp;
    priorityQueueAdjustTail(pqp);
    if (pqp->size >= pqp->capacity)
    {
        priorityQueueRealloc(pqp);
    }
}
/******************************************************************************
 priorityQueueSize
 This function is count size of queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
 output:
  - int datatype value, size of queue
 *****************************************************************************/
int priorityQueueSize(priorityqueue_t *pqp)
{
    return pqp->size;
}
/******************************************************************************
 priorityQueueEmpty
 This function is checking is queue is empty
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
 output:
  - 0 or 1 to show is queue is empty
 *****************************************************************************/
int priorityQueueEmpty(priorityqueue_t *pqp)
{
    return pqp->size <= 0;
}
/******************************************************************************
 priorityQueuePrint
 This function is printing the entire queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
 output:
  - none
 *****************************************************************************/
void priorityQueuePrint(priorityqueue_t *pqp)
{
    int i;
    map_t *mapp;
    printf("data in the pq->_nodes\n");
    for (i = 0; i < pqp->size; ++i)
    {
    /*
        debug only
        print all node in queue
    */
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
/******************************************************************************
 priorityQueueRealloc
 This function is reallocing the memory size of priority queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
 output:
  - none
 *****************************************************************************/
static void priorityQueueRealloc(priorityqueue_t *pqp)
{
    pqp->capacity = pqp->capacity * 2;
    pqp->nodesp = realloc(pqp->nodesp, sizeof(map_t *) * pqp->capacity);
}
/******************************************************************************
 priorityQueueAdjustHead
 This function is adjust the head of priority queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
 output:
  - none
 *****************************************************************************/
static void priorityQueueAdjustHead(priorityqueue_t *pqp)
{
    int i, j, parent, left, right;

    i = 0, j = 0;
    parent = left = right = 0;
    priorityQueueSwap(pqp->nodesp, 0, pqp->size - 1);
    pqp->size--;
    while (i < (pqp->size - 1) / 2)
    {
    /*
        for each queue node do adjust operaction
    */
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
/******************************************************************************
 priorityQueueAdjustTail
 This function is adjust the tail of priority queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
 output:
  - none
 *****************************************************************************/
static void priorityQueueAdjustTail(priorityqueue_t *pqp)
{
    int i, parent, child;

    i = pqp->size - 1;
    pqp->size++;
    while (i > 0)
    {
    /*
        for each queue node do adjust operaction
    */
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
/******************************************************************************
 priorityQueueCompare
 This function is compare the two value in queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
  - pos1: position of 1st node
  - pos2: position of 2nd node
 output:
  - compare result of 2 node
 *****************************************************************************/
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
/******************************************************************************
 priorityQueueSwap
 This function is swap 2 node in queue
 inputs:
  - pqp: pointer point to priorityqueue struct with nodesp, size, 
         capacity and priority
  - pos1: position of 1st node
  - pos2: position of 2nd node
 output:
  - none
 *****************************************************************************/
static void priorityQueueSwap(map_t **nodesp, int pos1, int pos2)
{
    map_t *temp = nodesp[pos1];
    nodesp[pos1] = nodesp[pos2];
    nodesp[pos2] = temp;
}
