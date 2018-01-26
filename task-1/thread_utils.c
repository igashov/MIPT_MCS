//
//  thread_utils.c
//  task1
//
//  Created by Igashov_Ilya on 30.09.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#include "thread_utils.h"

void* MasterFunction(void* args)
{
    master_args_t* arg = (master_args_t*)args;
    
    string_t string;
    while (GetLine(arg->file, string.str, &string.length))
    {
        Push(arg->queue, string);
    }
    
    Shutdown(arg->queue);
    pthread_exit(NULL);
}

void* SlaveFunction(void* args)
{
    slave_args_t* arg = (slave_args_t*)args;
    string_t string;
    longnum_t counter;
    create_longnum(&counter);
    while (Pop(arg->queue, &string))
    {
        CountDigits(string, &counter);
        longsum(&counter, &arg->local_counter, &arg->local_counter);
    }
    destroy_longnum(&counter);
    pthread_exit(NULL);
}

void CountDigits(string_t string, longnum_t* counter)
{
    init_longnum(counter, "0", 1);
    longnum_t current_number;
    create_longnum(&current_number);
    char last_symbol_is_digit = 0;
    char* current_ptr = string.str;
    int current_len = 0;
    for (unsigned long i = 0; i < string.length; ++i)
    {
        if (string.str[i] >= 48 && string.str[i] <= 57)
        {
            last_symbol_is_digit = 1;
            ++current_len;
        }
        else
        {
            if (last_symbol_is_digit)
            {
                init_longnum(&current_number, current_ptr, current_len);
                longsum(&current_number, counter, counter);
            }
            current_len = 0;
            current_ptr = string.str + i + 1;
            last_symbol_is_digit = 0;
        }
    }
    if (current_len > 0)
    {
        init_longnum(&current_number, current_ptr, current_len);
        longsum(&current_number, counter, counter);
    }
    destroy_longnum(&current_number);
}


char GetLine(FILE* file, char* str, unsigned long* length)
{
    char c = 0;
    *length = 0;
    while ((c = fgetc(file)) != '\n' && c != EOF)
    {
        str[*length] = c;
        ++(*length);
    }
    return (*length > 0) ? 1 : 0;
}
