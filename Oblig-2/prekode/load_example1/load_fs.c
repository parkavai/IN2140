#include "inode.h"
#include "allocation.h"

#include <stdio.h>

int main()
{
    printf("===================================\n");
    printf("= Load all inodes from the file   =\n");
    printf("= superblock                      =\n");
    printf("===================================\n");
    struct inode* root = load_inodes();
    debug_fs( root );
    debug_disk();

    printf("===================================\n");
    printf("= Trying to find some files.      =\n");
    printf("===================================\n");
    struct inode* dir;

    dir = root;
    dir = find_inode_by_name( dir, "kernel" );
    if( dir ) printf("Found /kernel\n");

    dir = root;
    dir = find_inode_by_name( dir, "var" );
    if( dir ) dir = find_inode_by_name( dir, "log" );
    if( dir ) dir = find_inode_by_name( dir, "message" );
    if( dir ) printf("Found /var/log/messages\n");

    dir = root;
    dir = find_inode_by_name( dir, "share" );
    if( dir ) dir = find_inode_by_name( dir, "man" );
    if( dir ) dir = find_inode_by_name( dir, "read.2" );
    if( dir ) printf("Found /share/man/read.2\n");

    dir = root;
    dir = find_inode_by_name( dir, "etc" );
    if( dir ) dir = find_inode_by_name( dir, "hosts" );
    if( dir ) printf("Found /etc/hosts\n");

    dir = root;
    dir = find_inode_by_name( dir, "etc" );
    if( dir ) dir = find_inode_by_name( dir, "host.conf" );
    if( dir ) printf("Found /etc/host.conf\n");

    dir = root;
    dir = find_inode_by_name( dir, "etc" );
    if( dir ) dir = find_inode_by_name( dir, "httpd" );
    if( dir ) dir = find_inode_by_name( dir, "conf" );
    if( dir ) printf("Found /etc/httpd/conf\n");

    dir = root;
    dir = find_inode_by_name( dir, "home" );
    if( dir ) dir = find_inode_by_name( dir, "user" );
    if( dir ) dir = find_inode_by_name( dir, "Download" );
    if( dir ) dir = find_inode_by_name( dir, "oblig2" );
    if( dir ) printf("Found /home/user/Download/oblig2\n");

    dir = root;
    dir = find_inode_by_name( dir, "home" );
    if( dir ) dir = find_inode_by_name( dir, "guest" );
    if( dir ) dir = find_inode_by_name( dir, "bashrc" );
    if( dir ) printf("Found /home/guest/bashrc\n");

    dir = root;
    dir = find_inode_by_name( dir, "root" );
    if( dir ) dir = find_inode_by_name( dir, "bashrc" );
    if( dir ) printf("Found /root/bashrc\n");

    fs_shutdown( root );
}

