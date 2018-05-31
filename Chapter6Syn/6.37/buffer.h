/* buffer.h */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef int buffer_item;

#define BUFFER_SIZE 5


/* the buffer */
buffer_item buffer[BUFFER_SIZE];




int st; /* sleep time */

int count; /* number of slots with products */
int in; /* pointer for production */
int out; /* pointer for take-out */

void *producer(void *param);
void *consumer(void *param);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);


