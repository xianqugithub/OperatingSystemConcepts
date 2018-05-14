#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_SEQUENCE 10

typedef struct {
  long fib_sequence[MAX_SEQUENCE];
  int sequence_size;
} shared_data;


int main(int argc, char *argv[])
{
  int n; // the number where fibonacci sequence to be generated
  int pid; // process id
  int i; // index
  int segment_id; // the identifier for the shared memory segment
  shared_data *shared_memory; // a pointer to the shared memory segment
  int size; // the size of the memory to be allocated
  
  if (argc != 2) {
    printf("Wrong Parameter Numbers.\n");
    return 0;
  }

  if ((n = atoi(argv[1])) <= 0 || n > MAX_SEQUENCE) {
    printf("Wrong Range of Input Parameter. Must Be Between [1, %d]\n", MAX_SEQUENCE);
    return 0;
  }

  // create a shared-memory segment of size shared_data
  size = sizeof(shared_data);
  segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
  // attach the shared-memory segment to its address space
  shared_memory = (shared_data *) shmat(segment_id, NULL, 0);
  // set the value of sequnce_size to the parameter on the command line
  shared_memory->sequence_size = atoi(argv[1]);
  
  if ((pid = fork()) < 0) {
    printf("Failure to fork.\n");
    return 0;
  } else if (pid > 0) { // parent process
    wait(NULL); // wait for the child process to finish
    // printout the results
    for (int i = 0; i < shared_memory->sequence_size; i++)
      printf("%ld ", shared_memory->fib_sequence[i]);
    printf("\n");
    shmdt(shared_memory); // detach the shared memory segment
    shmctl(segment_id, IPC_RMID, NULL); //remove the shared memory segment

  } else { // child process
    shared_memory->fib_sequence[0] = 0;
    if (n > 1)
      shared_memory->fib_sequence[1] = 1;
    if (n > 2) {
      i = 2;
      while (i < n) {
	shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i-1] + shared_memory->fib_sequence[i-2]; 
	i++;
      }
    }
  }

  return 0;

}
