//
//  queue.h
//  task1
//
//  Created by Igashov_Ilya on 30.09.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#ifndef task1_queue_h
#define task1_queue_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define STR_LEN 1000

typedef struct string
{
    char str[STR_LEN];
    unsigned long length;
} string_t;

typedef struct node
{
    string_t string;
    struct node* next;
    struct node* prev;
} node_t;

typedef struct queue
{
    node_t* head;
    node_t* tail;
    unsigned long size;
    pthread_mutex_t mutex;
    pthread_cond_t queue_is_not_empty;
    int is_shutted;
} queue_t;

void CreateQueue(queue_t*);
void DestroyQueue(queue_t*);
void Push(queue_t*, string_t);
int Pop(queue_t*, string_t*);
void Shutdown(queue_t*);

#endif
