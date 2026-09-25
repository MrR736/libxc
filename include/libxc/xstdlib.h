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

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI xfchmod(int fd, xmode_t mode);

XSTDAPI void* XCALLAPI xrand(size_t __size,unsigned int __flags);

XSTDAPI char* XCALLAPI xwcstombs(const wchar_t *xrestrict wcs);

XSTDAPI int XCALLAPI wsystem(const wchar_t *xrestrict __command);

XSTDAPI void XCALLAPI wperror(const wchar_t *xrestrict __s);

XSTDAPI wchar_t* XCALLAPI xmbstowcs(const char *xrestrict mbs);


XSTDAPI int XCALLAPI vnxsystem(size_t __len, const char *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI nxsystem(size_t __len,const char *xrestrict command, ...);

XSTDAPI int XCALLAPI vxsystem(const char *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI xsystem(const char *xrestrict command, ...);

XSTDAPI int XCALLAPI vnxwsystem(size_t __len, const wchar_t *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI nxwsystem(size_t len,const wchar_t *xrestrict command, ...);

XSTDAPI int XCALLAPI vxwsystem(const wchar_t *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI xwsystem(const wchar_t *xrestrict command, ...);


XSTDAPI void XCALLAPI vnxperror(size_t __len,const char*xrestrict __s, va_list ap);

XSTDAPI void XCALLAPI nxperror(size_t __len,const char*xrestrict __s, ...);

XSTDAPI void XCALLAPI vxperror(const char *xrestrict __s, va_list ap);

XSTDAPI void XCALLAPI xperror(const char *xrestrict __s, ...);

XSTDAPI void XCALLAPI vnxwperror(size_t __len,const wchar_t *xrestrict __s, va_list ap);

XSTDAPI void XCALLAPI nxwperror(size_t __len,const wchar_t *xrestrict __s, ...);

XSTDAPI void XCALLAPI vxwperror(const wchar_t *xrestrict __s, va_list ap);

XSTDAPI void XCALLAPI xwperror(const wchar_t *xrestrict __s, ...);


XSTDAPI int XCALLAPI csystem(const char*xrestrict __command);

XSTDAPI int XCALLAPI wcsystem(const wchar_t *xrestrict __command);

XSTDAPI int XCALLAPI vnxcsystem(size_t __len, const char *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI nxcsystem(size_t __len,const char *xrestrict command, ...);

XSTDAPI int XCALLAPI vxcsystem(const char *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI xcsystem(const char *xrestrict command, ...);

XSTDAPI int XCALLAPI vnxwcsystem(size_t __len, const wchar_t *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI nxwcsystem(size_t len,const wchar_t *xrestrict command, ...);

XSTDAPI int XCALLAPI vxwcsystem(const wchar_t *xrestrict command, va_list ap);

XSTDAPI int XCALLAPI xwcsystem(const wchar_t *xrestrict command, ...);

#ifdef __cplusplus
}
#endif

#endif // __XSTDLIB_H__
