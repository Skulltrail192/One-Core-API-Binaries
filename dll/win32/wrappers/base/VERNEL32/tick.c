/*

Copyright (c) 2017  Shorthorn Project

File name:

    sched.c
   
Author:
    
    adrmarin  Fri Jul 19 17:41:07 2002

*/

#include <main.h>
	
/*
 * @implemented
 */
ULONGLONG
WINAPI
GetTickCount64(VOID)
{
    ULARGE_INTEGER TickCount;
    
    while (TRUE)
    {
        TickCount.HighPart = (ULONG)SharedUserData->TickCount.High1Time;
        TickCount.LowPart = SharedUserData->TickCount.LowPart;

        if (TickCount.HighPart == (ULONG)SharedUserData->TickCount.High2Time) break;

        YieldProcessor();
     }
     
     return (UInt32x32To64(TickCount.LowPart, SharedUserData->TickCountMultiplier) >> 24) +
            (UInt32x32To64(TickCount.HighPart, SharedUserData->TickCountMultiplier) << 8);
}