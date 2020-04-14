/********************************************************
 *
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:       HASH.H
 *
 * VERSION:     1.0
 *
 * DATE:        APRIL 07 2020
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
#pragma once

DWORD GetStringHash( const PVOID Buffer, DWORD Length );
