#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h> 

int main(int argc, char *argv[]){
  
  char *name = argv[0];
  int child_pid = fork();
  if(child_pid == 0){
    printf("child of %s is %d\n", name, getpid());
    return 3;
  }else{
    printf("my child is %d\n", child_pid);
    return 0;
  }
}
