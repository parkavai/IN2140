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
    struct inode* dir_root  = create_dir( root, "root" );
    struct inode* dir_home  = create_dir( root, "home" );
    struct inode* dir_guest = create_dir( dir_home, "guest" );
    struct inode* dir_user  = create_dir( dir_home, "user" );
    struct inode* dir_down  = create_dir( dir_user, "Download" );
    create_dir( dir_home, "print" );

    create_file( dir_root,  "bashrc", 1, 100 );
    create_file( dir_root,  "profile", 1, 100 );
    create_file( dir_guest, "bashrc", 1, 100 );
    create_file( dir_guest, "profile", 1, 100 );
    create_file( dir_user,  "bashrc", 1, 100 );
    create_file( dir_user,  "profile", 1, 100 );
    create_file( dir_down,  "oblig2", 1, 163033 );

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

