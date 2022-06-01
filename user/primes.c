#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void prime(int input_fd) {
  int base;
  if (read(input_fd, &base, sizeof(int)) == 0) {
    close(input_fd);
    exit(0);
  }
  printf("prime %d\n", base);
  int n;
  int p[2];
  if (pipe(p) < 0) {
    printf("pipe() failed\n");
    exit(1);
  }
  if (fork() == 0) { // child
    close(p[1]);
    prime(p[0]);
  } else {
    close(p[0]);
    // parent
    while (read(input_fd, &n, sizeof(int)) != 0) {
      if (n % base != 0)
        write(p[1], &n, sizeof(int));
    }
    close(p[1]);
    wait(0);
    exit(0);
  }
}
int main(int argc, char **argv) {
  int n;
  int p1[2];

  if (pipe(p1) < 0) {
    printf("pipe() failed\n");
    exit(1);
  }

  for (int i = 0; i < 34; i++) {
    n = i + 2;
    write(p1[1], &n, sizeof(int));
  }
  close(p1[1]);
  prime(p1[0]);
  exit(0);
}
