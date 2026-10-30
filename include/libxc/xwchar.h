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

XSTDMETHOD(size_t) vxwcslen(const wchar_t *xrestrict __s, va_list ap);

XSTDMETHOD(size_t) xwcslen(const wchar_t *xrestrict __s, ...);

XSTDMETHOD(int) xwcscmp(const wchar_t *s1, const wchar_t **s2, size_t n);

XSTDMETHOD(int) xwcscasecmp(const wchar_t *s1, const wchar_t *s2);
XSTDMETHOD(int) xwcscasecmp_l(const wchar_t *s1, const wchar_t *s2,xlocale_t l);

XSTDMETHOD(int) xwcsicmp(const wchar_t *s1, const wchar_t *s2);
XSTDMETHOD(int) xwcsicmp_l(const wchar_t *s1, const wchar_t *s2,xlocale_t l);

XSTDMETHOD(wchar_t*) xwcsdup(const wchar_t *s);

XSTDMETHOD(wchar_t*) xwcscmb(const wchar_t *s1, const wchar_t *s2);
XSTDMETHOD(wchar_t*) xwcscmbs(const wchar_t** ss, size_t n);

XSTDMETHOD(const wchar_t*) xwbasename(const wchar_t* path);

XSTDMETHOD(wchar_t**) xwcssdup(const wchar_t** strs, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __XWCHAR_H__
