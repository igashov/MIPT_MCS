//
//  longnum.c
//  task1
//
//  Created by Igashov_Ilya on 27.10.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#include "longnum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_longnum(longnum_t* longnum)
{
    longnum->len = NUM_LEN;
    longnum->num = (char*)malloc(NUM_LEN * sizeof(char));
}

void destroy_longnum(longnum_t* longnum)
{
    free(longnum->num);
}

void init_longnum(longnum_t* longnum, char* string, int length)
{
    for (int i = 0; i < length; ++i)
    {
        longnum->num[longnum->len - length + i] = *(string + i) - 48;
    }
    for (int i = 0; i < longnum->len - length; ++i) {
        longnum->num[i] = 0;
    }
}

void longsum(longnum_t* a, longnum_t* b, longnum_t* result)
{
    char* temp = (char*)malloc(NUM_LEN * sizeof(int));
    char excess = 0;
    for (int i = NUM_LEN - 1; i >= 0; --i)
    {
        temp[i] = (a->num[i] + b->num[i] + excess) % 10;
        excess = (a->num[i] + b->num[i] + excess) / 10;
    }
    for (int i = 0; i < NUM_LEN; ++i)
    {
        result->num[result->len - NUM_LEN + i] = temp[i];
    }
    free(temp);
}

void print_longnum(longnum_t* num)
{
    char zeros_yet = 1;
    for (int i = 0; i < num->len; ++i)
    {
	if (num->num[i] != 0) 
	{
	    zeros_yet = 0;
            printf("%d", num->num[i]);
	}
	else 
	{
	    if (!zeros_yet)
	    {
		printf("%d", num->num[i]);
	    }
	}
    }
    printf("\n");
}
