#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100

int seq[MAXLEN];

void *runner(void *param); /* thread */

int main(int argc, char *argv[])
{
  pthread_t tid; /* thread id */
  pthread_attr_t attr; /* thread attributes */
  int n; 

  if (argc != 2) {
    printf("Wrong Number of Paramter.\n");
    return 1;
  }

  if ((n = atoi(argv[1])) < 0 || n >= MAXLEN) {
    printf("Wrong Input Range.\n");
    return 1;
  }

  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, runner, &n);
  pthread_join(tid, NULL);

  /* print out by the main thread */
  for (int i = 0; i <= n; i++) {
    printf("%d ", seq[i]);
  }

  printf("\n");

  return 0;
}

/* calculation by the child thread*/

void *runner(void *param) {

  int max = *((int *)param); // cast the pointer time

  seq[0] = 0;
  seq[1] = 1;

  for (int i = 2; i <= max ; i++)
    seq[i] = seq[i-1]+seq[i-2];
  pthread_exit(0);
}
