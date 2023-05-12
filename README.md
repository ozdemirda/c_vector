This library is a very minimalistic vector implementation in C. All functions
are thread-safe. This implementation does not offer all of the APIs provided
by the C++ vector implementation for the sake of simplicity, but it gets the
job done.

A simple usage example is below:

```c
#include <cvector.h>
#include <stdio.h>

int main() {
    // Here we could have gone for the CVEC_DECLARE and CVEC_INIT macros,
    // the CVEC_CONSTRUCT just combines them. The declarative macros are
    // useful when one wants to have a vector as a struct member.
	CVEC_CONSTRUCT(vec, int);

    // Here, since '3' itself does not have an actual address,
    // the library constructs an integer obj containing that value
    // and uses that to operate on.
	CVEC_PUSH_RVALUE(vec, 3);
	CVEC_PUSH_RVALUE(vec, 5);

    int val = 1;
    // Here, as vec corresponds to an integer of 4 bytes allocated on the stack,
    // it has a definite address and the library can directly operate on that.
    CVEC_PUSH(vec, val);

	printf("Size: %u\n", CVEC_SIZE(vec)); // This should print 3

	int target = 0;
	CVEC_POP(target, vec);
	printf("Popped %d\n", target); // This should print 1

	CVEC_POP(target, vec);
	printf("Popped %d\n", target); // This should print 5

	CVEC_POP(target, vec);
	printf("Popped %d\n", target); // This should print 3

	CVEC_DESTRUCT(vec);

    return 0;
}
```
