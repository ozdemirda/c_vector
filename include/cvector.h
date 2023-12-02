#pragma once

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct cvector cvector;

typedef struct cvector_memmgmt_procs_t {
  void* (*malloc)(size_t size);
  void (*free)(void* ptr);
  void* (*calloc)(size_t elem_count, size_t elem_size);
  void* (*realloc)(void* ptr, size_t size);
} cvector_memmgmt_procs_t;

typedef enum cvector_retval_t {
  // The provided arguments are not valid
  cvec_invalid_arguments = -4,
  // The provided key was not found
  cvec_key_not_found,
  // The container is empty
  cvec_empty,
  // We failed to create a message
  cvec_not_enough_memory,
  // All is good, the operation was a success
  cvec_success
} cvector_retval_t;

cvector* cvector_create_mp(uint32_t elem_size,
                           cvector_memmgmt_procs_t* mmgmt_procs, char** err);

#define cvector_create(elem_size, err) cvector_create_mp(elem_size, NULL, err)

void __cvector_destroy(cvector* v);

#define cvector_destroy(v)  \
  do {                      \
    if (v) {                \
      __cvector_destroy(v); \
      v = NULL;             \
    }                       \
  } while (0)

cvector_retval_t cvector_push_back(cvector* v, const void* new_elem);

cvector_retval_t cvector_pop_back(cvector* v, void* target_elem);

cvector_retval_t cvector_get_copy_at(cvector* v, uint32_t index,
                                     void* target_elem);

cvector_retval_t cvector_get_ptr_at(cvector* v, uint32_t index,
                                    void** target_elem_ptr);

uint32_t cvector_elem_count(cvector* v);

void cvector_reset(cvector* v);

void cvector_exec_for_each(cvector* v,
                           void (*read_write_callback)(uint32_t index,
                                                       void* elem, void* args),
                           void* args);

// Some useful macros
#define CVEC_DECLARE(v) cvector* v

#define CVEC_INIT(v, type)                      \
  do {                                          \
    char* err_str = NULL;                       \
    v = cvector_create(sizeof(type), &err_str); \
    if (err_str) {                              \
      assert(!err_str);                         \
    }                                           \
  } while (0)

#define CVEC_CONSTRUCT(v, type)                 \
  cvector* v;                                   \
  do {                                          \
    char* err_str;                              \
    v = cvector_create(sizeof(type), &err_str); \
    if (err_str) {                              \
      assert(!err_str);                         \
    }                                           \
  } while (0)

#define CVEC_DESTRUCT(v) cvector_destroy(v)

#define CVEC_PUSH(v, new_elem)                                         \
  do {                                                                 \
    cvector_retval_t r = cvector_push_back(v, (const void*)&new_elem); \
    if (r != cvec_success) {                                           \
      assert(r == cvec_success);                                       \
    }                                                                  \
  } while (0)

#define CVEC_PUSH_RVALUE(v, new_elem)                                         \
  do {                                                                        \
    cvector_retval_t r = cvector_push_back(v, &(typeof(new_elem)){new_elem}); \
    if (r != cvec_success) {                                                  \
      assert(r == cvec_success);                                              \
    }                                                                         \
  } while (0)

#define CVEC_POP(target, v)                            \
  do {                                                 \
    cvector_retval_t r = cvector_pop_back(v, &target); \
    if (r != cvec_success) {                           \
      assert(r == cvec_success);                       \
    }                                                  \
  } while (0)

#define CVEC_AT(target, v, index)                                \
  do {                                                           \
    cvector_retval_t r = cvector_get_copy_at(v, index, &target); \
    if (r != cvec_success) {                                     \
      assert(r == cvec_success);                                 \
    }                                                            \
  } while (0)

#define CVEC_PTR_AT(target, v, index)                                   \
  do {                                                                  \
    cvector_retval_t r = cvector_get_ptr_at(v, index, (void**)&target); \
    if (r != cvec_success) {                                            \
      assert(r == cvec_success);                                        \
    }                                                                   \
  } while (0)

#define CVEC_SIZE(v) cvector_elem_count(v)

#define CVEC_RESET(v) cvector_reset(v)

#define CVEC_FOR_EACH(v, writer_f, args) \
  cvector_exec_for_each(v, writer_f, (void*)args)
