
#ifndef PROCESSX3_TYPES_H
#define PROCESSX3_TYPES_H

#ifdef _WIN32
#include <windows.h>
typedef DWORD pid_t;
#else
#include <signal.h>
#endif

typedef struct {
  pid_t *stor_begin;
  pid_t *stor_end;
  pid_t *end;
} processx3_vector_t;

#define VECTOR(v) ((v).stor_begin)

void processx3_vector_init(processx3_vector_t *v, size_t size, size_t alloc_size);
size_t processx3_vector_size(const processx3_vector_t *v);
void processx3_vector_reserve(processx3_vector_t *v, size_t size);
void processx3_vector_clear(processx3_vector_t *v);
void processx3_vector_push_back(processx3_vector_t *v, pid_t e);
int processx3_vector_find(const processx3_vector_t *v, pid_t e, size_t from, size_t *idx);
void processx3_vector_rooted_tree(pid_t root, const processx3_vector_t *nodes,
				 const processx3_vector_t *parents,
				 processx3_vector_t *result);

#endif
