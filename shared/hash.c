/********************************************************
 * 
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:	HASH.C
 *
 * VERSION:	1.0
 *
 * DATE:	APRIL 07 2020
 *
 * Computes a hash using the fnva1 algorithm to avoid
 * potential collisions.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED 
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
 * PARTICULAR PURPOSE.
 *
*********************************************************/
#include "common.h"

DWORD GetStringHash( const PVOID Buffer, DWORD Length )
{
	const unsigned char * Input  = ( const unsigned char * ) Buffer;
	DWORD                 sHash  = 2166136261;

	while ( TRUE )
	{
		CHAR Current = *Input;
		if ( Length == 0 )
		{
			if ( * Input == 0 )
				break;
		} else {
			if ( ( DWORD )( Input - ( const unsigned char * )( Buffer ) ) >= Length )
				break;
			if ( * Input == 0 ) {
				++Input;
				break;
			};
		};
		if ( Current >= 'a' )
			Current -= 0x20 ;

		sHash ^= Current;
		sHash *= 16777619;

		++Input;
	};
	return sHash;
};
