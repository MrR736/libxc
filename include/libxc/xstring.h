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

XSTDAPI size_t XCALLAPI vxstrlen(const char *xrestrict __s, va_list ap);

XSTDAPI size_t XCALLAPI xstrlen(const char *xrestrict __s, ...);

XSTDAPI int XCALLAPI xstrcmp(const char *s1, const char **s2, size_t n);

XSTDAPI int XCALLAPI xstrcasecmp(const char *s1, const char *s2);
XSTDAPI int XCALLAPI xstrcasecmp_l(const char *s1, const char *s2,xlocale_t l);

XSTDAPI int XCALLAPI xstricmp(const char *s1, const char *s2);
XSTDAPI int XCALLAPI xstricmp_l(const char *s1, const char *s2,xlocale_t l);

XSTDAPI char* XCALLAPI xstrdup(const char *s);

XSTDAPI char* XCALLAPI xstrcmb(const char * s1, const char * s2);
XSTDAPI char* XCALLAPI xstrcmbs(const char** ss, size_t n);

XSTDAPI const char* XCALLAPI xbasename(const char* path);

XSTDAPI char** XCALLAPI xstrsdup(const char** strs, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __XSTRING_H__
