/******************************************************************************/
/*                                                                            */
/* Project : EXT2 File System												  */
/* File    : ext2_shell.c                                                     */
/* Author  : rev (revanZXSD@gmail.com)				                          */
/* Company : NcLab.													          */
/* Notes   : Adaption layer between EXT2 File System and shell                */
/* Date    : 2018/2/7                                                          */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "ext2_shell.h"

#define FSOPRS_TO_EXT2FS( a )		( EXT2_FILESYSTEM* )a->pdata

typedef struct
{
	BYTE	driveNumber;
	BYTE	reserved1;
	BYTE	bootSignature;
	DWORD	volumeID;
	BYTE	volumeLabel[11];
	BYTE	filesystemType[8];
} FAT_BOOTSECTOR;

typedef struct
{
	union
	{
		WORD	halfCluster[2];
		DWORD	fullCluster;
	};
	BYTE	attribute;
} PRIVATE_EXT2_ENTRY;

int fill_super_block(EXT2_SUPER_BLOCK* super_block, short groupnum, SECTOR numberOfSectors, UINT32 bytesPerSector)
{

	return FAT_SUCCESS;
}

int fs2_mount(DISK_OPERATIONS* disk, SHELL_FS_OPERATIONS* fsOprs, SHELL_ENTRY* root)
{
	
	return FAT_SUCCESS;
}

void fs2_umount(DISK_OPERATIONS* disk, SHELL_FS_OPERATIONS* fsOprs)
{

}

int fs2_format(DISK_OPERATIONS* disk, void* param)
{
	unsigned char EXTType = 0;
	
	printf("formatting as a EXT2");
	return EXT2_format(disk);
}

static SHELL_FILESYSTEM g_ext2 =
{
	"EXT2",
	fs2_mount,
	fs2_umount,
	fs2_format
};

void shell_register_filesystem(SHELL_FILESYSTEM* fs)
{
	*fs = g_ext2;
}
