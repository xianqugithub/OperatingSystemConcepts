#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define READ_END 0
#define WRITE_END 1
#define PERMS 0666

int main(int argc, char *argv[])
{
  char buf[BUFSIZ]; // read in buffers BUFSIZE defined already 

  int fd[2]; // file descriptor for pipes
  int input_fd; // input file descriptor
  int output_fd; // output file descriptor
  pid_t pid; // process id
  int n; // number of bytes read
  
  if (argc != 3) {
    printf("Wrong Number of Parameters.\n");
    return 1;
  }

  /* create pipe */
  if (pipe(fd) == -1) {
    printf("Pipe failed.\n");
    return 1;
  }

  /* fork the process */
  if ((pid = fork()) < 0) {
    printf("Fork Failure.\n");
    return 1;
  }

  if (pid > 0) { // the parent process
    if ((input_fd = open(argv[1], O_RDONLY, 0)) == -1) {
      printf("Error: Invalid Input file.\n");
      return 1;
    }
    /* close unused read end of the parent process*/
    close(fd[READ_END]);

    /* write the from the file to the pipe*/
    while ((n = read(input_fd, buf, BUFSIZ)) > 0)
      write(fd[WRITE_END], buf, n);

    close(fd[WRITE_END]);
  }

  else {
    close(fd[WRITE_END]);
    if ((output_fd = creat(argv[2], PERMS)) == -1) {
      printf("Error: Failed to create the output file.\n");
      return 1;
    }

    while ((n = read(fd[READ_END], buf, BUFSIZ)) > 0)
      write(output_fd, buf, n);
      close(fd[READ_END]);
  }

  return 0;

}
