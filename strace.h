#ifndef STRACE_H_
# define STRACE_H_

/*
** Defines
*/
#ifndef NULL
# define NULL	(void*)0
#endif

# define	MAXPARAMS	7

struct	syscalls
{
  char	*name;
  char	*rtype;
  char	*p[MAXPARAMS];   /* les parametres */
};

/*
** Prototypes
*/
void		exit_error(const char*);

void		exec_child(char*);
void		exec_parent(int, char **);

#endif /* !STRACE_H_ */
