/**
 * xstring.h: Extern string.h
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

#ifndef __XSTRING_H__
#define __XSTRING_H__

#include "xstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_INLINE_API size_t vxstrlen(const char *__restrict fmt, va_list ap) {
	if (fmt == NULL) return 0;
	va_list apc;
	int len = 0;
	va_copy(apc, ap);
#if defined(_WIN32) || defined(_WIN64)
	len = _vscprintf(fmt, apc);
#else
	len = vsnprintf(NULL, 0, fmt, apc);
#endif
	va_end(apc);

	return len < 0 ? (size_t)-1 : (size_t)len;
}

XSTDDEF_INLINE_API size_t xstrlen(const char *__restrict __s, ...) {
	va_list ap;
	va_start(ap,__s);
	size_t len = vxstrlen(__s, ap);
	va_end(ap);
	return len;
}

XSTDDEF_INLINE_API int xstrcmp(const char * __s1, const char ** __s2, size_t n) {
    if (!__s2) return 0;  // array pointer itself is NULL

    for (size_t i = 0; i < n; ++i) {
	const char *s2_str = __s2[i];
	if (__s1 == NULL && s2_str == NULL)
		return 1;
	else if (__s1 == NULL || s2_str == NULL)
		continue;

	if (strcmp(__s1, s2_str) == 0)
		return 1;
    }

    return 0; // no match
}

#ifdef __cplusplus
}
#endif

#endif // __XSTRING_H__
