/********************************************************
 * 
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:	ENTRY.C
 *
 * VERSION:	1.0
 *
 * DATE:	APRIL 07 2020
 *
 * Sample entrypoint for the kernel shellcode, with
 * c_EntryPoint marked as the programs "entry".
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED 
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
 * PARTICULAR PURPOSE.
 *
*********************************************************/
#define _WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "common.h"

#define H_NTOSKRNL 0x815d3210
#define H_RDPWD    0x8b39664d

BOOL c_EntryPoint( VOID )
{
	ULONG_PTR pNtosKrnl = ( ULONG_PTR ) KeGetDriverBase( H_NTOSKRNL );
	ULONG_PTR pRdpwdSys = ( ULONG_PTR ) KeGetDriverBase( H_RDPWD );
	return TRUE;
};
