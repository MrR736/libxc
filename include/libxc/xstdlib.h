/**
 * xstdlib.h: Extern Library
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

#ifndef __XSTDLIB_H__
#define __XSTDLIB_H__

#include "xtypes.h"
#include "xstddef.h"

#ifndef XRANDOM_FLAG
#define XRANDOM_FLAG 256
#endif

#define xrandom(size) xrand(size,XRANDOM_FLAG)

#ifdef __cplusplus
extern "C" {
#endif

XSTDMETHOD(void*) xrand(size_t size,unsigned int flags);

XSTDMETHOD(char*) xwcstombs_len(const wchar_t *xrestrict wcs,size_t* out_size);

XSTDMETHOD(char*) xwcstombs(const wchar_t *xrestrict wcs);

XSTDMETHOD(char**) xwcsstombss(const wchar_t **xrestrict wcs,size_t count);

XSTDMETHOD(wchar_t*) xmbstowcs_len(const char *xrestrict mbs, size_t *out_size);

XSTDMETHOD(wchar_t*) xmbstowcs(const char *xrestrict mbs);

XSTDMETHOD(wchar_t**) xmbsstowcss(const char **xrestrict mbs,size_t count);

XSTDMETHOD(int) wsystem(const wchar_t *xrestrict __command);

XSTDMETHOD(void) wperror(const wchar_t *xrestrict __s);


XSTDMETHOD(int) vnxsystem(size_t __len, const char *xrestrict command, va_list ap);

XSTDMETHOD(int) nxsystem(size_t __len,const char *xrestrict command, ...);

XSTDMETHOD(int) vxsystem(const char *xrestrict command, va_list ap);

XSTDMETHOD(int) xsystem(const char *xrestrict command, ...);

XSTDMETHOD(int) vnxwsystem(size_t __len, const wchar_t *xrestrict command, va_list ap);

XSTDMETHOD(int) nxwsystem(size_t len,const wchar_t *xrestrict command, ...);

XSTDMETHOD(int) vxwsystem(const wchar_t *xrestrict command, va_list ap);

XSTDMETHOD(int) xwsystem(const wchar_t *xrestrict command, ...);


XSTDMETHOD(void) vnxperror(size_t __len,const char*xrestrict __s, va_list ap);

XSTDMETHOD(void) nxperror(size_t __len,const char*xrestrict __s, ...);

XSTDMETHOD(void) vxperror(const char *xrestrict __s, va_list ap);

XSTDMETHOD(void) xperror(const char *xrestrict __s, ...);

XSTDMETHOD(void) vnxwperror(size_t __len,const wchar_t *xrestrict __s, va_list ap);

XSTDMETHOD(void) nxwperror(size_t __len,const wchar_t *xrestrict __s, ...);

XSTDMETHOD(void) vxwperror(const wchar_t *xrestrict __s, va_list ap);

XSTDMETHOD(void) xwperror(const wchar_t *xrestrict __s, ...);


XSTDMETHOD(int) csystem(const char*xrestrict __command);

XSTDMETHOD(int) wcsystem(const wchar_t *xrestrict __command);

XSTDMETHOD(int) vnxcsystem(size_t __len, const char *xrestrict command, va_list ap);

XSTDMETHOD(int) nxcsystem(size_t __len,const char *xrestrict command, ...);

XSTDMETHOD(int) vxcsystem(const char *xrestrict command, va_list ap);

XSTDMETHOD(int) xcsystem(const char *xrestrict command, ...);

XSTDMETHOD(int) vnxwcsystem(size_t __len, const wchar_t *xrestrict command, va_list ap);

XSTDMETHOD(int) nxwcsystem(size_t len,const wchar_t *xrestrict command, ...);

XSTDMETHOD(int) vxwcsystem(const wchar_t *xrestrict command, va_list ap);

XSTDMETHOD(int) xwcsystem(const wchar_t *xrestrict command, ...);

XSTDMETHOD(int) xwcsstrcmp(const wchar_t* s1, const char* s2);

XSTDMETHOD(int) xstrwcscmp(const char* s1, const wchar_t* s2);

#ifdef __cplusplus
}
#endif

#endif // __XSTDLIB_H__
