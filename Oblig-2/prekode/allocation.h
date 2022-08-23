#ifndef ALLOCATION_H
#define ALLOCATION_H

/* Set all the blocks in our simulated disk into an unused
 * state.
 * This function returns 0 in case of success and -1 if the
 * file simulating the blocks cannot be written.
 */
int format_disk();

/* Allocate exactly one block from the available free disk blocks.
 * Disk blocks are counted from 0 to max.
 * The function can return -1 if no block is available.
 */
int allocate_block();

/* Free the block with the given ID.
 * This functions returns 0 if the block was freed
 * or -1 if the block with this ID was not allocated
 */
int free_block(int block);

/* This debug function prints the table to stdout. */
void debug_disk();

#endif // ALLOCATION_H
