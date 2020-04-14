/********************************************************
 * 
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:	PEUTIL.C
 *
 * VERSION:	1.0
 *
 * DATE:	APRIL 07 2020
 *
 * Functions for parsing loaded portable executables in memory.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED 
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
 * PARTICULAR PURPOSE.
 *
*********************************************************/
#include "common.h"
#define NT_HEADER_P(x) ( ((PIMAGE_DOS_HEADER)x)->e_lfanew + PTR_V(x) )

LPVOID GetPeExport( IN LPVOID DriverBase, IN DWORD ExportHash )
{
	PIMAGE_NT_HEADERS       NtHeader   = NULL;
	PIMAGE_DATA_DIRECTORY   DataDirect = NULL;
	PIMAGE_EXPORT_DIRECTORY ExpDirect  = NULL;

	PDWORD			NameOffset = NULL;
	PDWORD			FuncOffset = NULL;
	PUSHORT			OrdinalOff = NULL;
	PCHAR                   NameString = NULL;

	ULONG			LoopIndex  = 0;
	DWORD                   StringHash = 0;

	NtHeader   = ( PIMAGE_NT_HEADERS ) NT_HEADER_P( DriverBase );
	DataDirect = ( PIMAGE_DATA_DIRECTORY ) &NtHeader->OptionalHeader.DataDirectory[0];
	ExpDirect  = ( PIMAGE_EXPORT_DIRECTORY )( PTR_V(DriverBase) + DataDirect->VirtualAddress );

	NameOffset = ( PDWORD  )( PTR_V( DriverBase ) + ExpDirect->AddressOfNames );
	FuncOffset = ( PDWORD  )( PTR_V( DriverBase ) + ExpDirect->AddressOfFunctions );
	OrdinalOff = ( PUSHORT )( PTR_V( DriverBase ) + ExpDirect->AddressOfNameOrdinals );
	
	for ( LoopIndex = 0 ; LoopIndex < ExpDirect->NumberOfNames ; ++LoopIndex )
	{
		NameString = ( PCHAR )( PTR_V( DriverBase ) + NameOffset[LoopIndex] );
		StringHash = GetStringHash( NameString, 0 );
		if ( ExportHash == StringHash ) 
			return ( LPVOID )( PTR_V( DriverBase ) + FuncOffset[OrdinalOff[LoopIndex]] );
	};
	return NULL;
};

DWORD GetPeSectOffset( IN LPVOID DriverBase, IN DWORD SectionHash, OUT PULONG Length )
{
	PIMAGE_NT_HEADERS     NtHeader   = NULL;
	PIMAGE_SECTION_HEADER SectHeader = NULL;
	ULONG                 LoopIndex  = 0;

	NtHeader = ( PIMAGE_NT_HEADERS ) ( ((PIMAGE_DOS_HEADER)DriverBase)->e_lfanew + PTR_V( DriverBase ) );

	SectHeader = IMAGE_FIRST_SECTION( NtHeader );
	for ( LoopIndex = 0 ; LoopIndex < NtHeader->FileHeader.NumberOfSections ; ++LoopIndex ) {
		if ( GetStringHash( (PCHAR)&SectHeader[LoopIndex].Name, 0 ) == SectionHash ) {
			*Length = SectHeader[LoopIndex].Misc.VirtualSize;
			return SectHeader[LoopIndex].VirtualAddress;
		};
	};
	*Length = 0;
	return 0;
};
