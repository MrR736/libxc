/**
 * xstat.h: Cross-Platform Directory Operations
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

#ifndef __XSTAT_H__
#define __XSTAT_H__

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDMETHOD(int) xchmod(const char *path, xmode_t m);

XSTDMETHOD(int) xwchmod(const wchar_t *path, xmode_t m);

XSTDMETHOD(int) cmkdir(const char *path, xmode_t m);

XSTDMETHOD(int) wmkdir(const wchar_t *path,xmode_t m);

XSTDMETHOD(int) wchdir(const wchar_t *path);

XSTDMETHOD(int) wrmdir(const wchar_t *path);

XSTDMETHOD(int) vxchdir(const char *xrestrict path, va_list ap);

XSTDMETHOD(int) xchdir(const char *xrestrict path, ...);

XSTDMETHOD(int) vxwchdir(const wchar_t *xrestrict path, va_list ap);

XSTDMETHOD(int) xwchdir(const wchar_t *xrestrict path, ...);

XSTDMETHOD(int) vxrmdir(const char *xrestrict path, va_list ap);

XSTDMETHOD(int) xrmdir(const char *xrestrict path, ...);

XSTDMETHOD(int) vxwrmdir(const wchar_t *xrestrict path, va_list ap);

XSTDMETHOD(int) xwrmdir(const wchar_t *xrestrict path, ...);

#ifdef __cplusplus
}
#endif

#endif // __XSTAT_H__