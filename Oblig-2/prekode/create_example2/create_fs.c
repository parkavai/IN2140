#include "inode.h"
#include "allocation.h"

#include <stdio.h>

int main()
{
    /* format_disk() makes sure that the simulation
     * disk is empty. It creates a file named
     * block_allocation_table that contains only
     * zeros. */
    format_disk();

    /* debug_disk() write the current content of the
     * block_allocation_table that simulates whether
     * blocks on disk contain file data (1) or not (0).
     */
    debug_disk();

    struct inode* root      = create_dir( NULL, "/" );
    struct inode* dir_etc   = create_dir( root, "etc" );
    struct inode* dir_share = create_dir( root, "share" );
    struct inode* dir_man   = create_dir( dir_share, "man" );
    struct inode* dir_var   = create_dir( root, "var" );
    struct inode* dir_log   = create_dir( dir_var, "log" );

    create_file( root,    "kernel", 1, 20000 );
    create_file( dir_log, "message", 1, 50000 );
    create_file( dir_log, "warn", 1, 50000 );
    create_file( dir_log, "fail", 1, 50000 );
    create_file( dir_etc, "hosts", 1, 200 );
    create_file( dir_man, "read.2", 1, 300 );
    create_file( dir_man, "write.2", 1, 400 );

    debug_fs( root );
    debug_disk();

    fs_shutdown( root );

    printf("+++++++++++++++++++++++++++++++++++++++++++\n");
    printf("+ All inodes structures have been         +\n");
    printf("+ deleted. The inode info is stored in    +\n");
    printf("+ superblock. The allocated file blocks   +\n");
    printf("+ are stored in block_allocation_table    +\n");
    printf("+++++++++++++++++++++++++++++++++++++++++++\n");
}

