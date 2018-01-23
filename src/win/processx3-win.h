
#ifndef R_PROCESSX3_WIN_H
#define R_PROCESSX3_WIN_H

#include <windows.h>

typedef struct processx3_handle_s {
  int exitcode;
  int collected;	 /* Whether exit code was collected already */
  HANDLE hProcess;
  DWORD  dwProcessId;
  BYTE *child_stdio_buffer;
  HANDLE waitObject;
  processx3_connection_t *pipes[3];
  int cleanup;
} processx3_handle_t;

extern HANDLE processx3__iocp;

int processx3__utf8_to_utf16_alloc(const char* s, WCHAR** ws_ptr);

int processx3__stdio_create(processx3_handle_t *handle,
			   const char *std_out, const char *std_err,
			   BYTE** buffer_ptr, SEXP privatex,
			   const char *encoding);
WORD processx3__stdio_size(BYTE* buffer);
HANDLE processx3__stdio_handle(BYTE* buffer, int fd);
void processx3__stdio_destroy(BYTE* buffer);

void processx3__handle_destroy(processx3_handle_t *handle);

void processx3__cleanup_child_tree(DWORD pid);

#define PROCESSX3_ERROR(m,c) processx3__error((m),(c),__FILE__,__LINE__)
void processx3__error(const char *message, DWORD errorcode, const char *file, int line);

#endif
