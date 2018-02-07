/***************************************************************************/
//
//  ext2.c
//  EXT2
//
//  Created by Tei Hong on 2018. 2. 6..
//  Copyright © 2018년 Tei Hong. All rights reserved.
/******************************************************************************/
#include "clusterlist.h"
#include "ext2.h"
#define MIN( a, b )                    ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define MAX( a, b )                    ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define NO_MORE_CLUSER()            WARNING( "No more clusters are remained\n" );
#define ROOT_SECTOR_SIZE            1
#define    BITMAP_INDEX                disk->bytesPerSector/4
#define BLOCKBIT                    (BLOCKSIZE * 8)
#define SECTOR_INDEX(offset)            (BLOCKSIZE/disk->bytesPerSector)*(offset)
#define BLOCK_GROUP_INDEX(i)        (ROOT_SECTOR_SIZE + BLOCKBIT * i)


int fill_superblock( EXT2_SUPER_BLOCK *sb,short groupnum,SECTOR numberOfSectors, UINT32 bytesPerSector )
{
   //슈퍼블록만 채운다
    ZeroMemory( sb, sizeof(EXT2_SUPER_BLOCK) );
   //superblock 0으로 리셋
    
    sb->max_inode_count=100*2;//아이노드 100개
    sb->block_count=8192*2+1;
    sb->reserved_block_count=450;
    sb->free_block_count=sb->block_count-100;
    sb->free_inode_count=100*2;
    
    sb->fist_data_block=0;
    sb->log_block_size=0;
    sb->log_fragmentation_size=0;
    sb->block_per_group=8192;
    sb->fragmentation_per_group=0;
    sb->inode_per_group=100;
    sb->magic_signature=0xEF53;
    sb->errors=0;
    sb->first_non_reserved_inode=11;
    sb->inode_structure_size=128;
    sb->block_group_number=groupnum;//글로벌 변수, fillbpb부를때마다 1씩 증가
    sb->fist_data_block=17;
     
    return FAT_SUCCESS;
}

int EXT2_format(DISK_OPERATIONS* disk)
{
    EXT2_SUPER_BLOCK super_block;
    EXT2_GROUP_DESCRIPTOR_BLOCK gdb;
    INODE_BITMAP inode_bitmap;
    BLOCK_BITMAP block_bitmap;
    
    for (short i = 0; i < disk->numberOfSectors / BLOCKBIT; i++)
        //블록 그룹 개수 = 전체 섹터 크기 / (비트맵 표현 가능 블록 == 블록 크기 비트)
        //               = 전체 섹터 크기 / 그룹당 블록 개수
    {
        if (fill_superblock(&super_block, i, disk->numberOfSectors, disk->bytesPerSector) != FAT_SUCCESS)    return FAT_ERROR;
        disk->write_sector(disk, 0, &super_block);
        printf("group %d: fill_super_block success %d \n", i,sizeof(EXT2_SUPER_BLOCK));
        /*
        init_group_des(&disk,&super_block,&gdb);
        disk->write_sector(disk, SECTOR_INDEX(BLOCK_GROUP_INDEX(i) + 1), &block_bitmap);
        printf("group %d: init_GD_ success %d \n", i, sizeof(EXT2_GROUP_DESCRIPTOR_BLOCK));
        
        init_block_bitmap(&block_bitmap, BITMAP_INDEX);
        disk->write_sector(disk, SECTOR_INDEX(BLOCK_GROUP_INDEX(i) + 2), &block_bitmap);
        printf("group %d: init_block_bitmap success %d \n", i, sizeof(INODE_BITMAP));
    
        init_inode_bitmap(&inode_bitmap, BITMAP_INDEX);
        disk->write_sector(disk, SECTOR_INDEX(BLOCK_GROUP_INDEX(i) + 3), &inode_bitmap);
        printf("group %d: init_inode_bitmap success %d \n", i, sizeof(BLOCK_BITMAP));
        */
    }
    
    BYTE dump[1024];
    //memcpy(dump,( ( DISK_MEMORY* )disk->pdata )->address,1024);
    disk->read_sector(disk,0,dump);
    printf("fist 16bytes \n");
   // printf("start address: %x\n",( ( DISK_MEMORY* )disk->pdata )->address);
    for(int i=0;i<16;i++)
    {
        printf("%02d  ",i);
    }
    printf("\n================================================================\n");
    for(int i=0;i<20;i++)
    {
        for(int j=(16*i);j<(16*i)+16;j++)
        {
            printf("%02x  ",dump[j]);
        }
        printf("\n");
    }
    printf("chabged\n");
    printf("fill_super_block success\n");
    
    
    return FAT_SUCCESS;
}
/*
int init_group_des(DISK_OPERATIONS* disk ,EXT2_SUPER_BLOCK* sb, EXT2_GROUP_DESCRIPTOR* gd)
{
    UINT32    i, end;
    BYTE    sector[1024];
    EXT2_GROUP_DESCRIPTOR* entry;
    entry = ( EXT2_GROUP_DESCRIPTOR* )sector;
    
    ZeroMemory( sector, sizeof( sector ) );//섹터를 0으로 초기화
    memset(&gd, 0, sizeof(gd));//그룹디스크립터를 일단 0으로 초기화
    
    gd->start_block_of_block_bitmap=2;//그룹 디스크립터 테이블이 1블록안에 들어가면 2라합니다
    gd->start_block_of_inode_bitmap=gd->start_block_of_block_bitmap+1;
    gd->start_block_of_inode_table=gd->start_block_of_inode_table+1;
    gd->free_blocks_count=sb->block_per_group-17;//17개를 쓰고있으니 17개 뺀다
    gd->free_inodes_count=sb->inode_per_group-10;//일단 안정적으로 10개뺌....
    gd->directories_count=1;//루트디렉토리
    
    for (short i = 0; i < disk->numberOfSectors / (BLOCKSIZE * 8); i++)
    {
        memcpy(entry, gd, sizeof(EXT2_GROUP_DESCRIPTOR));
        entry++;
    }
    
    return FAT_SUCCESS;
    
}*/

int init_group_des(DISK_OPERATIONS* disk ,EXT2_SUPER_BLOCK* sb, EXT2_GROUP_DESCRIPTOR_BLOCK* block)
{
    EXT2_GROUP_DESCRIPTOR gd;
    //entry = ( EXT2_GROUP_DESCRIPTOR* )sector;
    
    ZeroMemory( block, sizeof( EXT2_GROUP_DESCRIPTOR_BLOCK ) );//섹터를 0으로 초기화
    memset(&gd, 0, sizeof(gd));//그룹디스크립터를 일단 0으로 초기화
    
    gd.start_block_of_block_bitmap=2;//그룹 디스크립터 테이블이 1블록안에 들어가면 2라합니다
    gd.start_block_of_inode_bitmap=gd.start_block_of_block_bitmap+1;
    gd.start_block_of_inode_table=gd.start_block_of_inode_table+1;
    gd.free_blocks_count=sb->block_per_group-17;//17개를 쓰고있으니 17개 뺀다
    gd.free_inodes_count=sb->inode_per_group-10;//일단 안정적으로 10개뺌....
    gd.directories_count=1;//루트디렉토리
    
    for (short i = 0; i < 2 ; i++)
    {
        block->group_descriptor[i] = gd;
    }
    return FAT_SUCCESS;
}


int init_block_bitmap(BLOCK_BITMAP* bitmap, int index) {
    
    for (int i = 0; i < index; i++) {
        bitmap->block_bitmap[i] = 0;
    }
    return FAT_SUCCESS;
};

int init_inode_bitmap(INODE_BITMAP* bitmap, int index) {
    
    bitmap->inode_bitmap[0] = 0x01FF; // inode 9번까지는 사용
    for (int i = 1; i < index; i++) {
        bitmap->inode_bitmap[i] = 0;
    }
    return FAT_SUCCESS;
};


/* calculate the 'sectors per cluster' by some conditions */


