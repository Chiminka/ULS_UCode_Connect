#ifndef MAC_MALLOC_H
#define MAC_MALLOC_H
#include <malloc.h>

static inline unsigned long malloc_size(void *ptr)
{
	return malloc_usable_size(ptr);
}

#endif /* MAC_MALLOC_H */
