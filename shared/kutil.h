/********************************************************
 *
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:       KUTIL.H
 *
 * VERSION:     1.0
 *
 * DATE:        APRIL 06 2020
 *
 * Function for traversing a kernel process control routine (kpcr)
 * to obtain the base address of a ntoskrnl in memory.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
*********************************************************/
#pragma once

typedef struct _SYSTEM_MODULE_ENTRY
{
	HANDLE Section;
	PVOID  MappedBase;
	PVOID  ImageBase;
	ULONG  ImageSize;
	ULONG  Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR FullPathName[256];
} SYSTEM_MODULE_ENTRY, *PSYSTEM_MODULE_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG Count;
	SYSTEM_MODULE_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

LPVOID KeGetKernelBase( VOID );
LPVOID KeGetDriverBase( IN DWORD DriverHash );
