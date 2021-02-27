/*++

Copyright (c) 2019  Shorthorn Project

Module Name:

    nls.c

Abstract:

    This file contains functions that enumerate the user's portion of the
    registry for installed and supported locale ids and code page ids.
	
Author:

    Skulltrail 8-May-2017

Revision History:

--*/

#include <main.h>

typedef BOOL    (CALLBACK *CALINFO_ENUMPROCEXEX)(LPWSTR,CALID,LPWSTR,LPARAM);
typedef BOOL    (CALLBACK *DATEFMT_ENUMPROCEXEX)(LPWSTR,CALID,LPARAM);
typedef BOOL    (CALLBACK *TIMEFMT_ENUMPROCEX)(LPWSTR,LPARAM);

enum enum_callback_type {
    CALLBACK_ENUMPROC,
    CALLBACK_ENUMPROCEX,
    CALLBACK_ENUMPROCEXEX
};

struct enumcalendar_context {
    enum enum_callback_type type;  /* callback kind */
    union {
        CALINFO_ENUMPROCW    callback;     /* user callback pointer */
        CALINFO_ENUMPROCEXW  callbackex;
        CALINFO_ENUMPROCEXEX callbackexex;
    } u;
    LCID    lcid;     /* locale of interest */
    CALID   calendar; /* specific calendar or ENUM_ALL_CALENDARS */
    CALTYPE caltype;  /* calendar information type */
    LPARAM  lParam;   /* user input parameter passed to callback, for ExEx case only */
    BOOL    unicode;  /* A vs W callback type, only for regular and Ex callbacks */
};

struct enumdateformats_context {
    enum enum_callback_type type;  /* callback kind */
    union {
        DATEFMT_ENUMPROCW    callback;     /* user callback pointer */
        DATEFMT_ENUMPROCEXW  callbackex;
        DATEFMT_ENUMPROCEXEX callbackexex;
    } u;
    LCID   lcid;    /* locale of interest */
    DWORD  flags;
    LPARAM lParam;
    BOOL   unicode; /* A vs W callback type, only for regular and Ex callbacks */
};

/**************************************************************************
 * NLS_GetLocaleNumber <internal>
 *
 * Get a numeric locale format value.
 */
static DWORD NLS_GetLocaleNumber(LCID lcid, DWORD dwFlags)
{
  WCHAR szBuff[80];
  DWORD dwVal = 0;

  szBuff[0] = '\0';
  GetLocaleInfoW(lcid, dwFlags, szBuff, sizeof(szBuff) / sizeof(WCHAR));

  if (szBuff[0] && szBuff[1] == ';' && szBuff[2] != '0')
    dwVal = (szBuff[0] - '0') * 10 + (szBuff[2] - '0');
  else
  {
    const WCHAR* iter = szBuff;
    dwVal = 0;
    while(*iter >= '0' && *iter <= '9')
      dwVal = dwVal * 10 + (*iter++ - '0');
  }
  return dwVal;
}

/******************************************************************************
 * NLS_EnumCalendarInfo <internal>
 * Enumerates calendar information for a specified locale.
 *
 * PARAMS
 *    ctxt [I] enumeration context, see 'struct enumcalendar_context'
 *
 * RETURNS
 *    Success: TRUE.
 *    Failure: FALSE. Use GetLastError() to determine the cause.
 *
 * NOTES
 *    When the ANSI version of this function is used with a Unicode-only LCID,
 *    the call can succeed because the system uses the system code page.
 *    However, characters that are undefined in the system code page appear
 *    in the string as a question mark (?).
 *
 * TODO
 *    The above note should be respected by GetCalendarInfoA.
 */
static BOOL NLS_EnumCalendarInfo(const struct enumcalendar_context *ctxt)
{
  WCHAR *buf, *opt = NULL, *iter = NULL;
  CALID calendar = ctxt->calendar;
  BOOL ret = FALSE;
  int bufSz = 200;		/* the size of the buffer */

  if (ctxt->u.callback == NULL)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
  }

  buf = HeapAlloc(GetProcessHeap(), 0, bufSz);
  if (buf == NULL)
  {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    return FALSE;
  }

  if (calendar == ENUM_ALL_CALENDARS)
  {
    int optSz = GetLocaleInfoW(ctxt->lcid, LOCALE_IOPTIONALCALENDAR, NULL, 0);
    if (optSz > 1)
    {
      opt = HeapAlloc(GetProcessHeap(), 0, optSz * sizeof(WCHAR));
      if (opt == NULL)
      {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto cleanup;
      }
      if (GetLocaleInfoW(ctxt->lcid, LOCALE_IOPTIONALCALENDAR, opt, optSz))
        iter = opt;
    }
    calendar = NLS_GetLocaleNumber(ctxt->lcid, LOCALE_ICALENDARTYPE);
  }

  while (TRUE)			/* loop through calendars */
  {
    do				/* loop until there's no error */
    {
      if (ctxt->caltype & CAL_RETURN_NUMBER)
        ret = GetCalendarInfoW(ctxt->lcid, calendar, ctxt->caltype, NULL, bufSz / sizeof(WCHAR), (LPDWORD)buf);
      else if (ctxt->unicode)
        ret = GetCalendarInfoW(ctxt->lcid, calendar, ctxt->caltype, buf, bufSz / sizeof(WCHAR), NULL);
      else ret = GetCalendarInfoA(ctxt->lcid, calendar, ctxt->caltype, (CHAR*)buf, bufSz / sizeof(CHAR), NULL);

      if (!ret)
      {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {				/* so resize it */
          int newSz;
          if (ctxt->unicode)
            newSz = GetCalendarInfoW(ctxt->lcid, calendar, ctxt->caltype, NULL, 0, NULL) * sizeof(WCHAR);
          else newSz = GetCalendarInfoA(ctxt->lcid, calendar, ctxt->caltype, NULL, 0, NULL) * sizeof(CHAR);
          if (bufSz >= newSz)
          {
            //ERR("Buffer resizing disorder: was %d, requested %d.\n", bufSz, newSz);
            goto cleanup;
          }
          bufSz = newSz;
          //WARN("Buffer too small; resizing to %d bytes.\n", bufSz);
          buf = HeapReAlloc(GetProcessHeap(), 0, buf, bufSz);
          if (buf == NULL)
            goto cleanup;
        } else goto cleanup;
      }
    } while (!ret);

    /* Here we are. We pass the buffer to the correct version of
     * the callback. Because it's not the same number of params,
     * we must check for Ex, but we don't care about Unicode
     * because the buffer is already in the correct format.
     */
    switch (ctxt->type)
    {
    case CALLBACK_ENUMPROC:
      ret = ctxt->u.callback(buf);
      break;
    case CALLBACK_ENUMPROCEX:
      ret = ctxt->u.callbackex(buf, calendar);
      break;
    case CALLBACK_ENUMPROCEXEX:
      ret = ctxt->u.callbackexex(buf, calendar, NULL, ctxt->lParam);
      break;
    default:
      ;
    }

    if (!ret) {			/* the callback told to stop */
      ret = TRUE;
      break;
    }

    if ((iter == NULL) || (*iter == 0))	/* no more calendars */
      break;

    calendar = 0;
    while ((*iter >= '0') && (*iter <= '9'))
      calendar = calendar * 10 + *iter++ - '0';

    if (*iter++ != 0)
    {
      SetLastError(ERROR_BADDB);
      ret = FALSE;
      break;
    }
  }

cleanup:
  HeapFree(GetProcessHeap(), 0, opt);
  HeapFree(GetProcessHeap(), 0, buf);
  return ret;
}

/******************************************************************************
 * NLS_EnumDateFormats <internal>
 * Enumerates date formats for a specified locale.
 *
 * PARAMS
 *    ctxt [I] enumeration context, see 'struct enumdateformats_context'
 *
 * RETURNS
 *    Success: TRUE.
 *    Failure: FALSE. Use GetLastError() to determine the cause.
 */
static BOOL NLS_EnumDateFormats(const struct enumdateformats_context *ctxt)
{
    WCHAR bufW[256];
    char bufA[256];
    LCTYPE lctype;
    CALID cal_id;
    INT ret;

    if (!ctxt->u.callback)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (!GetLocaleInfoW(ctxt->lcid, LOCALE_ICALENDARTYPE|LOCALE_RETURN_NUMBER, (LPWSTR)&cal_id, sizeof(cal_id)/sizeof(WCHAR)))
        return FALSE;

    switch (ctxt->flags & ~LOCALE_USE_CP_ACP)
    {
    case 0:
    case DATE_SHORTDATE:
        lctype = LOCALE_SSHORTDATE;
        break;
    case DATE_LONGDATE:
        lctype = LOCALE_SLONGDATE;
        break;
    case DATE_YEARMONTH:
        lctype = LOCALE_SYEARMONTH;
        break;
    default:
//        FIXME("Unknown date format (0x%08x)\n", ctxt->flags);
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    lctype |= ctxt->flags & LOCALE_USE_CP_ACP;
    if (ctxt->unicode)
        ret = GetLocaleInfoW(ctxt->lcid, lctype, bufW, sizeof(bufW)/sizeof(bufW[0]));
    else
        ret = GetLocaleInfoA(ctxt->lcid, lctype, bufA, sizeof(bufA)/sizeof(bufA[0]));

    if (ret)
    {
        switch (ctxt->type)
        {
        case CALLBACK_ENUMPROC:
            ctxt->u.callback(ctxt->unicode ? bufW : (WCHAR*)bufA);
            break;
        case CALLBACK_ENUMPROCEX:
            ctxt->u.callbackex(ctxt->unicode ? bufW : (WCHAR*)bufA, cal_id);
            break;
        case CALLBACK_ENUMPROCEXEX:
            ctxt->u.callbackexex(bufW, cal_id, ctxt->lParam);
            break;
        default:
            ;
        }
    }

    return TRUE;
}

struct enumtimeformats_context {
    enum enum_callback_type type;  /* callback kind */
    union {
        TIMEFMT_ENUMPROCW  callback;     /* user callback pointer */
        TIMEFMT_ENUMPROCEX callbackex;
    } u;
    LCID   lcid;    /* locale of interest */
    DWORD  flags;
    LPARAM lParam;
    BOOL   unicode; /* A vs W callback type, only for regular and Ex callbacks */
};

static BOOL NLS_EnumTimeFormats(struct enumtimeformats_context *ctxt)
{
    WCHAR bufW[256];
    char bufA[256];
    LCTYPE lctype;
    INT ret;

    if (!ctxt->u.callback)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    switch (ctxt->flags & ~LOCALE_USE_CP_ACP)
    {
    case 0:
        lctype = LOCALE_STIMEFORMAT;
        break;
    case TIME_NOSECONDS:
        lctype = LOCALE_SSHORTTIME;
        break;
    default:
        //FIXME("Unknown time format (%d)\n", ctxt->flags);
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    lctype |= ctxt->flags & LOCALE_USE_CP_ACP;
    if (ctxt->unicode)
        ret = GetLocaleInfoW(ctxt->lcid, lctype, bufW, sizeof(bufW)/sizeof(bufW[0]));
    else
        ret = GetLocaleInfoA(ctxt->lcid, lctype, bufA, sizeof(bufA)/sizeof(bufA[0]));

    if (ret)
    {
        switch (ctxt->type)
        {
        case CALLBACK_ENUMPROC:
            ctxt->u.callback(ctxt->unicode ? bufW : (WCHAR*)bufA);
            break;
        case CALLBACK_ENUMPROCEX:
            ctxt->u.callbackex(bufW, ctxt->lParam);
            break;
        default:
            ;
        }
    }

    return TRUE;
}

/******************************************************************************
 *		EnumCalendarInfoExEx	[KERNEL32.@]
 */
BOOL 
WINAPI 
EnumCalendarInfoExEx( 
	CALINFO_ENUMPROCEXEX calinfoproc, 
	LPCWSTR locale, 
	CALID calendar,
    LPCWSTR reserved, 
	CALTYPE caltype, 
	LPARAM lParam
)
{
  struct enumcalendar_context ctxt;

  //TRACE("(%p,%s,0x%08x,%p,0x%08x,0x%ld)\n", calinfoproc, debugstr_w(locale), calendar, reserved, caltype, lParam);

  ctxt.type = CALLBACK_ENUMPROCEXEX;
  ctxt.u.callbackexex = calinfoproc;
  ctxt.lcid = LocaleNameToLCID(locale, 0);
  ctxt.calendar = calendar;
  ctxt.caltype = caltype;
  ctxt.lParam = lParam;
  ctxt.unicode = TRUE;
  return NLS_EnumCalendarInfo(&ctxt);
}

/**************************************************************************
 *              EnumDateFormatsExEx	(KERNEL32.@)
 */
BOOL 
WINAPI 
EnumDateFormatsExEx(
	DATEFMT_ENUMPROCEXEX proc, 
	const WCHAR *locale, 
	DWORD flags, 
	LPARAM lParam
)
{
    struct enumdateformats_context ctxt;

    ctxt.type = CALLBACK_ENUMPROCEXEX;
    ctxt.u.callbackexex = proc;
    ctxt.lcid = LocaleNameToLCID(locale, 0);
    ctxt.flags = flags;
    ctxt.lParam = lParam;
    ctxt.unicode = TRUE;

    return NLS_EnumDateFormats(&ctxt);
}

/**************************************************************************
 *              EnumTimeFormatsEx	(KERNEL32.@)
 */
BOOL 
WINAPI 
EnumTimeFormatsEx(
	TIMEFMT_ENUMPROCEX proc, 
	const WCHAR *locale, 
	DWORD flags, 
	LPARAM lParam
)
{
    struct enumtimeformats_context ctxt;

    ctxt.type = CALLBACK_ENUMPROCEX;
    ctxt.u.callbackex = proc;
    ctxt.lcid = LocaleNameToLCID(locale, 0);
    ctxt.flags = flags;
    ctxt.lParam = lParam;
    ctxt.unicode = TRUE;

    return NLS_EnumTimeFormats(&ctxt);
}