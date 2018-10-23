#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buddy16.h"


#define MEM_SIZE1        64
#define MIN_ALLOC_SIZE1  4

S8 mem_pool1[BUDDY_SIZE(MEM_SIZE1, MIN_ALLOC_SIZE1)];

#define MEM_SIZE2       32*1024
#define MIN_ALLOC_SIZE2 32

S8 mem_pool2[BUDDY_SIZE(MEM_SIZE2, MIN_ALLOC_SIZE2)];


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

    ret = buddy_init(mem_pool1, sizeof(mem_pool1), MIN_ALLOC_SIZE1);

    ASSERT(ret == 0);
    printf("buddy tree after init\n");
    buddy_dump(mem_pool1);

    p1 = buddy_alloc(mem_pool1, l1);
    printf("buddy tree after alloc %d bytes, address=%p\n", l1, p1);
    buddy_dump(mem_pool1);
    ASSERT(p1 != NULL);
    ASSERT(in_section(mem_pool1, sizeof(mem_pool1), p1, l1));

    p2 = buddy_alloc(mem_pool1, l2);
    printf("buddy tree after alloc %d bytes, address=%p\n", l2, p2);
    buddy_dump(mem_pool1);
    ASSERT(p2 != NULL);
    ASSERT(in_section(mem_pool1, sizeof(mem_pool1), p2, l2));
    ASSERT(!inter_section(p1, l1, p2, l2));

    p3 = buddy_alloc(mem_pool1, l3); 
    printf("buddy tree after alloc %d byte, address=%p\n", l3, p3);
    buddy_dump(mem_pool1);
    ASSERT(p3 != NULL);
    ASSERT(in_section(mem_pool1, sizeof(mem_pool1), p3, l3));
    ASSERT(!inter_section(p1, l1, p3, l3));
    ASSERT(!inter_section(p2, l2, p3, l3));

    p4 = buddy_alloc(mem_pool1, l4); 
    printf("buddy tree after alloc %d byte, address=%p\n", l4, p4);
    buddy_dump(mem_pool1);
    ASSERT(p4 != NULL);
    ASSERT(in_section(mem_pool1, sizeof(mem_pool1), p4, l4));
    ASSERT(!inter_section(p1, l1, p4, l4));
    ASSERT(!inter_section(p2, l2, p4, l4));
    ASSERT(!inter_section(p3, l3, p4, l4));

    p5 = buddy_alloc(mem_pool1, l5); 
    printf("buddy tree after alloc %d byte, address=%p\n", l5, p5);
    buddy_dump(mem_pool1);
    ASSERT(p5 != NULL);
    ASSERT(in_section(mem_pool1, sizeof(mem_pool1), p5, l5));
    ASSERT(!inter_section(p1, l1, p5, l5));
    ASSERT(!inter_section(p2, l2, p5, l5));
    ASSERT(!inter_section(p3, l3, p5, l5));
    ASSERT(!inter_section(p4, l4, p5, l5));

    ASSERT(buddy_alloc(mem_pool1, 1) == NULL);

    buddy_free(mem_pool1, p1);
    printf("buddy tree after free %d bytes\n", l1);
    buddy_dump(mem_pool1);

    buddy_free(mem_pool1, p2);
    printf("buddy tree after free %d byte\n", l2);  
    buddy_dump(mem_pool1);

    buddy_free(mem_pool1, p3);
    printf("buddy tree after free %d bytes\n", l3);
    buddy_dump(mem_pool1);

    buddy_free(mem_pool1, p4);
    printf("buddy tree after free %d bytes\n", l4);
    buddy_dump(mem_pool1);

    buddy_free(mem_pool1, p5);
    printf("buddy tree after free %d bytes\n", l5);
    buddy_dump(mem_pool1);

    ASSERT(buddy_is_empty(mem_pool1));

    return 0;
}


int test_case2()
{
    int i, j, k;
    int loop_count;
    int alloc_count;
    int free_count;
    /* random test */
    S8 *p1[200];
    S8 *p2[200];
    int size[200];
    int index = 0;

    srand(0x1234567);

    ASSERT(buddy_init(mem_pool2, sizeof(mem_pool2), MIN_ALLOC_SIZE2) == 0);

    loop_count = rand() % 200;
    for (i = 0; i < loop_count; i++) {
        alloc_count = rand() % 50;
        for (j = 0; j < alloc_count; j++) {
            size[index] = rand() % (MEM_SIZE2>>4);
            p1[index] = buddy_alloc(mem_pool2, size[index]);
            p2[index] = (S8 *)malloc(size[index]);
            if (p1[index]) {
                printf("alloc index = %d, size = %d, p = %p\n", index, size[index], p1[index]);
                buddy_dump(mem_pool2);
                for (k = 0; k < size[index]; k++) {
                    p2[index][k] = rand() % 0xff;
                }
                memcpy(p1[index], p2[index], size[index]);
                index++;
            } else {
                if (p2[index])
                    free(p2[index]);
            }
        }

        for (j = 0; j < index; j++) {
            ASSERT(memcmp(p1[j], p2[j], size[j]) == 0);
        }

        free_count = rand() % 50;
        index--;
        for (j = 0; j < free_count && index >= 0; j++) {
            printf("free index = %d size = %d, p = %p\n", index, size[index], p1[index]);
            ASSERT(buddy_free(mem_pool2, p1[index]) == 0);
            buddy_dump(mem_pool2);
            free(p2[index]);
            index--;
        }
        index++;
        if (index == 0)
            ASSERT(buddy_is_empty(mem_pool2));
    }

    index--;
    while (index >= 0) {
        printf("free index = %d size = %d, p = %p\n", index, size[index], p1[index]);
        ASSERT(buddy_free(mem_pool2, p1[index]) == 0);
        buddy_dump(mem_pool2);
        free(p2[index]);
        index--;
    }
    ASSERT(buddy_is_empty(mem_pool2));


    return 0;
}

int main(int argc, char *argv[])
{
    test_case1();
    test_case2();
    printf("=====Pass=======\n");
    return 0;
}