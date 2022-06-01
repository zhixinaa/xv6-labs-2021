#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char **argv) {
  int pid;
  char buf[512];
  for (int i = 0; i < argc - 1; i++) {
    strcpy(argv[i], argv[i + 1]);
  }
  while (gets(buf, 512)) {
    int len = strlen(buf);
    if(len==0) break;
    buf[len-1] = '\0';
    // printf("%s",buf);
    pid = fork();
    if (pid == 0) {
      strcpy(argv[argc-1], buf);
      exec(argv[0], argv);
    } else if (pid > 0) {
      wait(0);
    }
  }
  exit(0);
}
