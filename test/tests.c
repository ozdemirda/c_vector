#include <cvector.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <tau/tau.h>
TAU_MAIN()  // sets up Tau (+ main function)

// C_VECTOR TESTS

TEST(cvectors, create_fails) {
  cvector* cvec = cvector_create(0);
  REQUIRE_EQ((void*)cvec, NULL);
}

TEST(cvectors, create_succeeds) {
  cvector* cvec = cvector_create(sizeof(int));
  REQUIRE_NE((void*)cvec, NULL);
  cvector_destroy(cvec);
  REQUIRE_EQ((void*)cvec, NULL);
}

TEST(cvectors, simple_push_backs) {
  cvector* cvec = cvector_create(sizeof(int));

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  REQUIRE_EQ((int)cvector_push_back(cvec, &(int){1}), (int)true);
  REQUIRE_EQ(cvector_elem_count(cvec), 1);

  REQUIRE_EQ((int)cvector_push_back(cvec, &(int){1}), (int)true);
  REQUIRE_EQ(cvector_elem_count(cvec), 2);

  cvector_destroy(cvec);
}

TEST(cvectors, access_an_index) {
  cvector* cvec = cvector_create(sizeof(int));

  int target = -2;
  REQUIRE_EQ((int)cvector_get_copy_at(cvec, 0, &target), (int)false);
  REQUIRE_EQ((int)cvector_get_copy_at(cvec, 1, &target), (int)false);
  REQUIRE_EQ((int)cvector_get_copy_at(cvec, (uint32_t)-1, &target), (int)false);
  REQUIRE_EQ(target, -2);

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &(int){i + 1});
  }

  for (int i = 0; i < 4; ++i) {
    REQUIRE_EQ((int)cvector_get_copy_at(cvec, i, &target), (int)true);
    REQUIRE_EQ(target, i + 1);
  }

  REQUIRE_EQ((int)cvector_get_copy_at(cvec, 5, &target), (int)false);
  REQUIRE_EQ((int)cvector_get_copy_at(cvec, (uint32_t)-1, &target), (int)false);

  cvector_destroy(cvec);
}

TEST(cvectors, access_the_ptr_at_index) {
  cvector* cvec = cvector_create(sizeof(int));

  int* target = NULL;
  REQUIRE_EQ((int)cvector_get_ptr_at(cvec, 0, (void**)&target), (int)false);
  REQUIRE_EQ((int)cvector_get_ptr_at(cvec, 1, (void**)&target), (int)false);
  REQUIRE_EQ((int)cvector_get_ptr_at(cvec, (uint32_t)-1, (void**)&target),
             (int)false);
  REQUIRE_EQ((void*)target, NULL);

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &(int){i + 1});
  }

  for (int i = 0; i < 4; ++i) {
    REQUIRE_EQ((int)cvector_get_ptr_at(cvec, i, (void**)&target), (int)true);
    REQUIRE_EQ(*target, i + 1);
  }

  REQUIRE_EQ((int)cvector_get_ptr_at(cvec, 5, (void**)&target), (int)false);
  REQUIRE_EQ((int)cvector_get_ptr_at(cvec, (uint32_t)-1, (void**)&target),
             (int)false);

  cvector_destroy(cvec);
}

TEST(cvectors, simple_pop_backs) {
  cvector* cvec = cvector_create(sizeof(int));

  int target = -2;
  REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)false);
  REQUIRE_EQ(target, -2);

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  cvector_push_back(cvec, &(int){1});
  cvector_push_back(cvec, &(int){2});
  REQUIRE_EQ(cvector_elem_count(cvec), 2);

  REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
  REQUIRE_EQ(target, 2);
  REQUIRE_EQ(cvector_elem_count(cvec), 1);
  REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
  REQUIRE_EQ(target, 1);
  REQUIRE_EQ(cvector_elem_count(cvec), 0);

  cvector_destroy(cvec);
}

TEST(cvectors, different_sizes) {
  {
    cvector* cvec = cvector_create(sizeof(long));

    REQUIRE_EQ((int)cvector_push_back(cvec, &(long){1}), (int)true);
    REQUIRE_EQ((int)cvector_push_back(cvec, &(long){2}), (int)true);

    REQUIRE_EQ(cvector_elem_count(cvec), 2);

    long target;
    REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
    REQUIRE_EQ(target, 2);
    REQUIRE_EQ(cvector_elem_count(cvec), 1);
    REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
    REQUIRE_EQ(target, 1);
    REQUIRE_EQ(cvector_elem_count(cvec), 0);

    cvector_destroy(cvec);
  }

  {
    cvector* cvec = cvector_create(sizeof(char));

    REQUIRE_EQ((int)cvector_push_back(cvec, &(char){1}), (int)true);
    REQUIRE_EQ((int)cvector_push_back(cvec, &(char){2}), (int)true);

    REQUIRE_EQ(cvector_elem_count(cvec), 2);

    char target;
    REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
    REQUIRE_EQ(target, 2);
    REQUIRE_EQ(cvector_elem_count(cvec), 1);
    REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
    REQUIRE_EQ(target, 1);
    REQUIRE_EQ(cvector_elem_count(cvec), 0);

    cvector_destroy(cvec);
  }

  {
    cvector* cvec = cvector_create(sizeof(short));

    REQUIRE_EQ((int)cvector_push_back(cvec, &(short){1}), (int)true);
    REQUIRE_EQ((int)cvector_push_back(cvec, &(short){2}), (int)true);

    REQUIRE_EQ(cvector_elem_count(cvec), 2);

    short target;
    REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
    REQUIRE_EQ(target, 2);
    REQUIRE_EQ(cvector_elem_count(cvec), 1);
    REQUIRE_EQ((int)cvector_pop_back(cvec, &target), (int)true);
    REQUIRE_EQ(target, 1);
    REQUIRE_EQ(cvector_elem_count(cvec), 0);

    cvector_destroy(cvec);
  }
}

TEST(cvectors, reset) {
  cvector* cvec = cvector_create(sizeof(int));

  REQUIRE_EQ(cvector_elem_count(cvec), 0);
  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &(int){i + 1});
  }
  REQUIRE_EQ(cvector_elem_count(cvec), 4);

  cvector_reset(cvec);
  REQUIRE_EQ(cvector_elem_count(cvec), 0);

  cvector_destroy(cvec);
}

void square_int_elem(uint32_t index, void* elem, void* args) {
  // The following if block is there to make the compiler happy,
  // it doesn't mean anything.
  if (index == (uint32_t)-1) {
    *(int*)args = 0;
  }

  *(int*)elem *= *(int*)elem;
}

TEST(cvectors, exec_func_on_elem) {
  cvector* cvec = cvector_create(sizeof(int));

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &i);
  }

  for (int i = 0; i < 4; ++i) {
    REQUIRE_EQ((int)cvector_exec_func_on_elem(cvec, i, square_int_elem, NULL),
               (int)true);
  }

  for (int i = 0; i < 4; ++i) {
    int target = -2;
    REQUIRE_EQ((int)cvector_get_copy_at(cvec, i, &target), (int)true);
    REQUIRE_EQ(target, i * i);
  }

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
  cvector* cvec = cvector_create(sizeof(int));

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &i);
  }

  cvector_exec_for_each_wr(cvec, incr_int_elem, (void*)3);

  for (int i = 0; i < 4; ++i) {
    int target = -2;
    REQUIRE_EQ((int)cvector_get_copy_at(cvec, i, &target), (int)true);
    REQUIRE_EQ(target, i + 3);
  }

  cvector_destroy(cvec);
}

void add_int_elem_to_sum(uint32_t index, const void* elem, void* args) {
  // The following if block is there to make the compiler happy,
  // it doesn't mean anything.
  if (index == (uint32_t)-1) {
    *(int*)args = 0;
  }

  int* sum = (int*)args;
  *sum += *(int*)elem;
}

TEST(cvectors, exec_for_each_rd) {
  cvector* cvec = cvector_create(sizeof(int));

  for (int i = 0; i < 4; ++i) {
    cvector_push_back(cvec, &i);
  }

  int sum = 0;
  cvector_exec_for_each_rd(cvec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 6);  // 0 + 1 + 2 + 3

  for (int i = 0; i < 4; ++i) {
    int target = -2;
    REQUIRE_EQ((int)cvector_get_copy_at(cvec, i, &target), (int)true);
    REQUIRE_EQ(target, i);
  }

  cvector_destroy(cvec);
}

extern uint32_t cvector_get_capacity(cvector* v);
extern const uint32_t minimum_capacity;
extern const uint32_t scaling_factor;

TEST(cvectors, scaling) {
  cvector* cvec = cvector_create(sizeof(int));

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

void zero_int_elem(uint32_t index, void* elem, void* args) {
  // The following if block is there to make the compiler happy,
  // it doesn't mean anything.
  if (index == (uint32_t)-1) {
    *(int*)args = 0;
  }

  *(int*)elem *= 0;
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
  CVEC_FOR_EACH_RD(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 45);  // 9*(9+1)/2

  CVEC_FOR_EACH_WR(vec, incr_int_elem, 1);

  sum = 0;
  CVEC_FOR_EACH_RD(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 55);  // 10*(10+1)/2

  for (int i = 0; i < 10; ++i) {
    CVEC_EXEC_FUNC_AT(vec, i, zero_int_elem, NULL);
  }

  sum = 0;
  CVEC_FOR_EACH_RD(vec, add_int_elem_to_sum, &sum);
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
  CVEC_FOR_EACH_RD(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 45);  // 9*(9+1)/2

  CVEC_FOR_EACH_WR(vec, incr_int_elem, 1);

  sum = 0;
  CVEC_FOR_EACH_RD(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 55);  // 10*(10+1)/2

  for (int i = 0; i < 10; ++i) {
    CVEC_EXEC_FUNC_AT(vec, i, zero_int_elem, NULL);
  }

  sum = 0;
  CVEC_FOR_EACH_RD(vec, add_int_elem_to_sum, &sum);
  REQUIRE_EQ(sum, 0);  // All elements were zeroed

  CVEC_DESTRUCT(vec);
}
