#ifndef INODE_H
#define INODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is the inode structure as described in the
 * assignment.
 * It is mostly straightforward, but keep in mind
 * that the dynamically allocated array entries
 * contains values that you must interpret as pointers
 * when is_directory==1 and that you must interpret
 * as block numbers when is_directory==0.
 */
struct inode
{
	int id;
	int name_len;
	char *name;
	char is_directory;
	char is_readonly;
	int filesize;
	int num_entries;
	size_t* entries;
};

/* Create a file below the inode parent. Parent must
 * be a directory. The size of the file is size_in_bytes,
 * and create_file calls the allocate_block() function
 * enough number of times to reserve blocks in the simulated
 * disk to store all of these bytes.
 * Returns a pointer to file's inodes.
 */
struct inode* create_file( struct inode* parent, char* name, char readonly, int size_in_bytes );

/* Create a directory below the inode parent. Parent must
 * be a directory.
 * Returns a pointer to file's inodes.
 */
struct inode* create_dir( struct inode* parent, char* name );

/* Check all the inodes that are directly referenced by
 * the node parent. If one of them has the name "name",
 * its inode pointer is returned.
 * parent must be directory.
 */
struct inode* find_inode_by_name( struct inode* parent, char* name );

/* Read the file superblock and create an inode in memory
 * for every inode that is stored in the file. Set the pointers
 * between inodes correctly.
 * The file superblock remains unchanged.
 */
struct inode* load_inodes();

/* This function releases all dynamically allocated memory
 * recursively for all the referenced inodes are visited
 * and their memory is released, finally also for the
 * node that is passed as a parameter.
 * The simulated disk and the file superblock are
 * not changed.
 */
void fs_shutdown( struct inode* node );

/* This function is handed out.
 *
 * It prints the node that is receives as a parameter,
 * and recurivesly all inodes that are stored below it.
 */
void debug_fs( struct inode* node );

#endif

