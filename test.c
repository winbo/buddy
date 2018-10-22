#include <stdio.h>
#include <stdlib.h>
#include "buddy16.h"


#define MEM_SIZE    64
#define MIN_ALLOC_SIZE  4

S8 mem_pool[BUDDY_SIZE(MEM_SIZE, MIN_ALLOC_SIZE)];


void _assert(int condition, const char *file, int line)
{
	if (!condition) {
		printf("ASSERT failed: %s(%d)\n", file, line);
		exit(-1);
	}
}

#define ASSERT(A) _assert(A, __FILE__, __LINE__)


int inter_section(S8 *p1, U16 length1, S8 *p2, U16 length2)
{
	if (p1 >= p2 && p1 < (p2 + length2)) 
		return 1;
	else if (p2 >= p1 && p2 < (p1 + length1))
		return 1;
	else 
		return 0;
	
}

int in_section(S8 *p1, U16 length1, S8 *p2, U16 length2)
{
	if (p2 >= p1 && (p2 + length2) <= (p1 + length1))
		return 1;
	else 
		return 0;
}


int test_case1()
{
	int ret;
	S8 *p1, *p2, *p3, *p4, *p5;
	U16 l1 = 32;
	U16 l2 = 1;
	U16 l3 = 10;
	U16 l4 = 3;
	U16 l5 = 8;

	ret = buddy_init(mem_pool, sizeof(mem_pool), MIN_ALLOC_SIZE);

	ASSERT(ret == 0);
	printf("buddy tree after init\n");
	buddy_dump(mem_pool);

	p1 = buddy_alloc(mem_pool, l1);
	printf("buddy tree after alloc %d bytes, address=%p\n", l1, p1);
	buddy_dump(mem_pool);
	ASSERT(p1 != NULL);
	ASSERT(in_section(mem_pool, sizeof(mem_pool), p1, l1));

	p2 = buddy_alloc(mem_pool, l2);
	printf("buddy tree after alloc %d bytes, address=%p\n", l2, p2);
	buddy_dump(mem_pool);
	ASSERT(p2 != NULL);
	ASSERT(in_section(mem_pool, sizeof(mem_pool), p2, l2));
	ASSERT(!inter_section(p1, l1, p2, l2));

	p3 = buddy_alloc(mem_pool, l3);	
	printf("buddy tree after alloc %d byte, address=%p\n", l3, p3);
	buddy_dump(mem_pool);
	ASSERT(p3 != NULL);
	ASSERT(in_section(mem_pool, sizeof(mem_pool), p3, l3));
	ASSERT(!inter_section(p1, l1, p3, l3));
	ASSERT(!inter_section(p2, l2, p3, l3));

	p4 = buddy_alloc(mem_pool, l4);	
	printf("buddy tree after alloc %d byte, address=%p\n", l4, p4);
	buddy_dump(mem_pool);
	ASSERT(p4 != NULL);
	ASSERT(in_section(mem_pool, sizeof(mem_pool), p4, l4));
	ASSERT(!inter_section(p1, l1, p4, l4));
	ASSERT(!inter_section(p2, l2, p4, l4));
	ASSERT(!inter_section(p3, l3, p4, l4));

	p5 = buddy_alloc(mem_pool, l5);	
	printf("buddy tree after alloc %d byte, address=%p\n", l5, p5);
	buddy_dump(mem_pool);
	ASSERT(p5 != NULL);
	ASSERT(in_section(mem_pool, sizeof(mem_pool), p5, l5));
	ASSERT(!inter_section(p1, l1, p5, l5));
	ASSERT(!inter_section(p2, l2, p5, l5));
	ASSERT(!inter_section(p3, l3, p5, l5));
	ASSERT(!inter_section(p4, l4, p5, l5));

	ASSERT(buddy_alloc(mem_pool, 1) == NULL);

	buddy_free(mem_pool, p1);
	printf("buddy tree after free %d bytes\n", l1);
	buddy_dump(mem_pool);

	buddy_free(mem_pool, p2);
	printf("buddy tree after free %d byte\n", l2);	
	buddy_dump(mem_pool);

	buddy_free(mem_pool, p3);
	printf("buddy tree after free %d bytes\n", l3);
	buddy_dump(mem_pool);

	buddy_free(mem_pool, p4);
	printf("buddy tree after free %d bytes\n", l4);
	buddy_dump(mem_pool);

	buddy_free(mem_pool, p5);
	printf("buddy tree after free %d bytes\n", l5);
	buddy_dump(mem_pool);

	ASSERT(buddy_is_empty(mem_pool));

	return 0;
}

int test_case2()
{
	/* random test */
	return 0;
}

int main(int argc, char *argv[])
{
	test_case1();
	return 0;
}