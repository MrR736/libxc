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

XSTDMETHOD(int) fdputs(const char *xrestrict __s, int __fd);

XSTDMETHOD(FILE*) fopenm(const char *xrestrict filename, int oflag, ...);

XSTDMETHOD(FILE*) fdopenm(int fd, int oflag, ...);

XSTDMETHOD(FILE*) fdno(int __fd);

XSTDMETHOD(void) xflockfile(FILE *fp);

XSTDMETHOD(FILE*) fdno_unlocked(int __fd);

XSTDMETHOD(FILE*) wfopen(const wchar_t *xrestrict __filename, const wchar_t *xrestrict __modes);

XSTDMETHOD(FILE*) wfopenm(const wchar_t *xrestrict filename, int oflag, ...);

XSTDMETHOD(size_t) fpsize(FILE *fp);

XSTDMETHOD(void*) furead(FILE *fp, size_t *out_size);

XSTDMETHOD(size_t) fdsize(int fd);

XSTDMETHOD(void*) fduread(int __fd, size_t *out_size);

XSTDMETHOD(char*) vcprintf(const char *xrestrict fmt, va_list ap);

XSTDMETHOD(char*) cprintf(const char *xrestrict fmt, ...);

XSTDMETHOD(wchar_t*) vcwprintf(const char *xrestrict fmt, va_list ap);

XSTDMETHOD(wchar_t*) cwprintf(const char *xrestrict fmt, ...);

XSTDMETHOD(wchar_t*) vwcprintf(const wchar_t *xrestrict fmt, va_list ap);

XSTDMETHOD(wchar_t*) wcprintf(const wchar_t *xrestrict fmt, ...);

XSTDMETHOD(char*) vwccprintf(const wchar_t *xrestrict fmt, va_list ap);

XSTDMETHOD(char*) wccprintf(const wchar_t *xrestrict fmt, ...);

XSTDMETHOD(int) wremove(const wchar_t *xrestrict fmt);

XSTDMETHOD(int) vxremove(const char *xrestrict fmt, va_list ap);

XSTDMETHOD(int) xremove(const char *xrestrict fmt, ...);

XSTDMETHOD(int) vxwremove(const wchar_t *xrestrict fmt, va_list ap);

XSTDMETHOD(int) xwremove(const wchar_t *xrestrict fmt, ...);

XSTDMETHOD(char*) getcurrentdirectory_size(size_t);

XSTDMETHOD(char*) getcurrentdirectory(void);

XSTDMETHOD(wchar_t*) wgetcurrentdirectory_size(size_t);

XSTDMETHOD(wchar_t*) wgetcurrentdirectory(void);

XSTDMETHOD(bool) fexists(const char *xrestrict path);

XSTDMETHOD(bool) wfexists(const wchar_t *xrestrict path);

XSTDMETHOD(bool) dexists(const char *xrestrict path);

XSTDMETHOD(bool) wdexists(const wchar_t *xrestrict path);

#ifdef __cplusplus
}
#endif

#endif // __XSTDIO_H__
