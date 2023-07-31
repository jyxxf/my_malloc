#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "my_malloc.h"
u16 get_random(u16 max);
u8 whether_the_same_value(const void *p, u8 value, size_t length);

int main(void)
{
	srand(time(NULL));
	FILE *file = fopen("log", "w");

	struct
	{
		void *p;
		u16 length;
	} test[MEMORY_LENGTH];
	memset(test, 0, sizeof(test));

	size_t i = 0, malloc_index = 0, free_index = 0;
	while (i < MEMORY_LENGTH)
	{
		if (i % 3 == 0 || i % 3 == 1)
		{
			u16 temp = get_random(10000);
			test[malloc_index].p = my_malloc(temp);
			if (test[malloc_index].p)
			{
				fprintf(file, "malloc %d success\n", temp);
				test[malloc_index].length = temp;
				memset(test[malloc_index].p, test[malloc_index].length, test[malloc_index].length);
				malloc_index++;
			}
			else
			{
				fprintf(file, "malloc %d fail", temp);
				if (get_max_alloc_size_at_once() >= temp)
				{
					fprintf(file, " FATAL ERROR! can malloc %d size at once but fail", get_max_alloc_size_at_once());
					return 0;
				}
				else
				{
					fprintf(file, " because only can malloc %d size at once\n", get_max_alloc_size_at_once());
				}
			}
		}
		else
		{
			if (test[free_index].p)
			{
				if(!whether_the_same_value(test[free_index].p, test[free_index].length, test[free_index].length))
				{
					fprintf(file, "FATAL ERROR! The value is wrong!");
					return 0;
				}

				if (!my_free(test[free_index].p))
				{
					fprintf(file, "FATAL ERROR!");
					return 0;
				}
				else
				{
					fprintf(file, "free %d well\n\n", test[free_index].length);
				}
				free_index++;
			}
		}
		i++;
	}
	printf("ok");
	return 0;
}

/**
 * @brief if the size ram is the same value return 1
 *
 * @param p
 * @param value
 * @param length
 * @return u8
 */
u8 whether_the_same_value(const void *p, u8 value, size_t length)
{
	while (length--)
	{
		if (((u8 *)p)[length] != value)
			return 0;
	}
	return 1;
}

/**
 * @brief Get the random object 1~max
 *
 * @param max
 * @return u16
 */
u16 get_random(u16 max)
{
	while (1)
	{
		u32 temp = rand();
		if (temp >= RAND_MAX / 10000 * 10000)
			continue;
		else
		{
			temp %= 10000;
			return ++temp;
		}
	}
}
