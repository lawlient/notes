#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int status;
  pid_t pid = fork();

  if (pid == -1) {
    printf("fork() error");
    exit(3);
  } else if (pid == 0) {
    printf("child process ID : %d\n", pid);
    sleep(3);
    return 11;
  } else {
    wait(&status);
    // while (!waitpid(pid, &status, WNOHANG)) {
    //   sleep(1);
    //   printf("sleep 1 second\n");
    // }

    if (WIFEXITED(status)) {
      printf("child process send : %d\n", WEXITSTATUS(status));
      sleep(5);
    }
  }
  return 1;
}
