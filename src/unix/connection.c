
#include "../processx3.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


processx3_connection_t* processx3__create_connection(
  int fd, const char *membername,
  SEXP private,
  const char *encoding) {

  processx3_connection_t *con;
  SEXP res;

  con = processx3_c_connection_create(fd, PROCESSX3_FILE_TYPE_ASYNCPIPE,
				     encoding, &res);

  defineVar(install(membername), res, private);

  return con;
}

void processx3__create_connections(processx3_handle_t *handle, SEXP private,
				  const char *encoding) {
  handle->pipes[0] = handle->pipes[1] = handle->pipes[2] = 0;

  if (handle->fd1 >= 0) {
    handle->pipes[1] = processx3__create_connection(
      handle->fd1,
      "stdout_pipe",
      private,
      encoding);
  }

  if (handle->fd2 >= 0) {
    handle->pipes[2] = processx3__create_connection(
      handle->fd2,
      "stderr_pipe",
      private,
      encoding);
  }
}
