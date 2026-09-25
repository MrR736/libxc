/**
 * xstdio.h: Extern Library
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

#ifndef __XSTDIO_H__
#define __XSTDIO_H__

#include <stdio.h>
#include "xstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI fdputs(const char *xrestrict __s, int __fd);

XSTDAPI FILE* XCALLAPI fopenm(const char *xrestrict filename, int oflag, ...);

XSTDAPI FILE* XCALLAPI fdopenm(int fd, int oflag, ...);

XSTDAPI FILE* XCALLAPI fdno(int __fd);

XSTDAPI void XCALLAPI xflockfile(FILE *fp);

XSTDAPI FILE* XCALLAPI fdno_unlocked(int __fd);

XSTDAPI FILE* XCALLAPI wfopen(const wchar_t *xrestrict __filename, const wchar_t *xrestrict __modes);

XSTDAPI FILE* XCALLAPI wfopenm(const wchar_t *xrestrict filename, int oflag, ...);

XSTDAPI size_t XCALLAPI fpsize(FILE *fp);

XSTDAPI void* XCALLAPI furead(FILE *fp, size_t *out_size);

XSTDAPI size_t XCALLAPI fdsize(int fd);

XSTDAPI void* XCALLAPI fduread(int __fd, size_t *out_size);

XSTDAPI char* XCALLAPI vcprintf(const char *xrestrict fmt, va_list ap);

XSTDAPI char* XCALLAPI cprintf(const char *xrestrict fmt, ...);

XSTDAPI wchar_t* XCALLAPI vcwprintf(const char *xrestrict fmt, va_list ap);

XSTDAPI wchar_t* XCALLAPI cwprintf(const char *xrestrict fmt, ...);

XSTDAPI wchar_t* XCALLAPI vwcprintf(const wchar_t *xrestrict fmt, va_list ap);

XSTDAPI wchar_t* XCALLAPI wcprintf(const wchar_t *xrestrict fmt, ...);

XSTDAPI char* XCALLAPI vwccprintf(const wchar_t *xrestrict fmt, va_list ap);

XSTDAPI char* XCALLAPI wccprintf(const wchar_t *xrestrict fmt, ...);

XSTDAPI int XCALLAPI wremove(const wchar_t *xrestrict fmt);

XSTDAPI int XCALLAPI vxremove(const char *xrestrict fmt, va_list ap);

XSTDAPI int XCALLAPI xremove(const char *xrestrict fmt, ...);

XSTDAPI int XCALLAPI vxwremove(const wchar_t *xrestrict fmt, va_list ap);

XSTDAPI int XCALLAPI xwremove(const wchar_t *xrestrict fmt, ...);

XSTDAPI char* XCALLAPI getcurrentdirectory_size(size_t);

XSTDAPI char* XCALLAPI getcurrentdirectory(void);

XSTDAPI wchar_t* XCALLAPI wgetcurrentdirectory_size(size_t);

XSTDAPI wchar_t* XCALLAPI wgetcurrentdirectory(void);

#ifdef __cplusplus
}
#endif

#endif // __XSTDIO_H__
