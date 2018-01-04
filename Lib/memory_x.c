#include "hardwareConfig.h"
#include "memory_x.h"


#ifdef HEAP_BEGIN
char *heapEnd =  (char *)HEAP_BEGIN;

void *xmalloc(size_t size)
{
  void *result = malloc(size);

  heapEnd = (char *)(result);
  heapEnd += size;
  return result;
}

size_t xmallocAvailable(void)
{
  return __malloc_heap_end - heapEnd + 1;
}
#endif
