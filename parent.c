
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <signal.h>
#include <stdio.h>
#include "strace.h"

/* _NAME	_RTYPE		_PARAMS */
struct syscalls	g_syscalls[] =
  {
    { "read",	"ssize_t",	{ "int",
				  "void*",
				  "size_t",
				  NULL }},
    { "write",	"ssize_t",	{ "int",
				  "const void*",
				  "size_t",
				  NULL }},
    { "open",	"int",		{ "const char*",
				  "int",
				  NULL }},
    { "close",	"int",		{ "int",
				  NULL }},
    { "stat",	"int",		{ "const char*",
				  "struct stat*",
				  NULL }},
    { "fstat",	"int",		{"int",
				 "struct stat*",
				 NULL }},
    { "lstat",	"int",		{"const char*",
				 "struct stat*",
				 NULL }},
    { "poll",	"int",		{ "struct pollfd*",
				  "nfds_t",
				  "int",
				  NULL }},
    { "lseek",	"off_t",	{ "int",
				  "off_t",
				  "int",
				  NULL }},
    { "mmap",	"void*",	{ "void*",
				  "size_t",
				  "int",
				  "int",
				  "int",
				  "off_t",
				  NULL }},
    { "munmap",	"int",		{ "void*",
				  "size_t",
				  NULL }},
    { "brk",	"int",		{ "void*",
				  NULL }},
    { "rt_sigaction",	"int",	{ "int",
				  "const struct sigaction*",
				  "struct sigaction*",
				  NULL }},
    { "rt_sigprocmask",	"int",	{ "int",
				  "const sigset_t*",
				  "sigset_t*",
				  NULL }},
    { "rt_sigreturn",	"int",	{ "unsigned long",
				  NULL }},
    { "ioctl",	"int",		{ "int",
				  "int",
				  "...",
				  NULL }},
    { "pread64",	"ssize_t",	{ "int",
					  "void*",
					  "size_t",
					  "off_t",
					  NULL }},
    { "pwrite64",	"ssize_t",	{ "int",
					  "const void*",
					  "size_t",
					  "off_t",
					  NULL }},
    { "readv",	"ssize_t",	{ "int",
				  "const struct iovec*",
				  "int",
				  NULL }},
    { "writev",	"ssize_t",	{ "int",
				  "const struct iovec*",
				  "int",
				  NULL }},
    { "access",	"int",		{ "const char*",
				  "int",
				  NULL }},
    { "pipe",	"int",		{ "int[2]",
				  NULL }},
    { "select",	"int",		{ "int",
				  "fd_set*",
				  "fd_set*",
				  "fd_set*",
				  "struct timeval*",
				  NULL }},
    { "sched_yield",	"int",	{ "void",
				  NULL }},
    { "mremap",	"void*",	{ "void*",
				  "size_t",
				  "size_t",
				  "int",
				  "...",
				  NULL }},
    { "msync", "int",		{ "void*",
				  "size_t",
				  "int",
				  NULL}},
    { "mincore",	"int",	{ "void*",
				  "size_t",
				  "unsigned char*",
				  NULL }},
    { "madvise",	"int",	{ "void*",
				  "size_t",
				  "int",
				  NULL }},
    { "shmget",	"int",		{ "key_t",
				  "size_t",
				  "int",
				  NULL }},
    { "shmat",	"void*",	{ "int",
				  "const void*",
				  "int",
				  NULL }},
    { "shmctl",	"int",		{ "int",
				  "int",
				  "struct shmid_ds*",
				  NULL }},
    { "dup",	"int",		{ "int",
				  NULL }},
    { "dup2",	"int",		{ "int",
				  "int",
				  NULL }},
    { "pause",	"int",		{ "void",
				  NULL }},
    { "nanosleep",	"int",	{ "const struct timespec*",
				  "struct timespec*",
				  NULL }},
    { "getitimer",	"int",	{ "int",
				  "struct itimerval*",
				  NULL }},
    { "alarm",	"unsigned int",	{ "unsigned int",
				  NULL }},
    { "setitimer",	"int",	{ "int",
				  "const struct itimerval*",
				  "struct itimerval*",
				  NULL }},
    { "getpid",	"pid_t",	{ "void",
				  NULL }},
    { "sendfile",	"ssize_t",	{ "int",
					  "int",
					  "off_t*",
					  "size_t",
					  NULL }},
    { "socket",	"int",		{ "int",
				  "int",
				  "int",
				  NULL }},
    { "connect", "int",		{ "int",
				  "const struct sockaddr*",
				  "socklen_t",
				  NULL }},
    { "accept",	"int",		{ "int",
				  "struct sockaddr*",
				  "socklen_t*",
				  NULL }},
    { "sendto", "ssize_t",	{ "int",
				  "const void*",
				  "size_t",
				  "int",
				  NULL }},
    { "recvfrom",	"ssize_t",	{ "int",
					  "void*",
					  "size_t",
					  "int",
					  "struct sockaddr*",
					  "socklen_t*",
					  NULL }},
    { "sendmsg",	"ssize_t",	{ "int",
					  "const struct msghdr*",
					  "int",
					  NULL }},
    { "recvmsg",	"ssize_t",	{ "int",
					  "struct msghdr*",
					  "int",
					  NULL }},
    { "shutdown",	"int",	{ "int",
				  "int",
				  NULL }},
    { "bind",		"int",	{ "int",
				  "const struct sockaddr*",
				  "socklen_t",
				  NULL }},
    { "listen",		"int",	{ "int",
				  "int",
				  NULL }},
    { "getsockname",	"int",	{ "int",
				  "struct sockaddr*",
				  "socklen_t*",
				  NULL }},
    { "getpeername",	"int",	{ "int",
				  "struct sockaddr*",
				  "socklen_t*",
				  NULL }},
    { "socketpair",	"int",	{ "int",
				  "int",
				  "int",
				  "int*",
				  NULL }},
    { "setsockopt",	"int",	{ "int",
				  "int",
				  "int",
				  "const void*",
				  "socklen_t",
				  NULL }},
    { "getsockopt",	"int",	{ "int",
				  "int",
				  "int",
				  "void*",
				  "socklen_t*",
				  NULL }},
    { "clone",		"int",	{ "int (*)(void*)",
				  "void*",
				  "int",
				  "void*",
				  "...",
				  NULL }},
    { "fork",	"pid_t",	{ "void",
				  NULL }},
    { "vfork",	"pid_t",	{ "void",
				  NULL }},
    { "execve",		"int",	{ "const char*",
				  "char*const*",
				  "char*const*",
				  NULL }},    
    { "exit",		"void",	{ "int",
				  NULL }},
    { "wait4",	"pid_t",	{ "pid_t",
				  "int*",
				  "int",
				  "struct rusage*",
				  NULL }},
    { "kill",		"int",	{ "pid_t",
				  "int",
				  NULL }},
    { "uname",		"int",	{ "struct utsname*",
				  NULL }},
    { "semget",		"int",	{ "key_t",
				  "int",
				  "int",
				  NULL }},
    { "semop",		"int",	{ "int",
				  "struct sembuf*",
				  "unsigned",
				  NULL }},
    { "semctl",		"int",	{ "int",
				  "int",
				  "int",
				  NULL }},
    { "shmdt",		"int",	{ "const void*",
				  NULL }},
    { "msgget",		"int",	{ "key_t",
				  "int",
				  NULL }},
    { "msgsnd",		"int",	{ "int",
				  "const void*",
				  "size_t",
				  "int",
				  NULL }},
    { "msgrcv",	"ssize_t",	{ "int",
				  "void*",
				  "size_t",
				  "long",
				  "int",
				  NULL }},
    { "msgctl",		"int",	{ "int",
				  "int",
				  "struct msqid_ds*",
				  NULL }},
    { "fcntl",		"int",	{ "int",
				  "int",
				  "...",
				  NULL }},
    { "flock",		"int",	{ "int",
				  "int",
				  NULL }},
    { "fsync",		"int",	{ "int",
				  NULL }},
    { "fdatasync",	"int",	{ "int",
				  NULL }},
    { "truncate",	"int",	{ "const char*",
				  "off_t",
				  NULL }},
    { "ftruncate",	"int",	{ "int",
				  "off_t",
				  NULL }},
    { "getdents",	"int",	{ "unsigned int",
				  "struct linux_dirent*",
				  "unsigned int",
				  NULL }},
    { "getcwd",	"char*",	{ "char*",
				  "size_t",
				  NULL }},
    { "chdir",		"int",	{ "const char*",
				  NULL }},
    { "fchdir",		"int",	{ "int",
				  NULL }},
    { "rename",		"int",	{ "const char*",
				  "const char*",
				  NULL }},
    { "mkdir",		"int",	{ "const char*",
				  "mode_t",
				  NULL }},
    { "rmdir",		"int",	{ "const char*",
				  NULL }},
    { "creat",		"int",	{ "const char*",
				  "mode_t",
				  NULL }},
    { "link",		"int",	{ "const char*",
				  "const char*",
				  NULL }},
    { "unlink",		"int",	{ "const char*",
				  NULL }},
    { "symlink",	"int",	{ "const char*",
				  "const char*",
				  NULL }},
    { "readlink", "ssize_t",	{ "const char*",
				  "char*",
				  "size_t",
				  NULL }},
    { "chmod",		"int",	{ "const char*",
				  "mode_t",
				  NULL }},
    { "fchmod",		"int",	{ "int",
				  "mode_t",
				  NULL }},
    { "chown",		"int",	{ "const char*",
				  "uid_t",
				  "gid_t",
				  NULL }},
    { "fchown",		"int",	{ "int",
				  "uid_t",
				  "gid_t",
				  NULL }},
    { "lchown",		"int",	{ "const char*",
				  "uid_t",
				  "gid_t",
				  NULL }},
    { "umask",	"mode_t",	{ "mode_t",
				  NULL }},
    { "gettimeofday",	"int",	{ "struct timeval*",
				  "struct timezone*",
				  NULL }},
    { "getrlimit",	"int",	{ "int",
				  "struct rlimit*",
				  NULL }},
    { "getrusage",	"int",	{ "int",
				  "struct rusage*",
				  NULL }},
    { "sysinfo",	"int",	{ "struct sysinfo*",
				  NULL }},
    { "times",	"clock_t",	{ "struct tms*",
				  NULL }},
    { "ptrace",	"long",		{ "enum __ptrace_request",
				  "pid_t",
				  "void*",
				  "void*",
				  NULL }},
    { "getuid",	"uid_t",	{ "void",
				  NULL }},
    { "syslog",	"int",		{ "int",
				  "char*",
				  "int",
				  NULL }},
    { "getgid",	"uid_t",	{ "void",
				  NULL }},
    { "setuid",	"int",		{ "uid_t",
				  NULL }},
    { "setgid",	"int",		{ "gid_t",
				  NULL }},
    { "geteuid",	"uid_t",	{ "void",
					  NULL }},
    { "getegid",	"gid_t",	{ "void",
					  NULL }},
    { "setpgid",	"int",	{ "pid_t",
				  NULL }},
    { "getppid",	"pid_t",	{ "void",
					  NULL }},
    { "getpgrp",	"pid_t",	{ "void",
					  NULL }},
    { "setsid",	"pid_t",	{ "void",
				  NULL }},
    { "setreuid",	"int",	{ "uid_t",
				  "uid_t",
				  NULL }},
    { "setregid",	"int",	{ "gid_t",
				  "gid_t",
				  NULL }},
    { "getgroups",	"int",	{ "int",
				  "gid_t[]",
				  NULL }},
    { "setgroups",	"int",	{ "size_t",
				  "const gid_t*",
				  NULL }},
    { "setresuid",	"int",	{ "uid_t",
				  "uid_t",
				  "uid_t",
				  NULL }},
    { "getresuid",	"int",	{ "uid_t*",
				  "uid_t*",
				  "uid_t*",
				  NULL }},
    { "setresgid",	"int",	{ "gid_t",
				  "gid_t",
				  "gid_t",
				  NULL }},
    { "getresgid",	"int",	{ "gid_t*",
				  "gid_t*",
				  "gid_t*",
				  NULL }},
    { "getpgid",	"pid_t",	{ "pid_t",
					  NULL }},
    { "setfsuid",	"int",	{ "uid_t",
				  NULL }},
    { "setfsgid",	"int",	{ "uid_t",
				  NULL }},
    { "getsid",	"pid_t",	{ "pid_t",
				  NULL }},
    { "capget",	"int",		{ "cap_user_header_t",
				  "cap_user_data_t",
				  NULL }},
    { "capset",	"int",		{ "cap_user_header_t",
				  "const cap_user_data_t",
				  NULL }},
    { "rt_sigpending",	"int",	{ "sigset_t*",
				  NULL }},
    { "rt_sigtimedwait",	"int",	{ "const sigset_t*",
					  "siginfo_t*",
					  "const struct timespec*",
					  NULL }},
    { "rt_sigqueueinfo",	"int",	{ "pid_t",
					  "int",
					  "const union sigval",
					  NULL }},
    { "rt_sigsuspend",	"int",	{ "const sigset_t*",
				  NULL }},
    { "sigaltstack",	"int",	{ "const stack_t*",
				  "stack_t*",
				  NULL }},
    { "utime",	"int",		{ "const char*",
				  "const struct utimbuf*",
				  NULL }},
    { "mknod",	"int",		{ "const char*",
				  "mode_t",
				  "dev_t",
				  NULL }},
    { "uselib",	"int",		{ "const char*",
				  NULL }},
    { "personality",	"int",	{ "unsigned long",
				  NULL }},
    { "ustat",	"int",		{ "dev_t",
				  "struct ustat*",
				  NULL }},
    { "statfs",	"int",		{ "const char*",
				  "struct statfs*",
				  NULL }},
    { "fstatfs",	"int",	{ "int",
				  "struct statfs*",
				  NULL }},
    { "sysfs",	"int",		{ "int",
				  NULL }},
    { "getpriority",	"int",	{ "int",
				  "int",
				  NULL }},
    { "setpriority",	"int",	{ "int",
				  "int",
				  "int",
				  "int",
				  NULL }},
    { "sched_setparam",	"int",	{ "pid_t",
				  "const struct sched_param*",
				  NULL }},
    { "sched_getparam",	"int",	{ "pid_t",
				  "struct sched_param*",
				  NULL }},
    { "sched_setscheduler",	"int",	{ "pid_t",
					  "int",
					  "const struct sched_param*",
					  NULL }},
    { "sched_getscheduler",	"int",	{ "pid_t",
					  NULL }},
    { "sched_get_priority_max",	"int",	{ "int",
					  NULL }},
    { "sched_get_priority_min",	"int",	{ "int",
					  NULL }},
    { "sched_rr_get_interval",	"int",	{ "pid_t",
					  "struct timespec*",
					  NULL }},
    { "mlock",	"int",		{ "cont void*",
				  "size_t",
				  NULL }},
    { "munlock",	"int",	{ "const void*",
				  "size_t",
				  NULL }},
    { "mlockall",	"int",	{ "int",
				  NULL }},
    { "munlockall",	"int",	{ "void",
				  NULL }},
    { "vhangup",	"int",	{ "void",
				  NULL }},
    { "modify_ldt",	"int",	{ "int",
				  "void*",
				  "unsigned long",
				  NULL }},
    { "pivot_root",	"int",	{ "const char*",
				  "const char*",
				  NULL}},
    { "_sysctl",	"int",	{ "struct __sysctl_args*",
				  NULL }},
    { "prctl",	"int",		{ "int",
				  "unsigned long",
				  "unsigned long",
				  "unsigned long",
				  "unsigned long",
				  NULL }},
    { "arch_prctl",	"int",	{ "int",
				  "unsigned long",
				  NULL }},
    { "adjtimex",	"int",	{ "struct timex*",
				  NULL }},
    { "setrlimit",	"int",	{ "int",
				  "const struct rlimit*",
				  NULL }},
    { "chroot",	"int",		{ "const char*",
				  NULL }},
    { "sync",	"void",		{ "void",
				  NULL }},
    { "acct",	"int",		{ "const char*",
				  NULL }},
    { "settimeofday",	"int",	{ "const struct timeval*",
				  "const struct simezone*",
				  NULL }},
    { "mount",	"int",		{ "const char*",
				  "const char*",
				  "const char*",
				  "unsigned long",
				  "const void*",
				  NULL }},
    { "umount2",	"int",	{ "const char*",
				  "int",
				  NULL }},
    { "swapon",	"int",		{ "const char*",
				  "int",
				  NULL }},
    { "swapoff",	"int",	{ "const char*",
				  NULL }},
    { "reboot",	"int",		{ "int",
				  NULL }},
    { "sethostname",	"int",	{ "const char*",
				  "size_t",
				  NULL }},
    { "setdomainname",	"int",	{ "const char*",
				  "size_t",
				  NULL }},
    { "iopl",	"int",		{ "int",
				  NULL }},
    { "ioperm",	"int",		{ "unsigned long",
				  "unsigned long",
				  "int",
				  NULL }},
    { "create_module",	"caddr_t",	{ "const char*",
					  "size_t",
					  NULL }},
    { "init_module",	"int",	{ "const char*",
				  "struct module*",
				  NULL }},
    { "delete_module",	"int",	{ "const char*",
				  NULL }},
    { "get_kernel_syms",	"int",	{ "struct kernel_sym*",
					  NULL }},
    { "query_module",	"int",	{ "const char*",
				  "int",
				  "void*",
				  "size_t",
				  "size_t",
				  NULL }},
    { "quotactl",	"int",	{ "int",
				  "const char*",
				  "int",
				  "caddr_t",
				  NULL }},
    { "nfsservctl",	"long",	{ "int",
				  "struct nfsctl_arg*",
				  "union nfsctl_res*",
				  NULL }},
    { NULL,	NULL,		{ NULL }}
  };

static int	get_stopsig(int pid, char **strtab)
{
  siginfo_t	sig;

  (void)strtab;
  sig.si_signo = 0;
  if (-1 == ptrace(PTRACE_GETSIGINFO, pid, NULL, &sig))
    return printf("getsiginfo fail\n");
  if (sig.si_signo == 0 || sig.si_signo == 18 /* SIGCONT 18 19 25 ... */
      || sig.si_signo == 19 || sig.si_signo == 25
      || sig.si_signo == SIGTRAP)
    return 0;
  printf("Killed by signal %d : %s\n", sig.si_signo, strerror(sig.si_errno));
  return 1;
}

static struct syscalls	*get_call_infos(const char *name)
{
  size_t	i = 0;

  while (g_syscalls[i].name && strcmp(g_syscalls[i].name, name))
    ++i;
  if (g_syscalls[i].name)
    return &g_syscalls[i];
  return NULL;
}

static int	get_syscall(int pid, char **strtab)
{
  struct user	infos;
  struct syscalls *call;
  long		word;
  int		status;

  /* Return call informations */
  if (ptrace(PTRACE_GETREGS, pid, NULL, &infos) == -1)
    {      printf("getregs fail\n");      return 1;    }
  word = ptrace(PTRACE_PEEKTEXT, pid, infos.regs.rip, NULL);
  if ((word & 0xFFFF) != 0x050F)
    return 0;
  if (!(call = get_call_infos(strtab[(int)infos.regs.rax])))
    {
      printf("Unknown call...\n");
      return 0;
    }
  printf("%s\t", call->rtype);
  printf("%s(", call->name);
  for (int i = 0; call->p[i]; ++i) {
    printf("%s", call->p[i]);
    if (call->p[i+1])
      printf(", ");
  }
  printf(")");
  /* Go to return value */
  ptrace(PTRACE_SINGLESTEP, pid, NULL, 0);
  wait4(pid, &status, WUNTRACED, NULL);
  if (ptrace(PTRACE_GETREGS, pid, NULL, &infos) == -1)
    {      printf("\n");      return 1;    }
  if (strchr(call->rtype, '*'))
    printf("\t= %#lx\n", infos.regs.rax);
  else    
    printf("\t= %ld\n", infos.regs.rax);

  /* if (infos.regs.rdi) */
  /*   printf(">>>>>>> %s\n", infos.regs.rdi); */
  return 0;
}

static void	trace_process(int pid, char **strtab)
{
  int		status;

  while (1)
    {
      wait4(pid, &status, WUNTRACED, NULL);
      if (get_stopsig(pid, strtab))
      	break;
      if (status == 0)
	break;
      if (get_syscall(pid, strtab))
	break ;
      ptrace(PTRACE_SINGLESTEP, pid, NULL, 0);
    }
}

void		exec_parent(int pid, char **strtab)
{
  if (ptrace(PTRACE_ATTACH, pid, NULL, 0) == -1)
    {
      kill(pid, SIGKILL);
      exit_error("Cannot attach parent process");
    }
  trace_process(pid, strtab);
}
