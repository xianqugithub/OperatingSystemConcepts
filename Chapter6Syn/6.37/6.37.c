#include "buffer.h"
#define MAX_THREAD 10
#include <fcntl.h> 

sem_t empty; //semaphore for empty
sem_t full; //semaphore for full
sem_t mutex; // mutex for sem_t


void *producer(void *param) {
  buffer_item item;
  int n = (int)param;
  
  while (1) {

    sleep(1); // sleep 

    /* produce */
    item = rand()%10; // generate a random number

    /* wait for empty slots */
    /* seems like not working properly on mac? */
    sem_wait(&empty);
     
    printf("\n***producer %d by-passed the semaphore with current count value: %d ****\n\n", n, count);
    
    /* lock the mutex */
    sem_wait(&mutex);

    int previn = in;

    if (!insert_item(item))
      printf("producer %d error.\n", n);
    else
      printf("producer %d produced %d at position %d\n", n, item, previn);

    sem_post(&mutex);
    sem_post(&full);
  }

}

void *consumer(void *param) {

  buffer_item item;

  int n = (int)param;

  while (1) {
    sleep(1+rand()%5);

    sem_wait(&full);
    sem_wait(&mutex);

    int prevout = out;
    
    if (!remove_item(&item))
      printf("consumer %d error.\n", n);
    else
      printf("consumer %d consumed %d at position %d\n", n, item, prevout);

    sem_post(&mutex);
    sem_post(&empty);
  }
}



int main(int argc, char *argv[]) {

  pthread_t ptid[MAX_THREAD]; // producer threads
  pthread_t ctid[MAX_THREAD]; // consumer threads
 
  int npt; /*number of producer threads */
  int nct; /* number of consumer threads */
  int i; /* for cycle */
  pthread_t tid;       //Thread ID
  pthread_attr_t attr; //Set of thread attributes

  
  
  /* get command lines*/
  if (argc != 4) {
    printf("Usage: Program, Sleeping Time, Producer Threads, Consumer Threads.\n");
    return 1;
  }

  if ((st = atoi(argv[1])) < 0) {
    printf("Invalid Sleeping Time.\n");
    return 1;
  }

  if ((npt = atoi(argv[2])) < 1) {
    printf("Invalid Producer Threads Number.\n");
    return 1;
  }

  if ((nct = atoi(argv[3])) < 1) {
    printf("Invalid Consumer Threads Number.\n");
    return 1;
  }

  printf("Number of Producer Threads: %d\n", npt);
  printf("Number of Consumer Threads: %d\n", nct);
  
  
  /* initalize buffer to be zero */
  count = 0;
  in = 0;
  out = 0;
  for (i = 0; i < BUFFER_SIZE; i++)
    buffer[i] = 0;

  /* initialization of mutex and semaphores */
  sem_init(&mutex, 0, 1);
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);

  /* Get the default attributes */
  pthread_attr_init(&attr);

  
  /* create producer threads */
  for (i = 0; i < npt; i++) {
    pthread_create(&tid,&attr,producer,(void *)i);
    printf("\nproducer %d CREATED\n", i);
  }

  /* create consumer threads */
  for (i = 0; i < nct; i++) {
    pthread_create(&tid,&attr,consumer,(void *)i);
    printf("\nconsumer %d CREATED\n", i);
  }
  

  /* sleep for a long time */
  sleep(100000);

  exit(0);

}




