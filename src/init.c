
#include <R_ext/Rdynload.h>
#include <R.h>

#include "processx3.h"

void R_init_processx3_win();
void R_init_processx3_unix();
SEXP processx3__killem_all();
SEXP run_testthat_tests();

static const R_CallMethodDef callMethods[]  = {
  { "processx3_exec",               (DL_FUNC) &processx3_exec,               9 },
  { "processx3_wait",               (DL_FUNC) &processx3_wait,               2 },
  { "processx3_is_alive",           (DL_FUNC) &processx3_is_alive,           1 },
  { "processx3_get_exit_status",    (DL_FUNC) &processx3_get_exit_status,    1 },
  { "processx3_signal",             (DL_FUNC) &processx3_signal,             2 },
  { "processx3_kill",               (DL_FUNC) &processx3_kill,               2 },
  { "processx3_get_pid",            (DL_FUNC) &processx3_get_pid,            1 },
  { "processx3_poll",               (DL_FUNC) &processx3_poll,               2 },
  { "processx3__process_exists",    (DL_FUNC) &processx3__process_exists,    1 },
  { "processx3__killem_all",        (DL_FUNC) &processx3__killem_all,        0 },
  { "processx3_is_named_pipe_open", (DL_FUNC) &processx3_is_named_pipe_open, 1 },
  { "processx3_close_named_pipe",   (DL_FUNC) &processx3_close_named_pipe,   1 },
  { "processx3_create_named_pipe",  (DL_FUNC) &processx3_create_named_pipe,  2 },
  { "processx3_write_named_pipe",   (DL_FUNC) &processx3_write_named_pipe,   2 },
  { "processx3__disconnect_process_handle",
    (DL_FUNC) &processx3__disconnect_process_handle, 1 },

  { "processx3_connection_create",     (DL_FUNC) &processx3_connection_create,     2 },
  { "processx3_connection_read_chars", (DL_FUNC) &processx3_connection_read_chars, 2 },
  { "processx3_connection_read_lines", (DL_FUNC) &processx3_connection_read_lines, 2 },
  { "processx3_connection_is_eof",     (DL_FUNC) &processx3_connection_is_eof,     1 },
  { "processx3_connection_close",      (DL_FUNC) &processx3_connection_close,      1 },
  { "processx3_connection_poll",       (DL_FUNC) &processx3_connection_poll,       2 },

  { "run_testthat_tests", (DL_FUNC) &run_testthat_tests, 0 },

  { NULL, NULL, 0 }
};

void R_init_processx3(DllInfo *dll) {
  R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
#ifdef _WIN32
  R_init_processx3_win();
#else
  R_init_processx3_unix();
#endif
}
