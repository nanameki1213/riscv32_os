#pragma once

kz_thread_id_t kz_run(kz_func_t func, char *name, int stacksize,
											int argc, char *argv[]);
void kz_exit(void);

void kz_start(kz_func_t func, char *name, int stacksize,
							int argc, char *argv[]);
void kz_panic(void);
void kz_syscall(kz_syscall_type_t type, kz_syscall_param_t *param);
