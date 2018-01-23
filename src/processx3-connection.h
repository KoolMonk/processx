
#ifndef PROCESSX3_CONNECTION_H
#define PROCESSX3_CONNECTION_H

#include <Rinternals.h>
#include <R_ext/Riconv.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* --------------------------------------------------------------------- */
/* Data types                                                            */
/* --------------------------------------------------------------------- */

#ifdef _WIN32
typedef HANDLE processx3_file_handle_t;
typedef struct {
  HANDLE handle;
  BOOLEAN async;
  OVERLAPPED overlapped;
  BOOLEAN read_pending;
} processx3_i_connection_t;
#else
typedef int processx3_file_handle_t;
typedef int processx3_i_connection_t;
#endif

typedef enum {
  PROCESSX3_FILE_TYPE_FILE = 1,	/* regular file, blocking IO */
  PROCESSX3_FILE_TYPE_ASYNCFILE,	/* regular file, async IO (well, win only) */
  PROCESSX3_FILE_TYPE_PIPE,	/* pipe, blocking IO */
  PROCESSX3_FILE_TYPE_ASYNCPIPE	/* pipe, async IO */
} processx3_file_type_t;

typedef struct processx3_connection_s {
  processx3_file_type_t type;

  int is_closed_;
  int is_eof_;			/* the UTF8 buffer */
  int is_eof_raw_;		/* the raw file */

  char *encoding;
  void *iconv_ctx;

  processx3_i_connection_t handle;

  char* buffer;
  size_t buffer_allocated_size;
  size_t buffer_data_size;

  char *utf8;
  size_t utf8_allocated_size;
  size_t utf8_data_size;

} processx3_connection_t;

/* Generic poll method
 *
 * @param object The thing to poll.
 * @param status Currently not used.
 * @param handle A handle can be returned here, to `poll` or wait on.
 *   If this is not needed, set it to NULL.
 * @param timeout A timeout value can be returned here, for the next
 *   poll. If this is not needed, set it to NULL.
 * @return The result of the pre-polling. PXCLOSED, PXREADY or PXSILENT.
 *   PXREADY: data is readily available, at least one character.
 *     (But maybe not a full line.)
 *   PXSILENT: we don't know if data is available, we need to check the
 *     operating system via `poll` or `WaitForStatus`.
 */

typedef int (*processx3_connection_poll_func_t)(
  void *object,
  int status,
  processx3_file_handle_t *handle,
  int *again);

/* Data structure for a pollable object
 *
 * @member poll_func The function to call on the object, before
 *   the poll/wait system call. The pollable object might have data
 *   available without immediately, without poll/wait. If not, it
 *   will return the file descriptor or HANDLE to poll.
 * @member object The object to pass to `poll_func`.
 * @member free Whether to call `free()` on `object` when finalizing
 *   `processx3_pollable_t` objects.
 * @member event The result of the polling is stored here. Possible values:
 *   `PXSILENT` (no data), `PXREADY` (data), `PXTIMEOUT` (timeout).
 */

typedef struct processx3_pollable_s {
  processx3_connection_poll_func_t poll_func;
  void *object;
  int free;
  int event;
} processx3_pollable_t;

/* --------------------------------------------------------------------- */
/* API from R                                                            */
/* --------------------------------------------------------------------- */

/* Create connection from fd / HANDLE */
SEXP processx3_connection_create(SEXP handle, SEXP encoding);

/* Read characters in a given encoding from the connection. */
SEXP processx3_connection_read_chars(SEXP con, SEXP nchars);

/* Read lines of characters from the connection. */
SEXP processx3_connection_read_lines(SEXP con, SEXP nlines);

/* Check if the connection has ended. */
SEXP processx3_connection_is_eof(SEXP con);

/* Close the connection. */
SEXP processx3_connection_close(SEXP con);
SEXP processx3_is_closed(SEXP con);

/* Poll connections and other pollable handles */
SEXP processx3_connection_poll(SEXP pollables, SEXP timeout);

/* --------------------------------------------------------------------- */
/* API from C                                                            */
/* --------------------------------------------------------------------- */

/* Create connection object */
processx3_connection_t *processx3_c_connection_create(
  processx3_file_handle_t os_handle,
  processx3_file_type_t type,
  const char *encoding,
  SEXP *r_connection);

/* Destroy connection object. We need this for the C API */
void processx3_c_connection_destroy(processx3_connection_t *ccon);

/* Read characters */
ssize_t processx3_c_connection_read_chars(
  processx3_connection_t *con,
  void *buffer,
  size_t nbyte);

/* Read lines of characters */
ssize_t processx3_c_connection_read_line(
  processx3_connection_t *ccon,
  char **linep,
  size_t *linecapp);

/* Check if the connection has ended */
int processx3_c_connection_is_eof(
  processx3_connection_t *con);

/* Close */
void processx3_c_connection_close(
  processx3_connection_t *con);
int processx3_c_connection_is_closed(
  processx3_connection_t *con);

/* Poll connections and other pollable handles */
int processx3_c_connection_poll(
  processx3_pollable_t pollables[],
  size_t npollables, int timeout);

/* Helper function to create pollable handles*/
int processx3_c_pollable_from_connection(
  processx3_pollable_t *pollable,
  processx3_connection_t *ccon);

/* --------------------------------------------------------------------- */
/* Internals                                                             */
/* --------------------------------------------------------------------- */

#ifndef _WIN32
typedef unsigned long DWORD;
#endif

#define PROCESSX3_ERROR(m,c) processx3__error((m),(c),__FILE__,__LINE__)
void processx3__error(const char *message, DWORD errorcode,
		     const char *file, int line);

#endif
