INCLUDES = -I. -I../include
DEFINITIONS = -DRUNNING_UNIT_TESTS
SRC_FILE_PREFIX = cvector
SRC_FILES = ../src/$(SRC_FILE_PREFIX).c
ALL_SRC_FILES = tests.c $(SRC_FILES)
CFLAGS = $(INCLUDES) $(DEFINITIONS) -fstack-protector-all -Wstrict-overflow \
	-Wformat=2 -Wformat-security -Wall -Wextra -g3 -O3 -Werror
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage 
LFLAGS = -lm -lpthread

build:
	gcc $(CFLAGS) $(ALL_SRC_FILES) -o tests $(LFLAGS)

test: build
	./tests

memtest: build
	valgrind ./tests

generate_coverage_report:
	gcc $(COVERAGE_FLAGS) $(CFLAGS) $(ALL_SRC_FILES) -o tests $(LFLAGS) && \
	./tests && \
	gcov tests-$(SRC_FILE_PREFIX) && \
	lcov --capture --directory . --output-file $(SRC_FILE_PREFIX).c.info && \
	genhtml $(SRC_FILE_PREFIX).c.info --output-directory coverage && \
	rm -rf *.gcno *.gcda *.gcov *.c.info

all: build test memtest generate_coverage_report

clean:
	rm -rf tests coverage *.gcda *.gcdo

default: build
