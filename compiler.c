
#include <stdlib.h>
#include "compiler.h"

bool alignment_ok(const void *p, size_t n)
{
#if HAVE_UNALIGNED_ACCESS
	return true;
#else
	return ((size_t)p % n == 0);
#endif
}
