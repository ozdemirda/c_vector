#include <cvector.h>
#include <stdlib.h>
#include <string.h>
#include <tau/tau.h>
TAU_MAIN()  // sets up Tau (+ main function)

// C_VECTOR TESTS

TEST(cvectors, create_fails) {
  char* err_str = NULL;
  cvector* cvec = cvector_create(0, &err_str);
  REQUIRE_EQ((void*)cvec, NULL);
  REQUIRE_NE((void*)err_str, NULL);

  cvec = cvector_create_mp(
      0,
      &(cvector_memmgmt_procs_t){
          .malloc = NULL, .free = free, .calloc = calloc, .realloc = realloc},
      &err_str);
  REQUIRE_EQ((void*)cvec, NULL);
  REQUIRE_NE((void*)err_str, NULL);

  cvec = cvector_create_mp(
      0,
      &(cvector_memmgmt_procs_t){
          .malloc = malloc, .free = NULL, .calloc = calloc, .realloc = realloc},
      &err_str);
  REQUIRE_EQ((void*)cvec, NULL);
  REQUIRE_NE((void*)err_str, NULL);

  cvec = cvector_create_mp(
      0,
      &(cvector_memmgmt_procs_t){
          .malloc = malloc, .free = free, .calloc = NULL, .realloc = realloc},
      &err_str);
  REQUIRE_EQ((void*)cvec, NULL);
  REQUIRE_NE((void*)err_str, NULL);

  cvec = cvector_create_mp(
      0,
      &(cvector_memmgmt_procs_t){
          .malloc = malloc, .free = free, .calloc = calloc, .realloc = NULL},
      &err_str);
  REQUIRE_EQ((void*)cvec, NULL);
  REQUIRE_NE((void*)err_str, NULL);
}

TEST(cvectors, create_succeeds) {
  char* err_str = NULL;
  cvector* cvec = cvector_create(sizeof(int), &err_str);
  REQUIRE_NE((void*)cvec, NULL);
  REQUIRE_EQ((void*)err_str, NULL);
  cvector_destroy(cvec);
  REQUIRE_EQ((void*)cvec, NULL);

  cvec = cvector_create_mp(
      sizeof(int),
      &(cvector_memmgmt_procs_t){
          .malloc = malloc, .free = free, .calloc = calloc, .realloc = realloc},
      &err_str);
  REQUIRE_NE((void*)cvec, NULL);
  REQUIRE_EQ((void*)err_str, NULL);
  cvector_destroy(cvec);
  REQUIRE_EQ((void*)cvec, NULL);
}

TEST(cvectors, simple_push_backs) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  REQUIRE_EQ(cvector_push_back(cvec, &(int){1}), cvec_success);
  REQUIRE_EQ(cvector_elem_count(cvec), 1);

  REQUIRE_EQ(cvector_push_back(cvec, &(int){1}), cvec_success);
  REQUIRE_EQ(cvector_elem_count(cvec), 2);

  cvector_destroy(cvec);
}

TEST(cvectors, simple_push_backs_with_memmgmt_procs) {
  cvector* cvec = cvector_create_mp(
      sizeof(int),
      &(cvector_memmgmt_procs_t){
          .malloc = malloc, .free = free, .calloc = calloc, .realloc = realloc},
      NULL);

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  REQUIRE_EQ(cvector_push_back(cvec, &(int){1}), cvec_success);
  REQUIRE_EQ(cvector_elem_count(cvec), 1);

  REQUIRE_EQ(cvector_push_back(cvec, &(int){1}), cvec_success);
  REQUIRE_EQ(cvector_elem_count(cvec), 2);

  cvector_destroy(cvec);
}

TEST(cvectors, access_an_index) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  int target = -2;
  REQUIRE_EQ(cvector_get_copy_at(cvec, 0, &target), cvec_key_not_found);
  REQUIRE_EQ(cvector_get_copy_at(cvec, 1, &target), cvec_key_not_found);
  REQUIRE_EQ(cvector_get_copy_at(cvec, (uint32_t)-1, &target),
             cvec_key_not_found);
  REQUIRE_EQ(target, -2);

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &(int){i + 1});
  }

  for (int i = 0; i < 4; ++i) {
    REQUIRE_EQ(cvector_get_copy_at(cvec, i, &target), cvec_success);
    REQUIRE_EQ(target, i + 1);
  }

  REQUIRE_EQ(cvector_get_copy_at(cvec, 5, &target), cvec_key_not_found);
  REQUIRE_EQ(cvector_get_copy_at(cvec, (uint32_t)-1, &target),
             cvec_key_not_found);

  cvector_destroy(cvec);
}

TEST(cvectors, access_the_ptr_at_index) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  int* target = NULL;
  REQUIRE_EQ(cvector_get_ptr_at(cvec, 0, (void**)&target), cvec_key_not_found);
  REQUIRE_EQ(cvector_get_ptr_at(cvec, 1, (void**)&target), cvec_key_not_found);
  REQUIRE_EQ(cvector_get_ptr_at(cvec, (uint32_t)-1, (void**)&target),
             cvec_key_not_found);
  REQUIRE_EQ((void*)target, NULL);

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &(int){i + 1});
  }

  for (int i = 0; i < 4; ++i) {
    REQUIRE_EQ(cvector_get_ptr_at(cvec, i, (void**)&target), cvec_success);
    REQUIRE_EQ(*target, i + 1);
  }

  REQUIRE_EQ(cvector_get_ptr_at(cvec, 5, (void**)&target), cvec_key_not_found);
  REQUIRE_EQ(cvector_get_ptr_at(cvec, (uint32_t)-1, (void**)&target),
             cvec_key_not_found);

  cvector_destroy(cvec);
}

TEST(cvectors, simple_pop_backs) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  int target = -2;
  REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_empty);
  REQUIRE_EQ(target, -2);

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  cvector_push_back(cvec, &(int){1});
  cvector_push_back(cvec, &(int){2});
  REQUIRE_EQ(cvector_elem_count(cvec), 2);

  REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
  REQUIRE_EQ(target, 2);
  REQUIRE_EQ(cvector_elem_count(cvec), 1);
  REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
  REQUIRE_EQ(target, 1);
  REQUIRE_EQ(cvector_elem_count(cvec), 0);

  cvector_destroy(cvec);
}

TEST(cvectors, different_sizes) {
  {
    cvector* cvec = cvector_create(sizeof(long), NULL);

    REQUIRE_EQ(cvector_push_back(cvec, &(long){1}), cvec_success);
    REQUIRE_EQ(cvector_push_back(cvec, &(long){2}), cvec_success);

    REQUIRE_EQ(cvector_elem_count(cvec), 2);

    long target;
    REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
    REQUIRE_EQ(target, 2);
    REQUIRE_EQ(cvector_elem_count(cvec), 1);
    REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
    REQUIRE_EQ(target, 1);
    REQUIRE_EQ(cvector_elem_count(cvec), 0);

    cvector_destroy(cvec);
  }

  {
    cvector* cvec = cvector_create(sizeof(char), NULL);

    REQUIRE_EQ(cvector_push_back(cvec, &(char){1}), cvec_success);
    REQUIRE_EQ(cvector_push_back(cvec, &(char){2}), cvec_success);

    REQUIRE_EQ(cvector_elem_count(cvec), 2);

    char target;
    REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
    REQUIRE_EQ(target, 2);
    REQUIRE_EQ(cvector_elem_count(cvec), 1);
    REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
    REQUIRE_EQ(target, 1);
    REQUIRE_EQ(cvector_elem_count(cvec), 0);

    cvector_destroy(cvec);
  }

  {
    cvector* cvec = cvector_create(sizeof(short), NULL);

    REQUIRE_EQ(cvector_push_back(cvec, &(short){1}), cvec_success);
    REQUIRE_EQ(cvector_push_back(cvec, &(short){2}), cvec_success);

    REQUIRE_EQ(cvector_elem_count(cvec), 2);

    short target;
    REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
    REQUIRE_EQ(target, 2);
    REQUIRE_EQ(cvector_elem_count(cvec), 1);
    REQUIRE_EQ(cvector_pop_back(cvec, &target), cvec_success);
    REQUIRE_EQ(target, 1);
    REQUIRE_EQ(cvector_elem_count(cvec), 0);

    cvector_destroy(cvec);
  }
}

TEST(cvectors, reset) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &(int){i + 1});
  }
  REQUIRE_EQ(cvector_elem_count(cvec), 4);

  cvector_reset(cvec);
  REQUIRE_EQ(cvector_elem_count(cvec), 0);

  cvector_destroy(cvec);
}

void incr_int_elem(uint32_t index, void* elem, void* args) {
  // The following if block is there to make the compiler happy,
  // it doesn't mean anything.
  if (index == (uint32_t)-1) {
    *(int*)args = 0;
  }

  *(int*)elem += (unsigned long)args;
}

TEST(cvectors, exec_for_each_wr) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &i);
  }

  cvector_exec_for_each(cvec, incr_int_elem, (void*)3);

  for (int i = 0; i < 4; ++i) {
    int target = -2;
    REQUIRE_EQ(cvector_get_copy_at(cvec, i, &target), cvec_success);
    REQUIRE_EQ(target, i + 3);
  }

  cvector_destroy(cvec);
}

void add_int_elem_to_sum(uint32_t index, void* elem, void* args) {
  // The following if block is there to make the compiler happy,
  // it doesn't mean anything.
  if (index == (uint32_t)-1) {
    *(int*)args = 0;
  }

  int* sum = (int*)args;
  *sum += *(int*)elem;
}

TEST(cvectors, exec_for_each_rd) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &i);
  }

  int sum = 0;
  cvector_exec_for_each(cvec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 6);  // 0 + 1 + 2 + 3

  for (int i = 0; i < 4; ++i) {
    int target = -2;
    REQUIRE_EQ(cvector_get_copy_at(cvec, i, &target), cvec_success);
    REQUIRE_EQ(target, i);
  }

  cvector_destroy(cvec);
}

extern uint32_t cvector_get_capacity(cvector* v);
extern const uint32_t minimum_capacity;
extern const uint32_t scaling_factor;

TEST(cvectors, scaling) {
  cvector* cvec = cvector_create(sizeof(int), NULL);

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  REQUIRE_EQ(cvector_get_capacity(cvec), minimum_capacity);

  for (uint32_t i = 0; i < minimum_capacity; ++i) {
    cvector_push_back(cvec, &i);
  }

  REQUIRE_EQ(cvector_elem_count(cvec), minimum_capacity);
  REQUIRE_EQ(cvector_get_capacity(cvec), minimum_capacity * scaling_factor);

  int tmp;
  for (uint32_t i = 0; i < minimum_capacity; ++i) {
    cvector_pop_back(cvec, &tmp);
  }
  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  REQUIRE_EQ(cvector_get_capacity(cvec), minimum_capacity);

  cvector_destroy(cvec);
}

TEST(cvectors, constructive_macros) {
  CVEC_CONSTRUCT(vec, int);

  int tmp = 2;
  CVEC_PUSH(vec, tmp);

  int val;
  CVEC_POP(val, vec);

  CVEC_RESET(vec);

  for (int i = 0; i < 10; ++i) {
    CVEC_PUSH_RVALUE(vec, i);
    CVEC_AT(val, vec, i);
    REQUIRE_EQ(val, i);

    int* ptr = NULL;
    CVEC_PTR_AT(ptr, vec, i);
    REQUIRE_EQ(*ptr, i);
  }

  int sum = 0;
  CVEC_FOR_EACH(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 45);  // 9*(9+1)/2

  CVEC_FOR_EACH(vec, incr_int_elem, 1);

  sum = 0;
  CVEC_FOR_EACH(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 55);  // 10*(10+1)/2

  for (int i = 0; i < 10; ++i) {
    int* ptr = NULL;
    CVEC_PTR_AT(ptr, vec, i);
    *ptr = 0;
  }

  sum = 0;
  CVEC_FOR_EACH(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 0);  // All elements were zeroed

  CVEC_DESTRUCT(vec);
}

TEST(cvectors, declarative_macros) {
  CVEC_DECLARE(vec);
  CVEC_INIT(vec, int);

  int tmp = 2;
  CVEC_PUSH(vec, tmp);

  int val;
  CVEC_POP(val, vec);

  CVEC_RESET(vec);

  for (int i = 0; i < 10; ++i) {
    CVEC_PUSH_RVALUE(vec, i);
    CVEC_AT(val, vec, i);
    REQUIRE_EQ(val, i);

    int* ptr = NULL;
    CVEC_PTR_AT(ptr, vec, i);
    REQUIRE_EQ(*ptr, i);
  }

  int sum = 0;
  CVEC_FOR_EACH(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 45);  // 9*(9+1)/2

  CVEC_FOR_EACH(vec, incr_int_elem, 1);

  sum = 0;
  CVEC_FOR_EACH(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 55);  // 10*(10+1)/2

  for (int i = 0; i < 10; ++i) {
    int* ptr = NULL;
    CVEC_PTR_AT(ptr, vec, i);
    *ptr = 0;
  }

  sum = 0;
  CVEC_FOR_EACH(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 0);  // All elements were zeroed

  CVEC_DESTRUCT(vec);
}
