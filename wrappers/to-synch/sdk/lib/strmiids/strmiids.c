 /*

	strmiids.c - DirectShow GUIDs

	Written by Filip Navara <xnavara@volny.cz>

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/
#define INITGUID
#include <basetyps.h>
#include <guiddef.h>
#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"

#include "objbase.h"
#include "oleauto.h"
#include "olectl.h"
#include "d3d9.h"

#include "initguid.h"

#include "uuids.h"
#include "strmif.h"
#include "control.h"
#include "amstream.h"
#include "qedit.h"
#include "vmr9.h"