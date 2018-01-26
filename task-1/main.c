//
//  main.c
//  task1
//
//  Created by Igashov_Ilya on 30.09.17.
//  Copyright (c) 2017 Igashov_Ilya. All rights reserved.
//

#define _POSIX_C_SOURCE 199309L 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "queue.h"
#include "thread_utils.h"
#include "longnum.h"

int main(int argc, char** argv)
{
    
    if(argc<2)
    {
        fprintf(stderr,"We need file with numbers in program arguments\n");
        return 1;
    }

    char* filename = argv[1];
    
    for (int threads_num = 1; threads_num < 33; ++threads_num)
    {
    //int threads_num = 30;
        FILE* file = fopen(filename, "r");
        queue_t queue;
        CreateQueue(&queue);
        
        pthread_t master_thread;
        pthread_t* slave_threads = (pthread_t*)malloc(threads_num * sizeof(pthread_t));
        
        master_args_t master_args;
        master_args.file = file;
        master_args.queue = &queue;
        
        slave_args_t* slave_args = (slave_args_t*)malloc(threads_num * sizeof(slave_args_t));
        
        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC, &start);

        pthread_create(&master_thread, NULL, MasterFunction, &master_args);
        for (int i = 0; i < threads_num; ++i)
        {
            slave_args[i].queue = &queue;
            create_longnum(&slave_args[i].local_counter);
            init_longnum(&slave_args[i].local_counter, "0", 1);
            pthread_create(&slave_threads[i], NULL, SlaveFunction, &slave_args[i]);
            
        }
        
        //pthread_join(master_thread, NULL);
        for (int i = 0; i < threads_num; ++i)
        {
            pthread_join(slave_threads[i], NULL);
            
        }
        pthread_join(master_thread, NULL);

        longnum_t total_cnt;
        create_longnum(&total_cnt);
        init_longnum(&total_cnt, "0", 1);
        for (int i = 0; i < threads_num; ++i)
        {
            longsum(&slave_args[i].local_counter, &total_cnt, &total_cnt);
        }
	
        clock_gettime(CLOCK_MONOTONIC, &finish);
        double time = (finish.tv_sec - start.tv_sec) * 1.0 + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("threads: %d\n time: %f\n", threads_num, time);
        printf("result: ");
        print_longnum(&total_cnt);
        
        destroy_longnum(&total_cnt);
        for (int i = 0; i < threads_num; ++i)
        {
            destroy_longnum(&slave_args[i].local_counter);
        }

        
        DestroyQueue(&queue);
        free(slave_threads);
        free(slave_args);
        fclose(file);
    }
    return 0;
}
