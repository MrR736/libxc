/**
 * xstring.h: Extern Library
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

#ifndef __XSTRING_H__
#define __XSTRING_H__

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDMETHOD(size_t) vxstrlen(const char *xrestrict __s, va_list ap);

XSTDMETHOD(size_t) xstrlen(const char *xrestrict __s, ...);

XSTDMETHOD(int) xstrcmp(const char *s1, const char **s2, size_t n);

XSTDMETHOD(int) xstrcasecmp(const char *s1, const char *s2);
XSTDMETHOD(int) xstrcasecmp_l(const char *s1, const char *s2,xlocale_t l);

XSTDMETHOD(int) xstricmp(const char *s1, const char *s2);
XSTDMETHOD(int) xstricmp_l(const char *s1, const char *s2,xlocale_t l);

XSTDMETHOD(char*) xstrdup(const char *s);

XSTDMETHOD(char*) xstrcmb(const char * s1, const char * s2);
XSTDMETHOD(char*) xstrcmbs(const char** ss, size_t n);

XSTDMETHOD(const char*) xbasename(const char* path);

XSTDMETHOD(char**) xstrsdup(const char** strs, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __XSTRING_H__
