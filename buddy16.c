#include "buddy16.h"


#define IS_POWER2(A) (!((A) & ((A) -1)))

/* 
 * return the number of power of 2 which just more than(or equal to) 
 * the given number. eg. EXTEND_POWER2(5) = 8, EXTEND_POWER2(10) = 16
 * EXTEND_POWER2(8) = 8
 */
#define EXTEND_POWER2(A) (IS_POWER2(A) ? (A) :  \
	((((((A) | (A)>>1) | ((A) >> 2)) | ((A) >> 4)) | ((A) >> 8)) + 1))

/* 
 * return the number of power of 2 which just less than(or equal to) 
 * the given number. eg. FOLD_POWER2(5) = 4, EXTEND_POWER2(10) = 8
 * FOLD_POWER2(8) = 8
 */
#define FOLD_POWER2(A) (IS_POWER2(A) ? (A) :  \
	(((((((A) | (A)>>1) | ((A) >> 2)) | ((A) >> 4)) | ((A) >> 8)) + 1)>>1))


struct buddy {
	U16 mem_size;
	U16 min_alloc_size;
	U16 tree[2];
};

int buddy_init(S8 *buffer, U16 buffer_size, U16 min_alloc_size)
{
	U16 i;
	U16 mem_size  = FOLD_POWER2(buffer_size);
	U16 manage_size = MANAGE_SIZE(mem_size, min_alloc_size);
	struct buddy *buddy = (struct buddy *)buffer;

	ASSERT(buffer_size >= BUDDY_SIZE(alloc_size));
	if (buffer_size < BUDDY_SIZE(alloc_size)) 
		return -1;

	ASSERT(buffer_size > min_alloc_size);
	if (buffer_size <= min_alloc_size) 
		return -1;

	for (i = 0; i < manage_size; i++) 
		buffer_size[i] = 0;

	buddy->mem_size = mem_size;
	buddy->min_alloc_size = min_alloc_size;
	buddy->tree[0] = 1;

	return 0;
}

/** 
 * @param buddy 		buddy pointer
 * @param alloc_size 	allocation size
 * @return 				the free buddy's bit offset in the tree bitmap. 
 */
static U16 buddy_alloc_level(struct buddy *buddy, U16 alloc_size)
{
	U8 try_upper_level;
	U16 i, level_bit_offset, level_bit_length;
	U16 word_index, bit_index;
	U16 bitmap, pattern;

	level_bit_offset = buddy->mem_size / alloc_size;
	level_bit_length = level_bit_offset;

	/* find in this level if there is '01', '10', which means one of the buddy 
	 * has been used, and the other one is free, we just allocate the free one 
	 * and return it. if there is patten '00', there might be 2 sinarioes, one is 
	 * that two buddies are free, or 2 buddies have been used, these depend on the 
	 * upper level state. if all pattern are '11', this means all buddy in this 
	 * level have been used, it's out of memory, return NULL.
	 * ps: 0 - free, 1 - used.
	 */
	word_index = level_bit_offset >> 4;
	bit_index  = level_bit_offset & 0xf;
	bitmap = (buddy->tree[word_index] >> bit_index)
	for (i = 0; i < level_bit_length; i += 2) {
		pattern = bitmap & 0x3;
		switch(pattern) {
		case 0x0:
			try_upper_level = 1;
			break;
		case 0x1: /* pattern '01' */
			return (level_bit_offset + i + 1);
		case 0x2: /* pattern '10' */
			return (level_bit_offset + i);
		default:
			break;
		}

		bit_index += 2;
		if (bit_index >= 16) {
			word_index++;
			bit_index = 0;
			bitmap = buddy->tree[word_index];
		}
	}

	if (buddy->mem_size == alloc_size)
		return 0;

	if (try_upper_level) 
		return buddy_alloc_level(buddy, alloc_size<<1)

	return 0;
}

S8* buddy_alloc(S8 *buffer, U16 alloc_size)
{		
	U16 extend_size, buffer_offset;
	U16 buddy_offset, buddy_size, fold_offset;
	U16 word_index, bit_index;
	struct buddy *buddy = (struct buddy *)buffer;
	U16 manage_size = MANAGE_SIZE(buddy->mem_size, buddy->min_alloc_size);

	if (alloc_size < buddy->min_alloc_size)
		extend_size = buddy->min_alloc_size;
	else
		extend_size = EXTEND_POWER2(alloc_size);

	buddy_offset = buddy_alloc_level(buddy, extend_size);
	if (!buddy)
		return NULL;

	fold_pow2 = FOLD_POWER2(buddy_offset);
	buddy_size = buddy->mem_size / fold_pow2;
	buffer_offset = (buddy_offset - fold_pow2) * buddy_size;
	do {
		word_index = buddy_offset >> 4;
		bit_index  = buddy_offset & 0xf;
		buddy->tree[word_index] |= 1 << bit_index;
		if (buddy_size == extend_size) 
			break;
		buddy_size >>= 1;
		buddy_offset <<= 1;
	} while (1);

	return (S8 *)(buffer + manage_size + buffer_offset);
}

int buddy_free(S8 *buffer, S8* tofree)
{
	struct buddy *buddy = (struct buddy *)buffer;
	U16 manage_size = MANAGE_SIZE(buddy->mem_size, buddy->min_alloc_size);
	U16 buffer_offset = free - buffer - manage_size;


}
