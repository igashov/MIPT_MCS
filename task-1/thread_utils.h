//
//  thread_utils.h
//  task1
//
//  Created by Igashov_Ilya on 30.09.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#ifndef __task1__thread_utils__
#define __task1__thread_utils__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "longnum.h"

typedef struct master_args
{
    FILE* file;
    queue_t* queue;
} master_args_t;

typedef struct slave_args
{
    longnum_t local_counter;
    queue_t* queue;
} slave_args_t;

void* MasterFunction(void*);
void* SlaveFunction(void*);

void CountDigits(string_t string, longnum_t* counter);
char GetLine(FILE*, char*, unsigned long*);

#endif 
