
#ifndef COMPILER_H
#define COMPILER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool alignment_ok(const void *p, size_t n);

#define UNUSED __attribute__((__unused__))

/**
 * BUILD_ASSERT - assert a build-time dependency.
 * @cond: the compile-time condition which must be true.
 *
 * Your compile will fail if the condition isn't true, or can't be evaluated
 * by the compiler.  This can only be used within a function.
 *
 * Example:
 *	#include <stddef.h>
 *	...
 *	static char *foo_to_char(struct foo *foo)
 *	{
 *		// This code needs string to be at start of foo.
 *		BUILD_ASSERT(offsetof(struct foo, string) == 0);
 *		return (char *)foo;
 *	}
 */
#define BUILD_ASSERT(cond) \
	do { (void) sizeof(char [1 - 2*!(cond)]); } while(0)

/**
 * BUILD_ASSERT_OR_ZERO - assert a build-time dependency, as an expression.
 * @cond: the compile-time condition which must be true.
 *
 * Your compile will fail if the condition isn't true, or can't be evaluated
 * by the compiler.  This can be used in an expression: its value is "0".
 *
 * Example:
 *	#define foo_to_char(foo)					\
 *		 ((char *)(foo)						\
 *		  + BUILD_ASSERT_OR_ZERO(offsetof(struct foo, string) == 0))
 */
#define BUILD_ASSERT_OR_ZERO(cond) \
	(sizeof(char [1 - 2*!(cond)]) - 1)

#define memclear(mem, size) memset(mem, 0, size)
#define memclear_2(m1, s1, m2, s2) { memclear(m1, s1); memclear(m2, s2); }
#define memclear_3(m1, s1, m2, s2, m3, s3) { memclear(m1, s1); memclear(m2, s2); memclear(m3, s3); }
#define wally_clear memclear
#define wally_clear_2 memclear_2
#define wally_clear_3 memclear_3

#endif /* COMPILER_H */
