#ifndef BUDDY16_H
#define BUDDY16_H 


typedef char           S8;
typedef short          S16;
typedef long           S32;
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned long  U32;

#ifndef NULL
#define NULL           ((void *)0)
#endif

/**
 * the buddy buffer not only includes memory for allocation, but also an extra memory for 
 * allocation management, the size of this memory depends on the size of the allocation 
 * buffer, and the minimal size of allocation, this macro return the total size of buddy 
 * buffer.
 * for example, if you want the buddy manage a 512 bytes buffer for allocation, and the 
 * minimal allocation size is 16 bytes, the size of memory management in this case will be 
 * ((512 / 16 ) >> 2) + 4 = 12 bytes. so the actual size of the buddy buffer should be 
 * BUDDY_SIZE(512, 16) = 512 + 12 = 524 bytes. 
 * the minimal management size is 8 bytes.
 */

#define MANAGE_SIZE(MEMORY_SIZE, MIN_ALLOC_SIZE)  \
    (((((MEMORY_SIZE)/(MIN_ALLOC_SIZE)) >> 2) + 4) < 8 ? 8 : \
    ((((MEMORY_SIZE)/(MIN_ALLOC_SIZE)) >> 2) + 4))

#define BUDDY_SIZE(MEMORY_SIZE, MIN_ALLOC_SIZE)  \
    ((MEMORY_SIZE) + MANAGE_SIZE(MEMORY_SIZE, MIN_ALLOC_SIZE))

/**
 * Initialize the buddy buffer
 * @param buffer        memory address of the buddy buffer
 * @param buffer_size   the total size of the buddy buffer, include size for allocation
 *                      and size for management.
 * @param min_alloc_size  the minimal allocation size, if alloc_size in buddy_alloc() function 
 *                      less than min_alloc_size, it will allocate a min_alloc_size buffer.
 * @return  0 - on succeeded, < 0 on failed.
 */
int buddy_init(S8 *buffer, U16 buffer_size, U16 min_alloc_size);

/** 
 * Alloc memory from the buddy buffer.
 * @param buffer        the buddy buffer address
 * @param alloc_size    allocate size from buddy buffer.
 * @return   NULL - out of memory, others - address of the allocate memory.
 */
S8* buddy_alloc(S8 *buffer, U16 alloc_size);

/**
 * Free the memory back to the buddy buffer.
 * @param buffer        the 
 * @param tofree        the address of memory to free, which returned by buddy_alloc().
 * @return              0 - on succeeded, <0 on failed.
 */
int buddy_free(S8 *buffer, S8 *tofree);

/**
 * dump the buddy tree, for test only.
 */
void buddy_dump(S8 *buffer);

/**
 * check if the whole memory is free.
 */
int buddy_is_empty(S8 *buffer);

#endif