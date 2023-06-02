/*
MIT License

Copyright (c) 2018 Danis Ozdemir

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <cvector.h>
#include <stdlib.h>
#include <string.h>

#define mem_alloc(size) malloc(size)
#define mem_calloc(elem_count, elem_size) calloc(elem_count, elem_size)
#define mem_realloc(ptr, new_size) realloc(ptr, new_size)
#define mem_free(ptr) free(ptr)

#define stringify(s) #s
#define x_stringify(s) stringify(s)
#define CERR_STR(x) (__FILE__ ":" x_stringify(__LINE__) " - " x)

const uint32_t minimum_capacity = 4;
const uint32_t scaling_factor = 2;

struct cvector {
  uint32_t elem_size;
  uint32_t elem_count;
  uint32_t capacity;
  void* data_ptr;
};

void __cvector_destroy(cvector* v) {
  if (v) {
    mem_free(v->data_ptr);
    mem_free(v);
  }
}

cvector* cvector_create(uint32_t elem_size, char** err) {
  if (elem_size == 0) {
    if (err) {
      *err = CERR_STR("elem_size is zero");
    }
    return NULL;
  }

  cvector* v = mem_calloc(1, sizeof(cvector));
  if (!v) {
    if (err) {
      *err = CERR_STR("failed to allocate vector container");
    }
    return NULL;
  }

  v->data_ptr = mem_alloc(minimum_capacity * elem_size);
  if (!v->data_ptr) {
    __cvector_destroy(v);
    if (err) {
      *err = CERR_STR("failed to allocate data container");
    }
    return NULL;
  }

  if (err) {
    *err = NULL;
  }

  v->capacity = minimum_capacity;
  v->elem_count = 0;
  v->elem_size = elem_size;

  return v;
}

bool scale_the_cvector_size_up(cvector* v) {
  if (!v) {
    return false;
  }

  void* orig = v->data_ptr;
  v->data_ptr =
      mem_realloc(v->data_ptr, scaling_factor * v->capacity * v->elem_size);
  if (!v->data_ptr) {
    v->data_ptr = orig;
    return false;
  }

  v->capacity *= scaling_factor;
  return true;
}

void scale_the_cvector_size_down(cvector* v) {
  if (!v) {
    return;
  }

  if (v->capacity == minimum_capacity) {
    return;
  }

  void* orig = v->data_ptr;
  v->data_ptr =
      mem_realloc(v->data_ptr, (v->capacity / scaling_factor) * v->elem_size);
  if (!v->data_ptr) {
    v->data_ptr = orig;
    return;
  }

  v->capacity /= scaling_factor;
}

static inline void assign(void* dest, const void* src, uint32_t size) {
  if (size == sizeof(unsigned int)) {
    *(unsigned int*)dest = *(unsigned int*)src;
  } else if (size == sizeof(unsigned char)) {
    *(unsigned char*)dest = *(unsigned char*)src;
  } else if (size == sizeof(unsigned long)) {
    *(unsigned long*)dest = *(unsigned long*)src;
  } else if (size == sizeof(unsigned short)) {
    *(unsigned short*)dest = *(unsigned short*)src;
  } else {
    memcpy(dest, src, size);
  }
}

cvector_retval_t cvector_push_back(cvector* v, const void* new_elem) {
  if (!v || !new_elem) {
    return cvec_invalid_arguments;
  }

  cvector_retval_t result = cvec_success;

  if (v->elem_count < v->capacity) {
    assign((void*)((unsigned long)v->data_ptr + v->elem_count * v->elem_size),
           new_elem, v->elem_size);
    if (++v->elem_count == v->capacity) {
      // Ignoring the return value of scale_the_cvector_size_up
      // as we managed to insert the new_elem.
      scale_the_cvector_size_up(v);
    }
  } else {
    if (scale_the_cvector_size_up(v)) {
      assign((void*)((unsigned long)v->data_ptr + v->elem_count * v->elem_size),
             new_elem, v->elem_size);
      ++v->elem_count;
    } else {
      result = cvec_not_enough_memory;
    }
  }

  return result;
}

cvector_retval_t cvector_pop_back(cvector* v, void* target_elem) {
  if (!v || !target_elem) {
    return cvec_invalid_arguments;
  }

  cvector_retval_t result = cvec_empty;

  if (v->elem_count > 0) {
    result = cvec_success;
    --v->elem_count;

    assign(target_elem,
           (void*)((unsigned long)v->data_ptr + v->elem_count * v->elem_size),
           v->elem_size);

    if (v->elem_count < (v->capacity / minimum_capacity)) {
      scale_the_cvector_size_down(v);
    }
  }

  return result;
}

cvector_retval_t cvector_get_copy_at(cvector* v, uint32_t index,
                                     void* target_elem) {
  if (!v || !target_elem) {
    return cvec_invalid_arguments;
  }

  cvector_retval_t result = cvec_key_not_found;

  if (v->elem_count > 0 && index < v->elem_count) {
    assign(target_elem,
           (void*)((unsigned long)v->data_ptr + index * v->elem_size),
           v->elem_size);
    result = cvec_success;
  }

  return result;
}

cvector_retval_t cvector_get_ptr_at(cvector* v, uint32_t index,
                                    void** target_elem_ptr) {
  if (!v || !target_elem_ptr) {
    return cvec_invalid_arguments;
  }

  cvector_retval_t result = cvec_key_not_found;

  if (v->elem_count > 0 && index < v->elem_count) {
    *target_elem_ptr =
        (void*)((unsigned long)v->data_ptr + index * v->elem_size);
    result = cvec_success;
  }

  return result;
}

uint32_t cvector_elem_count(cvector* v) {
  if (!v) {
    return 0;
  }

  return v->elem_count;
}

void cvector_reset(cvector* v) {
  if (!v) {
    return;
  }

  void* orig = v->data_ptr;
  v->data_ptr = mem_realloc(v->data_ptr, minimum_capacity * v->elem_size);
  if (!v->data_ptr) {
    // Capacity should remain unchanged if reallocation fails.
    v->data_ptr = orig;
  } else {
    v->capacity = minimum_capacity;
  }
  v->elem_count = 0;
}

void cvector_exec_for_each(cvector* v,
                           void (*rw_callback)(uint32_t index, void* elem,
                                               void* args),
                           void* args) {
  if (!v || !rw_callback) {
    return;
  }

  unsigned long data_ptr = (unsigned long)v->data_ptr;
  uint32_t elem_size = v->elem_size;
  uint32_t elem_count = v->elem_count;

  for (uint32_t i = 0; i < elem_count; ++i) {
    (*rw_callback)(i, (void*)(data_ptr + i * elem_size), args);
  }
}

#ifdef RUNNING_UNIT_TESTS
uint32_t cvector_get_capacity(cvector* v) {
  if (!v) {
    return 0;
  }

  return v->capacity;
}
#endif
