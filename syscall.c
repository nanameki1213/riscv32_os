#include "defines.h"
#include "proc.h"
#include "syscall.h"

kz_thread_id_t kz_run(kz_func_t func, char *name, int stacksize,
                      int argc, char *argv[])
{
  printf("kz_run start\n");
  kz_syscall_param_t param;
  param.un.run.func = func;
  param.un.run.name = name;
  param.un.run.stacksize = stacksize;
  param.un.run.argc = argc;
  param.un.run.argv = argv;
  kz_syscall(KZ_SYSCALL_TYPE_RUN, &param);
  return param.un.run.ret;
}

void kz_exit(void)
{
  printf("kz_exit start\n");
  kz_syscall(KZ_SYSCALL_TYPE_EXIT, NULL);
}
