#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "my_malloc.h"
u16 get_random(u16 max);

int main(void)
{
	srand(time(NULL));
	FILE *file = fopen("log", "w");
	void *test[MEMORY_LENGTH];
	memset(test, 0, sizeof(test));

	size_t i = 0, malloc_index = 0, free_index = 0;
	while (i < MEMORY_LENGTH)
	{
		if (i % 3 == 0 || i % 3 == 1)
		{
			u16 temp = get_random(10000);
			test[malloc_index] = my_malloc(temp);
			if (test[malloc_index])
			{
				fprintf(file, "malloc %d success\n", temp);
				malloc_index++;
			}
			else
			{
				fprintf(file, "malloc %d fail\n", temp);
			}
		}
		else
		{
			if(test[free_index])
			{
				if(!my_free(test[free_index]))
				{
					printf("error");
					return 0;
				}
				else
				{
					fprintf(file, "free well\n\n");
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
