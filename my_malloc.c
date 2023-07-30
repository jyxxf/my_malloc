#include "my_malloc.h"
#include <string.h>

u8 memory[MEMORY_LENGTH];

struct
{
	struct
	{
		u16 index;
		u16 length;
	} config[MAX_MALLOC_TIMES];
	u16 current_index; // 在这之前所有index均已经分配
} table;

void *my_malloc(size_t length)
{
	if (!length)
		return 0;
	if (table.current_index == MAX_MALLOC_TIMES) // 满了
		return 0;

	if (table.config[0].index >= length)
	{
		memmove(&table.config[1], &table.config[0], table.current_index * sizeof(table.config[0]));
		table.config[0].index = 0;
		table.config[0].length = length;
		table.current_index++;
		return memory;
	}

	for (size_t i = 0; i < MAX_MALLOC_TIMES - 1 && table.config[i + 1].index; i++)
	{
		if (table.config[i + 1].index - (table.config[i].index + table.config[i].length) >= length)
		{
			// 找到
			memmove(&table.config[i + 2], &table.config[i + 1], (table.current_index - i - 1) * sizeof(table.config[0]));
			table.config[i + 1].index = table.config[i].index + table.config[i].length;
			table.config[i + 1].length = length;
			table.current_index++;
			return &memory[table.config[i + 1].index];
		}
	}

	// 到结尾还没找到
	if (MEMORY_LENGTH - (table.config[table.current_index - 1].index + table.config[table.current_index - 1].length) >= length)
	{
		table.config[table.current_index].index = table.config[table.current_index - 1].index + table.config[table.current_index - 1].length;
		table.config[table.current_index].length = length;
		return &memory[table.config[table.current_index++].index];
	}
	else
		return 0;
}

/**
 * @brief
 *
 * @param p
 * @return 1表示释放成功
 */
u8 my_free(void *p)
{
	u16 index = p - (void *)memory;
	for (size_t i = 0; i < table.current_index; i++)
	{
		if (table.config[i].index == index)
		{
			memmove(&table.config[i], &table.config[i + 1], sizeof(table.config[0]) * (table.current_index - i - 1));
			memset(&table.config[table.current_index - 1], 0, sizeof(table.config[0]));
			table.current_index--;
			return 1;
		}
		else if (table.config[i].index > index)
			return 0;
	}
	return 0;
}

u16 get_heap_remaining_size(void)
{
	u16 i = 0, sum = 0;
	while (table.config[i].length)
	{
		sum += table.config[i].length;
		i++;
	}
	return MEMORY_LENGTH - sum;
}

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

u16 get_max_alloc_size_at_once(void)
{
	u16 size = 0, i = 0;
	while (i < MAX_MALLOC_TIMES - 1 && table.config[i + 1].length)
	{
		size = MAX(size, table.config[i + 1].index - (table.config[i].index + table.config[i].length));
		i++;
	}
	size = MAX(size, MEMORY_LENGTH - (table.config[i].index + table.config[i].length));
	return size;
}
