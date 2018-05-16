#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 3
#define K 2
#define N 3
#define NUM_THREADS 50 

int A[M][K] = {{1,4}, {2,5}, {3,6}};
int B[K][N] = {{8,7,6}, {5,4,3}};
int C[M][N];

struct v
{
  int row; 
  int col; 
};

void *runner(void *param);

int main()
{
  int wid; // workers id
    
  pthread_t workers[NUM_THREADS];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  wid = 0;

  for (int i = 0; i < M; i++)
    for (int j = 0; j < N; j++) {
      struct v* data = (struct v *) malloc(sizeof(struct v));
      data->row = i;
      data->col = j;
      pthread_create(&workers[wid++], &attr, runner, data);
    }

  for (int k = 0; k < wid; k++)
    pthread_join(workers[k], NULL);

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++)
      printf("%d ", C[i][j]);
    printf("\n");
  }

  return 0;
}

void *runner(void *param)
{
  int sum;
  int k;

  for (sum = 0, k = 0; k < K; k++)
    sum += A[((struct v*)param)->row][k]*B[k][((struct v*)param)->col];
  
  C[((struct v*)param)->row][((struct v*)param)->col] = sum;
}

