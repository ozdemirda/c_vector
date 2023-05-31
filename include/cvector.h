#pragma once

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct cvector cvector;

cvector* cvector_create(uint32_t elem_size);

void __cvector_destroy(cvector* v);

#define cvector_destroy(v)  \
  do {                      \
    if (v) {                \
      __cvector_destroy(v); \
      v = NULL;             \
    }                       \
  } while (0)

bool cvector_push_back(cvector* v, const void* new_elem);

bool cvector_pop_back(cvector* v, void* target_elem);

bool cvector_get_copy_at(cvector* v, uint32_t index, void* target_elem);

bool cvector_get_ptr_at(cvector* v, uint32_t index, void** target_elem_ptr);

uint32_t cvector_elem_count(cvector* v);

void cvector_reset(cvector* v);

void cvector_exec_for_each(cvector* v,
                           void (*read_write_callback)(uint32_t index,
                                                       void* elem, void* args),
                           void* args);

// Some useful macros
#define CVEC_DECLARE(v) cvector* v

#define CVEC_INIT(v, type) v = cvector_create(sizeof(type))

#define CVEC_CONSTRUCT(v, type) cvector* v = cvector_create(sizeof(type))

#define CVEC_DESTRUCT(v) cvector_destroy(v)

#define CVEC_PUSH(v, new_elem) \
  if (!cvector_push_back(v, (const void*)&new_elem)) assert(false)

#define CVEC_PUSH_RVALUE(v, new_elem) \
  if (!cvector_push_back(v, &(typeof(new_elem)){new_elem})) assert(false)

#define CVEC_POP(target, v) \
  if (!cvector_pop_back(v, &target)) assert(false)

#define CVEC_AT(target, v, index) \
  if (!cvector_get_copy_at(v, index, &target)) assert(false)

#define CVEC_PTR_AT(target, v, index) \
  if (!cvector_get_ptr_at(v, index, (void**)&target)) assert(false)

#define CVEC_SIZE(v) cvector_elem_count(v)

#define CVEC_RESET(v) cvector_reset(v)

#define CVEC_FOR_EACH(v, writer_f, args) \
  cvector_exec_for_each(v, writer_f, (void*)args)
