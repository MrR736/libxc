/**
 * xstdlib.h: Extern Library
 *
 * Copyright (C) 2025 MrR736 <MrR736@users.github.com>
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

#include <xstddef.h>

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_IMPORT_API void* xrand(size_t __size,unsigned int __flags);

XSTDDEF_IMPORT_API char* xwcstombs(const wchar_t *__restrict wcs);

XSTDDEF_IMPORT_API int wsystem(const wchar_t *__restrict __command);

XSTDDEF_IMPORT_API void wperror(const wchar_t *__restrict __s);

XSTDDEF_IMPORT_API wchar_t* xmbstowcs(const char *__restrict mbs);

XSTDDEF_IMPORT_API int vnxsystem(size_t __len, const char *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int nxsystem(size_t __len,const char *__restrict command, ...);

XSTDDEF_IMPORT_API int vxsystem(const char *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int xsystem(const char *__restrict command, ...);

XSTDDEF_IMPORT_API int vnxwsystem(size_t __len, const wchar_t *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int nxwsystem(size_t len,const wchar_t *__restrict command, ...);

XSTDDEF_IMPORT_API int vxwsystem(const wchar_t *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int xwsystem(const wchar_t *__restrict command, ...);

XSTDDEF_IMPORT_API void vnxperror(size_t __len,const char*__restrict __s, va_list ap);

XSTDDEF_IMPORT_API void nxperror(size_t __len,const char*__restrict __s, ...);

XSTDDEF_IMPORT_API void vxperror(const char *__restrict __s, va_list ap);

XSTDDEF_IMPORT_API void xperror(const char *__restrict __s, ...);

XSTDDEF_IMPORT_API void vnxwperror(size_t __len,const wchar_t *__restrict __s, va_list ap);

XSTDDEF_IMPORT_API void nxwperror(size_t __len,const wchar_t *__restrict __s, ...);

XSTDDEF_IMPORT_API void vxwperror(const wchar_t *__restrict __s, va_list ap);

XSTDDEF_IMPORT_API void xwperror(const wchar_t *__restrict __s, ...);

XSTDDEF_IMPORT_API int csystem(const char*__restrict __command);

XSTDDEF_IMPORT_API int wcsystem(const wchar_t *__restrict __command);

XSTDDEF_IMPORT_API int vnxcsystem(size_t __len, const char *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int nxcsystem(size_t __len,const char *__restrict command, ...);

XSTDDEF_IMPORT_API int vxcsystem(const char *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int xcsystem(const char *__restrict command, ...);

XSTDDEF_IMPORT_API int vnxwcsystem(size_t __len, const wchar_t *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int nxwcsystem(size_t len,const wchar_t *__restrict command, ...);

XSTDDEF_IMPORT_API int vxwcsystem(const wchar_t *__restrict command, va_list ap);

XSTDDEF_IMPORT_API int xwcsystem(const wchar_t *__restrict command, ...);

#ifdef __cplusplus
}
#endif

#endif // __XSTDLIB_H__
