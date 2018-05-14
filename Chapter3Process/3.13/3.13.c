#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int n; /* given input to calclate the sequence */
  int i; // index number
  int prev, next, cur; // numbers
  int pid; /* process id */
  
  if (argc != 2) {
    fprintf(stderr, "Error: Wrong Number of Parameters Given.\n");
    return 0;
  }

  if ((n = atoi(argv[1])) < 0) {
    fprintf(stderr, "Error: Input Invalid. Must input an  Non-negative Integer.\n");
    return 0;
  }

  if ((pid = fork()) < 0) {
    fprintf(stderr, "Fork Failed.\n");
    return 0;
  }

  else if (pid == 0) { /* execute the fib calculation */
    if (n == 0) 
      fprintf(stdout, "%d\n", 0);
    else if (n == 1) {
      fprintf(stdout, "%d %d\n", 0, 1); 
    } else {
      fprintf(stdout, "%d %d ", 0, 1);
      i = 1;
      prev = 0;
      next = 1;
      while (++i <= n) {
	cur = prev + next;
	prev = next;
	next = cur;
	fprintf(stdout, "%d ", cur);
      }

      fprintf(stdout, "\n");
    }
    
  }

  else { /* parent process to wait the childe process to finish */
    wait(NULL);
    printf("child process completed.\n");
  }


  return 0;

}
