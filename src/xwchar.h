/**
 * xwchar.h: Extern wchar.h
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

#include "xstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_INLINE_API size_t vxwcslen(const wchar_t *__restrict fmt, va_list ap) {
	if (!fmt) return 0;

	va_list apc;
	va_copy(apc, ap);

#ifdef _WIN32
	int len = _vscwprintf(fmt, apc);
	va_end(apc);
	return (len < 0) ? (size_t)-1 : (size_t)len;
#else
	int len = vswprintf(NULL, 0, fmt, apc);
	va_end(apc);

	if (len >= 0)
		return (size_t)len;

	size_t bufSize = 256;
	const size_t LIMIT = (SIZE_MAX / sizeof(wchar_t)) / 4;

	while (bufSize < LIMIT) {
		wchar_t*buf = (wchar_t*)malloc(bufSize * sizeof(wchar_t));
		if (!buf) {
			errno = ENOMEM;
			return (size_t)-1;
		}

		va_list ap2;
		va_copy(ap2, ap);
		int needed = vswprintf(buf, bufSize, fmt, ap2);
		va_end(ap2);

		free(buf);

		if (needed >= 0)
			return (size_t)needed;

		if (bufSize > LIMIT / 2) {
			errno = EOVERFLOW;
			return (size_t)-1;
		}

		bufSize *= 2;
	}

	errno = EOVERFLOW;
	return (size_t)-1;
#endif
}

XSTDDEF_INLINE_API size_t xwcslen(const wchar_t *__restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	size_t len = vxwcslen(fmt, ap);
	va_end(ap);
	return len;
}

XSTDDEF_INLINE_API int xwcscmp(const wchar_t * __s1, const wchar_t ** __s2, size_t n) {
	if (!__s2) return 0;  // array pointer itself is NULL

	for (size_t i = 0; i < n; ++i) {
		const wchar_t *s2_str = __s2[i];
		if (__s1 == NULL && s2_str == NULL)
			return 1;
		else if (__s1 == NULL || s2_str == NULL)
			continue;

		if (wcscmp(__s1, s2_str) == 0)
			return 1;
	}

	return 0; // no match
}


XSTDDEF_INLINE_API wchar_t *xwcscmb(const wchar_t *s1, const wchar_t *s2) {
	if (!s1 && !s2) return NULL;
	if (!s1) {
		size_t len = wcslen(s2);
		wchar_t *dup = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
		if (!dup) return NULL;
		wcscpy(dup, s2);
		return dup;
	}
	if (!s2) {
		size_t len = wcslen(s1);
		wchar_t *dup = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
		if (!dup) return NULL;
		wcscpy(dup, s1);
		return dup;
	}
	size_t len1 = wcslen(s1);
	size_t len2 = wcslen(s2);
	wchar_t *result = (wchar_t *)malloc((len1 + len2 + 1) * sizeof(wchar_t));
	if (!result) return NULL;
	memcpy(result, s1, len1 * sizeof(wchar_t));
	memcpy(result + len1, s2, len2 * sizeof(wchar_t));
	result[len1 + len2] = L'\0';
	return result;
}

#ifdef __cplusplus
}
#endif

#endif // __XWCHAR_H__
