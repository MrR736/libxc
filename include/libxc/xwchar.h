/**
 * xwchar.h: Extern Library
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

#ifndef __XWCHAR_H__
#define __XWCHAR_H__

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI size_t XCALLAPI vxwcslen(const wchar_t *xrestrict __s, va_list ap);

XSTDAPI size_t XCALLAPI xwcslen(const wchar_t *xrestrict __s, ...);

XSTDAPI int XCALLAPI xwcscmp(const wchar_t *s1, const wchar_t **s2, size_t n);

XSTDAPI int XCALLAPI xwcscasecmp(const wchar_t *s1, const wchar_t *s2);
XSTDAPI int XCALLAPI xwcscasecmp_l(const wchar_t *s1, const wchar_t *s2,xlocale_t l);

XSTDAPI int XCALLAPI xwcsicmp(const wchar_t *s1, const wchar_t *s2);
XSTDAPI int XCALLAPI xwcsicmp_l(const wchar_t *s1, const wchar_t *s2,xlocale_t l);

XSTDAPI wchar_t* XCALLAPI xwcsdup(const wchar_t *s);

XSTDAPI wchar_t* XCALLAPI xwcscmb(const wchar_t *s1, const wchar_t *s2);
XSTDAPI wchar_t* XCALLAPI xwcscmbs(const wchar_t** ss, size_t n);

XSTDAPI const wchar_t* XCALLAPI xwbasename(const wchar_t* path);

XSTDAPI wchar_t** XCALLAPI xwcssdup(const wchar_t** strs, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __XWCHAR_H__
