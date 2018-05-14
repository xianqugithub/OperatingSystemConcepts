// Does IO action here exempt the requirement for synchonrization?

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define READ_END 0
#define WRITE_END 1

int main()
{
  char write_msg[BUFFER_SIZE] = "Hello, Youyou";
  char read_msg[BUFFER_SIZE];
  /* two pipes used for communication*/
  int fd_one[2], fd_two[2];
  pid_t pid;

  /* create a pipe */
  if ((pipe(fd_one) == -1) || (pipe(fd_two) == -1)) {
    printf("Pipe failed.\n");
    return 1;
  }

  if ((pid = fork()) < 0) {
    printf("Fork Failure.\n");
    return 1;
  }

  if (pid > 0) {
    /* close unused end in the main process*/
    close(fd_one[READ_END]);
    close(fd_two[WRITE_END]);
    
    /* write the original message to the */
    write(fd_one[WRITE_END], write_msg, strlen(write_msg)+1);
    close(fd_one[WRITE_END]);

    read(fd_two[READ_END], read_msg, BUFFER_SIZE);
    printf("%s\n", read_msg);
    close(fd_two[READ_END]);
  }

  else {
    close(fd_one[WRITE_END]);
    close(fd_two[READ_END]);

    read(fd_one[READ_END], read_msg, BUFFER_SIZE);
    close(fd_one[READ_END]);

    // convert the read_msg;
    for (int i = 0; i <= strlen(read_msg); i++) {
      if (read_msg[i] >= 'a' && read_msg[i] <= 'z')
	write_msg[i] = read_msg[i] - 'a' + 'A';
      else if (read_msg[i] >= 'A' && read_msg[i] <= 'Z')
	write_msg[i] = read_msg[i] - 'A' + 'a';
      else
	write_msg[i] = read_msg[i];
    }
    
    write(fd_two[WRITE_END], write_msg, strlen(write_msg)+1);
    close(fd_two[WRITE_END]);
  }

  return 0;

}
