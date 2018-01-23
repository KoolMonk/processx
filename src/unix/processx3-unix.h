
#ifndef R_PROCESSX3_UNIX_H
#define R_PROCESSX3_UNIX_H

typedef struct processx3_handle_s {
  int exitcode;
  int collected;	 /* Whether exit code was collected already */
  pid_t pid;
  int fd0;			/* writeable */
  int fd1;			/* readable */
  int fd2;			/* readable */
  int waitpipe[2];		/* use it for wait() with timeout */
  int cleanup;
  processx3_connection_t *pipes[3];
} processx3_handle_t;

char *processx3__tmp_string(SEXP str, int i);
char **processx3__tmp_character(SEXP chr);

void processx3__sigchld_callback(int sig, siginfo_t *info, void *ctx);
void processx3__setup_sigchld();
void processx3__remove_sigchld();
void processx3__block_sigchld();
void processx3__unblock_sigchld();

void processx3__finalizer(SEXP status);
SEXP processx3__killem_all();

/* Child list and its functions */

typedef struct processx3__child_list_s {
  pid_t pid;
  SEXP status;
  struct processx3__child_list_s *next;
} processx3__child_list_t;

int processx3__child_add(pid_t pid, SEXP status);
void processx3__child_remove(pid_t pid);
processx3__child_list_t *processx3__child_find(pid_t pid);
void processx3__freelist_add(processx3__child_list_t *ptr);
void processx3__freelist_free();

void processx3__collect_exit_status(SEXP status, int wstat);

int processx3__nonblock_fcntl(int fd, int set);
int processx3__cloexec_fcntl(int fd, int set);

/* Control connections*/

void processx3__create_control_read(processx3_handle_t *handle,
				   int fd, const char *membername,
				   SEXP privatex);
void processx3__create_control_write(processx3_handle_t *handle,
				    int fd, const char *membername,
				    SEXP privatex);

/* Interruptible system calls */

int processx3__interruptible_poll(struct pollfd fds[],
				 nfds_t nfds, int timeout);

#endif
