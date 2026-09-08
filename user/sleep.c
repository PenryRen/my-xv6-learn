#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
sleep(int time)
{
  pause(time);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc <= 1){
    fprintf(2, "Usage: sleep times...\n");
    exit(0);
  }

  for(i = 1; i < argc; i++){
    //TODO: how to avoid user input negetive number?
    sleep(atoi(argv[i]));
  }
  exit(0);
}
