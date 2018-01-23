// On non-windows platforms, we still need the C interfaces, but they simply
// give errors.

#ifndef _WIN32

#include <Rdefines.h>

SEXP processx3_is_named_pipe_open(SEXP pipe_ext) {
    error("processx3_is_named_pipe_open only valid on Windows.");
    return R_NilValue;
}

SEXP processx3_close_named_pipe(SEXP pipe_ext) {
    error("processx3_close_named_pipe only valid on Windows.");
    return R_NilValue;
}

SEXP processx3_create_named_pipe(SEXP name, SEXP mode) {
    error("processx3_create_named_pipe only valid on Windows.");
    return R_NilValue;
}

SEXP processx3_write_named_pipe(SEXP pipe_ext, SEXP text) {
    error("processx3_write_named_pipe only valid on Windows.");
    return R_NilValue;
}

#endif
