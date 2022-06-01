#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  // if(strlen(p) >= DIRSIZ)
  //   return p;
  // memmove(buf, p, strlen(p));
  // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return p;
}

void
find(char *path, char * str)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // switch(st.type){
  // case T_FILE:
  //   printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
  //   // if(!strcmp(fmtname(path),str))
  //   // printf("%s %d %d %l\n", path, st.type, st.ino, st.size);
  //   break;

  // case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      char * fmt  =fmtname(buf);
      // printf("%s %d %d %d\n", fmt, st.type, st.ino, st.size);
      // printf("*********\n");
      // printf("%s %d\n", fmt,strlen(fmt));
      // printf("%s %d\n", str,strlen(str));
      // printf("%d\n",strcmp(fmt,str));
      switch(st.type){
        case T_FILE:
          if(strcmp(fmt,str)==0){
            printf("%s\n", buf);
          }
          break;
        case T_DIR:
          // printf("%s %d\n", buf,strlen(buf));
          if(strcmp(fmt,".") && strcmp(fmt,".."))
            find(buf,str);
          break;
      }
    }
  
  // }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    printf("error\n");
    exit(0);
  }
  find(argv[1],argv[2]);
  exit(0);
}
