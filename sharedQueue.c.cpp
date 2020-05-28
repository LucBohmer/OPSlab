/******************************************************************************
 * File:         main.c
 * Version:      1.2
 * Date:         2017-10-18
 * Author:       J. Onokiewicz, M. van der Sluys
 * Description:  OPS exercise 5: Queues
 ******************************************************************************/


#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

data_t thread1 = {2, "Thread 1"};
data_t thread2 = {3, "Thread 2"};
data_t thread3 = {4, "Thread 3"};

queue_t queue = {NULL};

void *producer(void *input);
void *consumer(void *input);

int main()
{
  pthread_t tid;
  pthread_create(&tid, NULL, &producer, (void *)&thread1);
  pthread_create(&tid, NULL, &producer, (void *)&thread2);
  pthread_create(&tid, NULL, &producer, (void *)&thread3);
  pthread_create(&tid, NULL, &consumer, NULL);
  pthread_join(tid, NULL);
  return 0;
}

void *producer(void *input)
{
  data_t *arguments = input;
  sleep(arguments->intVal);
  if(backQueue(&queue) == NULL)
  {
    createQueue(&queue, *arguments);
  }
  else
  {
    pushQueue(&queue, *arguments);
  }
  pthread_exit(NULL);
}

void *consumer(void *input)
{
  sleep(15);
  showQueue(&queue);
  pthread_exit(NULL);
}