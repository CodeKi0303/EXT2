////////////////////////////////////////////////////////////////
//  ext2.h
//  EXT2
//
//  Created by Tei Hong on 2018. 2. 6..
//  Copyright © 2018년 Tei Hong. All rights reserved.
////////////////////////////////////////////////////////////////

#ifndef ext2_h
#define ext2_h


#include "common.h"
#include "disk.h"
#include "clusterlist.h"

#define EXT2                    0
#define EXT2_N_BLOCKS           1
#define BLOCKSIZE 1024//bytes
#define    BITMAP_INDEX                BLOCKSIZE/4

typedef struct
{
    BYTE    driveNumber;
    BYTE    reserved1;
    BYTE    bootSignature;
    DWORD    volumeID;
    BYTE    volumeLabel[11];
    BYTE    filesystemType[8];
} EXT2_BOOTSECTOR;

typedef struct
{
    UINT32 block_bitmap[BITMAP_INDEX];//blocksize / 4
}BLOCK_BITMAP;

typedef struct
{
    UINT32 inode_bitmap[BITMAP_INDEX];//blocksize / 4
}INODE_BITMAP;

typedef struct
{
    UINT16 mode;
    UINT16 uid;
    UINT32 size;
    UINT32 atime;
    UINT32 ctime;
    UINT32 mtime;
    UINT32 dtime;
    UINT16 gid;
    UINT16 links_count;
    
    UINT32 blocks;
    UINT32 flags;
    UINT32 i_reserved1;
    UINT32 block[EXT2_N_BLOCKS];
    UINT32 generation;
    UINT32 file_acl;
    UINT32 dir_acl;
    UINT32 faddr;
    UINT32 i_reserved2[3];
}INODE;

typedef struct
{
    UINT32 start_block_of_block_bitmap;
    UINT32 start_block_of_inode_bitmap;
    UINT32 start_block_of_inode_table;
    UINT16 free_blocks_count;
    UINT16 free_inodes_count;
    UINT16 directories_count;
    BYTE padding[2];
    BYTE reserved[12];
}EXT2_GROUP_DESCRIPTOR;

typedef struct
{
    EXT2_GROUP_DESCRIPTOR group_descriptor[32];
}EXT2_GROUP_DESCRIPTOR_BLOCK;

typedef struct
{
    UINT32 max_inode_count;
    UINT32 block_count;
    UINT32 reserved_block_count;
    UINT32 free_block_count;
    UINT32 free_inode_count;
    
    UINT32 first_data_block;
    UINT32 log_block_size;
    UINT32 log_fragmentation_size;
    UINT32 block_per_group;
    UINT32 fragmentation_per_group;
    UINT32 inode_per_group;
    UINT16 magic_signature;
    UINT16 errors;
    UINT32 first_non_reserved_inode;
    UINT16 inode_structure_size;
    
    UINT16 block_group_number;
    UINT32 fist_data_block;
}EXT2_SUPER_BLOCK;


typedef struct
{
    INODE inode[8];
}EXT2_INODE_TABLE;

typedef struct
{
    UINT32 inode;
    BYTE name[11];
    UINT32 name_len;
}EXT2_DIR_ENTRY;

typedef struct
{
    EXT2_SUPER_BLOCK sb;
    EXT2_GROUP_DESCRIPTOR gd;
    DISK_OPERATIONS*    disk;
}EXT2_FILESYSTEM;

typedef struct
{
    UINT32    group;
    UINT32    block;
    UINT32    offset;        /* in the sector */
}EXT2_ENTRY_LOCATION;

typedef struct
{
    EXT2_FILESYSTEM*        fs;
    EXT2_DIR_ENTRY        entry;
    EXT2_ENTRY_LOCATION    location;
} EXT2_NODE;


//typedef int ( *FAT_NODE_ADD )( void*, FAT_NODE* );

void ext_umount(EXT2_FILESYSTEM* fs);
int ext_read_superblock(EXT2_FILESYSTEM* fs, EXT2_NODE* root);
//int ext_read_dir( EXT2_NODE* dir, EXT2_NODE_ADD adder, void* list );
//int ext_mkdir( const EXT2_NODE* parent, const char* entryName, EXT2_NODE* retEntry );
//int ext_rmdir( EXT2_NODE* node );
//int ext_lookup( EXT2_NODE* parent, const char* entryName, EXT2_NODE* retEntry );
//int ext_create( EXT2_NODE* parent, const char* entryName, EXT2_NODE* retEntry );
int ext_read(EXT2_NODE* file, unsigned long offset, unsigned long length, char* buffer);
int ext_write(EXT2_NODE* file, unsigned long offset, unsigned long length, const char* buffer);
int ext_remove(EXT2_NODE* file);
int ext_df(EXT2_FILESYSTEM* fs, UINT32* totalSectors, UINT32* usedSectors);

#endif

