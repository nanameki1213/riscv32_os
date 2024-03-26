#include "defines.h"
#include "proc.h"
#include "lib.h"

int thread_1_main(int argc, char *argv[])
{
  printf("I'm thread_1_main\n");
  return 0;
}

int thread_2_main(int argc, char *argv[])
{
  printf("I'm thread_2_main\n");
  return 0;
}
