# buddy
This is a buddy memory management module designed specifically for MCU system. It manages less than 64K bytes of memory, and it costs very few memory for allocation management.

## Getting start
Before using this module, you need to determine how much memory pool that your module needed, and the minimal block size allocated from pool, the size of memory pool and the size of minimal block should be 2 power value. for example, the memory pool is 2048 bytes, minimal block size is 32 bytes.

	#include "buddy16.h"

	S8 mem_pool[BUDDY_SIZE(2048, 32)];

Then, you need to initialize the memory pool as following,

	buddy_init(mem_pool, sizeof(mem_pool), 32);

After initialization, you can allocate memory by buddy_alloc(), as following.

	S8 *p = buddy_alloc(mem_pool, 45);     /* allocate 45 bytes buffer */

And last, free it after the buffer is not needed any more, as following,

	buddy_free(mem_pool, p);



I wish it can help you. :)

- Winbo (lu.winbo@qq.com)



