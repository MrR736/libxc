/**
 * xdlfcn.h: Cross-Platform Dynamic Library Utilities
 *
 * Copyright (C) 2026 MrR736 <MrR736@users.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The complete text of the GNU General Public License
 * can be found in /usr/share/common-licenses/GPL-3 file.
 */

#ifndef __XDLFCN_H__
#define __XDLFCN_H__

#include "xtypes.h"
#include "xstddef.h"

#ifdef _WIN32
/* dlopen() flags */
#define XRTLD_LAZY			0x00001
#define XRTLD_NOW			0x00002
#define XRTLD_BINDING_MASK	0x00003
#define XRTLD_NOLOAD		0x00004
#define XRTLD_DEEPBIND		0x00008
#define XRTLD_NODELETE		0x00010
#define XRTLD_LOCAL			0x00000
#define XRTLD_GLOBAL		0x00100
#define XRTLD_FIRST			0x00000
#else
#include <dlfcn.h>

/* dlopen() flags */
#define XRTLD_LAZY			RTLD_LAZY
#define XRTLD_NOW			RTLD_NOW
#define XRTLD_BINDING_MASK	RTLD_BINDING_MASK
#define XRTLD_LOCAL			RTLD_LOCAL
#define XRTLD_GLOBAL		RTLD_GLOBAL

#ifdef RTLD_NOLOAD
# define XRTLD_NOLOAD		RTLD_NOLOAD
#else
# define XRTLD_NOLOAD		0x00004
#endif

#ifdef RTLD_DEEPBIND
# define XRTLD_DEEPBIND		RTLD_DEEPBIND
#else
# define XRTLD_DEEPBIND		0x00008
#endif

#ifdef RTLD_NODELETE
# define XRTLD_NODELETE		RTLD_NODELETE
#else
# define XRTLD_NODELETE		0x00010
#endif
#endif

/* xdlsym() lookup controls */
#ifdef RTLD_NEXT
# define XRTLD_NEXT ((HMODULE)RTLD_NEXT)
#else
# define XRTLD_NEXT ((HMODULE)-1L)
#endif

#ifdef RTLD_DEFAULT
# define XRTLD_DEFAULT ((HMODULE)RTLD_DEFAULT)
#else
# ifdef __APPLE__
#  define XRTLD_DEFAULT ((HMODULE)-2L)
# else
#  define XRTLD_DEFAULT ((HMODULE)0)
# endif
#endif

#ifdef __APPLE__
# ifdef RTLD_FIRST
#  define XRTLD_FIRST RTLD_FIRST
# else
#  define XRTLD_FIRST ((void*)0)
# endif

# ifdef RTLD_SELF
#  define XRTLD_SELF RTLD_SELF
# else
#  define XRTLD_SELF ((void*)-3L)
# endif

# ifdef RTLD_MAIN_ONLY
#  define XRTLD_MAIN_ONLY RTLD_MAIN_ONLY
# else
#  define XRTLD_MAIN_ONLY ((void*)-5L)
# endif
#endif

#define XDLFCN_ERROR_BUFFER 512
#define XDLFCN_PATH_BUFFER  XPATH_MAX
#define XDLFCN_MAX_SYM_NAME 2000

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI char *XCALLAPI xdllibpath(const char *dir, const char *name);

XSTDAPI int XCALLAPI xdladdr(const void *addr, XDl_info *info);

XSTDAPI HMODULE XCALLAPI xdlopen(const char * file, int mode);

XSTDAPI FARPROC XCALLAPI xdlsym(HMODULE handle,const char *name);

XSTDAPI char* XCALLAPI xdlerror(void);


XSTDAPI HMODULE XCALLAPI xwdlopen(const wchar_t* file, int mode);

XSTDAPI FARPROC XCALLAPI xwdlsym(HMODULE handle,const wchar_t* name);

XSTDAPI wchar_t* XCALLAPI xwdlerror(void);


XSTDAPI int XCALLAPI xdlclose(HMODULE handle);


XSTDAPI FARPROC XCALLAPI GetProcAddressA (HMODULE hModule, LPCSTR lpProcName);

XSTDAPI FARPROC XCALLAPI GetProcAddressW (HMODULE hModule, LPCWSTR lpProcName);

#ifndef _WIN32
XSTDAPI HMODULE XCALLAPI GetModuleHandleA(const char* lpModuleName);

XSTDAPI HMODULE XCALLAPI GetModuleHandleW(const wchar_t* lpModuleName);

XSTDAPI HMODULE XCALLAPI LoadLibraryA(const char* lpLibFileName);

XSTDAPI HMODULE XCALLAPI LoadLibraryW(const wchar_t* lpLibFileName);

XSTDAPI FARPROC XCALLAPI GetProcAddress (HMODULE hModule, LPCSTR lpProcName);
#endif

#ifdef __cplusplus
}
#endif

#endif