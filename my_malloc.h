#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define MEMORY_LENGTH 10000
#define MAX_MALLOC_TIMES (MEMORY_LENGTH / 5)

u8 my_free(void *p);
void *my_malloc(size_t length);
u16 get_heap_remaining_size(void);
u16 get_max_alloc_size_at_once(void);

#endif
