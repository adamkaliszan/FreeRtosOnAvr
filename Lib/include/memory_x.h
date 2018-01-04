#ifndef MEMORY_X_H
#define MEMORY_X_H

#include "hardwareConfig.h"
#include <stdlib.h>

#ifdef HEAP_BEGIN
/**
 * Allocate memory
 * @param size - amount of memory to allocate
 */
void  *xmalloc(size_t size);
#else
#define xmalloc( SIZE ) malloc(SIZE)
#endif

#ifdef HEAP_BEGIN
/**
 * Amount of free space on heap
 * @return number of free bytes on heap
 */
size_t xmallocAvailable(void);
#endif

#endif /*MEMORY_X_H*/
