/**
 * xstdio.h: Extern Library
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

#ifndef __XSTDIO_H__
#define __XSTDIO_H__

#include <stdio.h>
#include <xstddef.h>

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_IMPORT_API int fdputs(const char *__restrict __s, int __fd);

XSTDDEF_IMPORT_API FILE* fdno(int __fd);

XSTDDEF_IMPORT_API FILE* fdno_unlocked(int __fd);

XSTDDEF_IMPORT_API FILE* wfopen(const wchar_t *__restrict __filename, const wchar_t *__restrict __modes);

XSTDDEF_IMPORT_API size_t fpsize(FILE *fp);

XSTDDEF_IMPORT_API void *furead(FILE *fp, size_t *out_size);

XSTDDEF_IMPORT_API size_t fdsize(int fd);

XSTDDEF_IMPORT_API void *fduread(int __fd, size_t *out_size);

XSTDDEF_IMPORT_API char* vcprintf(const char *__restrict fmt, va_list ap);

XSTDDEF_IMPORT_API char* cprintf(const char *__restrict fmt, ...);

XSTDDEF_IMPORT_API wchar_t* vcwprintf(const char *__restrict fmt, va_list ap);

XSTDDEF_IMPORT_API wchar_t* cwprintf(const char *__restrict fmt, ...);

XSTDDEF_IMPORT_API wchar_t* vwcprintf(const wchar_t *__restrict fmt, va_list ap);

XSTDDEF_IMPORT_API wchar_t* wcprintf(const wchar_t *__restrict fmt, ...);

XSTDDEF_IMPORT_API char* vwccprintf(const wchar_t *__restrict fmt, va_list ap);

XSTDDEF_IMPORT_API char* wccprintf(const wchar_t *__restrict fmt, ...);

XSTDDEF_IMPORT_API int wremove(const wchar_t *__restrict fmt);

XSTDDEF_IMPORT_API int vxremove(const char *__restrict fmt, va_list ap);

XSTDDEF_IMPORT_API int xremove(const char *__restrict fmt, ...);

XSTDDEF_IMPORT_API int vxwremove(const wchar_t *__restrict fmt, va_list ap);

XSTDDEF_IMPORT_API int xwremove(const wchar_t *__restrict fmt, ...);

XSTDDEF_IMPORT_API char* getcurrentdirectory_size(size_t);

XSTDDEF_IMPORT_API char* getcurrentdirectory(void);

XSTDDEF_IMPORT_API wchar_t* wgetcurrentdirectory_size(size_t);

XSTDDEF_IMPORT_API wchar_t* wgetcurrentdirectory(void);

#ifdef __cplusplus
}
#endif

#endif // __XSTDIO_H__
