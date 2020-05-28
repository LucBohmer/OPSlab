#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

data_t thread1 = {2, "Thread 1"};
data_t thread2 = {3, "Thread 2"};
data_t thread3 = {4, "Thread 3"};

queue_t queue = {NULL};

int CTRLCPressed = 0;

int runningProducers = 0;

pthread_mutex_t lock;
FILE * fp;

void *producer(void *input);
void *consumer(void *input);
void sighandler(int sig_num);

int main()
{
  pthread_mutex_init(&lock, NULL);
  signal(SIGINT, sighandler);
	 
  pthread_t tid;
  pthread_create(&tid, NULL, &producer, (void *)&thread1);
  pthread_create(&tid, NULL, &producer, (void *)&thread2);
  pthread_create(&tid, NULL, &producer, (void *)&thread3);
  pthread_create(&tid, NULL, &consumer, NULL);
  pthread_join(tid, NULL);
  pthread_mutex_destroy(&lock);
  return 0;
}

void *producer(void *input)
{
  runningProducers++;
  data_t *arguments = input;
  while(1)
    {
      sleep(arguments->intVal);
	  pthread_mutex_lock(&lock);
	  
      if(backQueue(&queue) == NULL)
	{
	  createQueue(&queue, *arguments);
	}
      else
	{
	  pushQueue(&queue, *arguments);
	}
	pthread_mutex_unlock(&lock);
    if(CTRLCPressed == 1)
	{
	  runningProducers--;
	  pthread_exit(NULL);
	}
    }
}

void *consumer(void *input)
{
   while(1)
    {
      sleep(15);
	  pthread_mutex_lock(&lock);
      showQueue(&queue);
	  showQueueToFile(&queue, "log.txt");
      deleteQueue(&queue);
      pthread_mutex_unlock(&lock);
      if(runningProducers<=0)
	{
	  pthread_exit(NULL);
	}
   }
}

void sighandler(int sig_num)
{
  CTRLCPressed = 1;
}