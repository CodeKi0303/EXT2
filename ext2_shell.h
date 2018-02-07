/******************************************************************************/
/*                                                                            */
/* Project : EXT2 File System												  */
/* File    : ext2_shell.h                                                    */
/* Author  : rev (revanZXSD@gmail.com)				                          */
/* Company : NcLab.													          */
/* Notes   : Adaption layer between EXT2 File System and shell                */
/* Date    : 2018/2/7                                                        */
/*                                                                            */
/******************************************************************************/

#ifndef _EXT2_SHELL_H_
#define _EXT2_SHELL_H_

#include "ext2.h"
#include "shell.h"

void shell_register_filesystem(SHELL_FILESYSTEM*);

#endif
