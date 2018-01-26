//
//  longnum.h
//  task1
//
//  Created by Igashov_Ilya on 27.10.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#ifndef __task1__longnum__
#define __task1__longnum__

#include <stdio.h>

#define NUM_LEN 100

typedef struct {
    int len;
    char *num;
} longnum_t;

void create_longnum(longnum_t*);
void destroy_longnum(longnum_t*);
void init_longnum(longnum_t* longnum, char* string, int length);
void longsum(longnum_t* a, longnum_t* b, longnum_t* result);
void print_longnum(longnum_t* num);

#endif /* defined(__task1__longnum__) */
