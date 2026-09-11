#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

char *target;
struct stat st;
struct dirent de;
char *p;
char path[512];

void
find()
{
  int fd;
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannont stat %s\n", path);
    return;
  }

  close(fd);

  switch(st.type){
    case T_FILE:
    case T_DEVICE:
      for(; p > path && *(p - 1) != '/'; p--);
      if(strcmp(p,target) == 0){
        fprintf(2, "%s\n", path);
      }
      p--;
      *p = '\0';
      return;
    case T_DIR:
      if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
      }

      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum != 0 && strcmp(de.name, ".") && strcmp(de.name, "..")){
          *p = '/';
          p++;
          memmove(p, de.name, sizeof(de.name));
          p += strlen(de.name);
          *p = '\0';
          find();
        }
      }

      for(; p > path && *(p - 1) != '/'; p--);
      p--;
      *p = '\0';
 
      close(fd);
  }
  return;
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "hello hello hello\n");
    exit(0);
  }

  target = argv[2];

  //判断文件名是否超出缓冲期大小
  if((strlen(argv[1]) + strlen(argv[2])) > 512){
    fprintf(2, "find: too long path or name\n");
  }
  memmove(path, argv[1], strlen(argv[1]) + 1);
  p = path + strlen(argv[1]);

  find();
  
  exit(0);
}
