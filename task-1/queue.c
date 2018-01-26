//
//  queue.c
//  task1
//
//  Created by Igashov_Ilya on 30.09.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#include "queue.h"

void CreateQueue(queue_t* queue)
{
    queue->size = 0;
    queue->head = (node_t*)malloc(sizeof(node_t)); // error check
    queue->head->next = queue->head->prev = NULL;
    queue->tail = queue->head;
    queue->is_shutted = 0;
    pthread_mutex_init(&queue->mutex, NULL); // error check, destroy then
    pthread_cond_init(&queue->queue_is_not_empty, NULL); // error check, destroy then
}

void DestroyQueue(queue_t* queue)
{
    free(queue->head);
    pthread_mutex_destroy(&queue->mutex); // error check
    pthread_cond_destroy(&queue->queue_is_not_empty); // error check
}

void Push(queue_t* queue, string_t string)
{
    pthread_mutex_lock(&queue->mutex); // error check
    if (queue->size == 0)
    {
        queue->head->string = string;
    }
    else
    {
        queue->tail->next = (node_t*)malloc(sizeof(node_t)); // error check
        queue->tail->next->string = string;
        queue->tail->next->next = NULL;
        queue->tail->next->prev = queue->tail;
        queue->tail = queue->tail->next;
    }
    ++queue->size;
    pthread_cond_broadcast(&queue->queue_is_not_empty); // error check
    pthread_mutex_unlock(&queue->mutex); // error check
}

int Pop(queue_t* queue, string_t* string)
{
    pthread_mutex_lock(&queue->mutex); // error check
    while (queue->size == 0 && !queue->is_shutted)
    {
        pthread_cond_wait(&queue->queue_is_not_empty, &queue->mutex); // error check
    }
    if (queue->size == 0 && queue->is_shutted)
    {
        pthread_mutex_unlock(&queue->mutex); // error check
        return 0;
    }
    *string = queue->head->string;
    if (queue->size > 1)
    {
        node_t* tmp = queue->head;
        queue->head = tmp->next;
        queue->head->prev = NULL;
        free(tmp);
    }
    --queue->size;
    pthread_mutex_unlock(&queue->mutex); // error check
    return 1;
}

void Shutdown(queue_t* queue)
{
    pthread_mutex_lock(&queue->mutex); // error check
    queue->is_shutted = 1;
    pthread_cond_broadcast(&queue->queue_is_not_empty); // error check
    pthread_mutex_unlock(&queue->mutex); // error check
}
