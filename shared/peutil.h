/********************************************************
 *
 * (C) COPYRIGHT AUTHORS, 2020
 *
 * TITLE:       PEUTIL.H
 *
 * VERSION:     1.0
 *
 * DATE:        APRIL 07 2020
 *
 * Functions for parsing loaded portable executables in memory.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
*********************************************************/
#pragma once 

LPVOID GetPeExport( IN LPVOID DriverBase, IN DWORD ExportHash );
