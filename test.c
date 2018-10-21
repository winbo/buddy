#include <stdio.h>
#include "buddy16.h"


S8 mem_pool[BUDDY_SIZE(32, 4)];
void _assert(int condition, const char *file, int line);
#define ASSERT(A) _assert(A, __FILE__, __LINE__)

/* 
 * return the number of power of 2 which just more than(or equal to) 
 * the given number. eg. upper_pow2(5) = 8, upper_pow2(10) = 16
 * upper_pow2(8) = 8
 */

static U16 upper_pow2(U16 num) 
{
    if (!(num & (num - 1)))
        return num;

    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    return num + 1;
}

/* 
 * return the number of power of 2 which just less than(or equal to) 
 * the given number. eg. lower_pow2(5) = 4, upper_pow2(10) = 8
 * lower_pow2(8) = 8
 */
static U16 lower_pow2(U16 num)
{
    if (!(num & (num - 1)))
        return num;

    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    return (num + 1) >> 1;
}

int main(int argc, char *argv[])
{
	int ret;
	S8 *p1, *p2, *p3, *p4;

	ASSERT(upper_pow2(5) == 8);
	ASSERT(upper_pow2(10) == 16);
	ASSERT(upper_pow2(8) == 8);
	ASSERT(upper_pow2(40) == 64);
	ASSERT(upper_pow2(65) == 128);

	ASSERT(lower_pow2(5) == 4);
	ASSERT(lower_pow2(10) == 8);
	ASSERT(lower_pow2(8) == 8);
	ASSERT(lower_pow2(40) == 32);
	ASSERT(lower_pow2(65) == 64);
	ASSERT(lower_pow2(255) == 128);

	ret = buddy_init(mem_pool, sizeof(mem_pool), 4);
	ASSERT(ret == 0);
	printf("buddy tree after init\n");
	buddy_dump(mem_pool);

	p1 = buddy_alloc(mem_pool, 3);
	ASSERT(p1 != NULL);
	ASSERT(p1 > mem_pool && p1 < (mem_pool + sizeof(mem_pool)));
	printf("buddy tree after alloc 3 bytes\n");
	buddy_dump(mem_pool);

	p2 = buddy_alloc(mem_pool, 8);
	ASSERT(p2 != NULL);
	ASSERT(p2 > mem_pool && p2 < (mem_pool + sizeof(mem_pool)));
	printf("buddy tree after alloc 8 bytes\n");
	buddy_dump(mem_pool);

	p3 = buddy_alloc(mem_pool, 1);	
	ASSERT(p3 != NULL);
	ASSERT(p3 > mem_pool && p3 < (mem_pool + sizeof(mem_pool)));
	printf("buddy tree after alloc 1 byte\n");
	buddy_dump(mem_pool);

	p4 = buddy_alloc(mem_pool, 15);	
	ASSERT(p4 != NULL);
	ASSERT(p4 > mem_pool && p4 < (mem_pool + sizeof(mem_pool)));
	printf("buddy tree after alloc 15 byte\n");
	buddy_dump(mem_pool);

	ASSERT(buddy_alloc(mem_pool, 1) == NULL);

	buddy_free(mem_pool, p1);
	printf("buddy tree after free 3 bytes\n");
	buddy_dump(mem_pool);
	printf("buddy tree after free 1 byte\n");	
	buddy_free(mem_pool, p3);
	buddy_dump(mem_pool);

	buddy_free(mem_pool, p2);
	printf("buddy tree after free 8 bytes\n");
	buddy_dump(mem_pool);

	return 0;
}
void _assert(int condition, const char *file, int line)
{
	if (!condition) {
		printf("ASSERT failed: %s(%d)\n", file, line);
	}
}
