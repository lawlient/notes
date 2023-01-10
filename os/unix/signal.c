#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void read_childproc(int sig) {
  int status;
  pid_t pid = waitpid(-1, &status, WNOHANG);
  if (WIFEXITED(status)) {
    printf("Removed proc id : %d\n", pid);
    printf("Child send : %d\n", WEXITSTATUS(status));
  }
}

int main(int argc, char *argv[]) {
  struct sigaction act;
  act.sa_handler = read_childproc;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGCHLD, &act, 0);

  pid_t pid = fork();
  if (pid == 0) {
    puts("Hi! I'm child process\n");
    sleep(10);
    return 11;
  } else {
    printf("Child proc id : %d\n", pid);
    pid = fork();
    if (pid == 0) {
      puts("Hi! I'm chlid process\n");
      sleep(10);
      exit(13);
    } else {
      printf("Chlid process id : %d\n", pid);
      for (int i = 0; i < 5; i++) {
        puts("wait ...\n");
        sleep(5);
      }
    }
  }
  return 0;
}
