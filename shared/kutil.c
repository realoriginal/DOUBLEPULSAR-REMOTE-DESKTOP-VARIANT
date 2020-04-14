/********************************************************
 * 
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:	KUTIL.C
 *
 * VERSION:	1.0
 *
 * DATE:	APRIL 06 2020
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
#include "common.h"

typedef struct 
{
	NTSTATUS (*ZwQuerySystemInformation)(
			IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
			IN OUT PVOID SystemInformation,
			IN ULONG SystemInformationLength,
			OUT PULONG ReturnLength
		);
	PVOID (*ExAllocatePoolWithTag)(
			IN POOL_TYPE PoolType,
			IN SIZE_T NumberOfBytes,
			IN ULONG Tag
		);
	VOID (*ExFreePoolWithTag)(
			IN PVOID Pointer,
			IN ULONG Tag
		);
} API_TABLE;

LPVOID KeGetKernelBase( VOID )
{
	PKPCR       CurrentPcrAddr  = NULL;
	ULONG_PTR   InterruptorBase = 0;
	ULONG_PTR   NtosKrnlAddress = 0;

	CurrentPcrAddr  = (PKPCR)__readgsqword(FIELD_OFFSET(KPCR, SelfPcr));
	InterruptorBase = ( ULONG_PTR )(( (ULONG64)CurrentPcrAddr->IdtBase->OffsetHigh   << 32 ) +
				        ( (ULONG32)CurrentPcrAddr->IdtBase->OffsetMiddle << 16 ) +
			                ( CurrentPcrAddr ));

	NtosKrnlAddress = ( ULONG_PTR )( InterruptorBase &~ 0xFFF );
	while ( ((PIMAGE_DOS_HEADER)NtosKrnlAddress)->e_magic != IMAGE_DOS_SIGNATURE )
		NtosKrnlAddress = ( ULONG_PTR )( NtosKrnlAddress - 0x1000 );

	return ( LPVOID ) NtosKrnlAddress;
};

LPVOID KeGetDriverBase( IN DWORD DriverHash )
{
	LPVOID                     DriverBaseAddr    = NULL;
	PSYSTEM_MODULE_INFORMATION SystemModuleInfo  = NULL;
	PSYSTEM_MODULE_ENTRY       SystemModuleEntry = NULL;
	LPSTR                      DriverPathName    = NULL;
	ULONG                      InfoSize          = 0;
	ULONG                      LoopIndex         = 0;
	DWORD                      StringHash        = 0;
	ULONG_PTR                  ImageBase         = 0;
	API_TABLE                  FunctionTable     = { 0 };

	FunctionTable.ExFreePoolWithTag        = GetPeExport( KeGetKernelBase(), H_EXFREEPOOLWITHTAG );
	FunctionTable.ExAllocatePoolWithTag    = GetPeExport( KeGetKernelBase(), H_EXALLOCATEPOOLWITHTAG );
	FunctionTable.ZwQuerySystemInformation = GetPeExport( KeGetKernelBase(), H_ZWQUERYSYSTEMINFORMATION );

	FunctionTable.ZwQuerySystemInformation(0xb, &InfoSize, 0, &InfoSize);
	if ( ( SystemModuleInfo = FunctionTable.ExAllocatePoolWithTag(PagedPool, InfoSize, ALLOC_TAG) ) != NULL )
	{
		if ( NT_SUCCESS(FunctionTable.ZwQuerySystemInformation(0xb, SystemModuleInfo, InfoSize, &InfoSize)) )
		{
			SystemModuleEntry = SystemModuleInfo->Module;
			for ( LoopIndex = 0 ; LoopIndex < SystemModuleInfo->Count ; ++LoopIndex )
			{
				DriverPathName = ( PCHAR )( SystemModuleEntry[LoopIndex].FullPathName + SystemModuleEntry[LoopIndex].OffsetToFileName );
				StringHash     = ( DWORD )( GetStringHash( DriverPathName, 0 ) );

				if ( StringHash == DriverHash )
					ImageBase = ( ULONG_PTR )( SystemModuleEntry[LoopIndex].ImageBase );
			};
		};
		FunctionTable.ExFreePoolWithTag(SystemModuleInfo, ALLOC_TAG);
	};
	return ( LPVOID ) ImageBase;
};
