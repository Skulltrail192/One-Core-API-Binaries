/*

Copyright (c) 2017  Shorthorn Project

File name:

    tick.c
   
Author:
    
   Skulltrail 21 - July - 2017

*/

#include <main.h>
	
/*
 * @implemented
 */
ULONGLONG
WINAPI
GetTickCount64(VOID)
{
    LARGE_INTEGER counter, frequency;
	
    NtQueryPerformanceCounter( &counter, &frequency );
    return counter.QuadPart * 1000 / frequency.QuadPart;
}