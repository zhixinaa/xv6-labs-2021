#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void pingpong() {
  int pid, n;
  int master_pid = getpid();
  int p1[2], p2[2]; // p1:child wirte p2:parent write
  if (pipe(p1) < 0) {
    printf("pipe() failed\n");
    exit(1);
  }
  if (pipe(p2) < 0) {
    printf("pipe() failed\n");
    exit(1);
  }
  char buf[1];
  pid = fork();
  if (pid > 0) { // parent
    n = write(p2[1], "x", 1);
    if(n<0){
      printf("write error\n");
    }
    n = read(p1[0], buf, 1);
    printf("%d: received pong\n", master_pid);
  } else if (pid == 0) { // child
    n = read(p2[0], buf, 1);

    master_pid = getpid();
    printf("%d: received ping\n", master_pid);

    n = write(p1[1], buf, 1);
    exit(0);
  } else {
    printf("fork error\n");
  }
}
int main(int argc, char **argv) {
  pingpong();
  exit(0);
}
