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
#include "xmemory.h"
#include "xtypes.h"
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI size_t XCALLAPI vxwcslen(const wchar_t *xrestrict fmt, va_list ap) {
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
	if (len >= 0) return (size_t)len;
	size_t bufSize = 256;
	const size_t LIMIT = (SIZE_MAX / sizeof(wchar_t)) / 4;
	while (bufSize < LIMIT) {
		wchar_t*buf = (wchar_t*)xmalloc(bufSize * sizeof(wchar_t));
		if (!buf) {
			errno = ENOMEM;
			return (size_t)-1;
		}

		va_list ap2;
		va_copy(ap2, ap);
		int needed = vswprintf(buf, bufSize, fmt, ap2);
		va_end(ap2);

		xfree(buf);

		if (needed >= 0) return (size_t)needed;
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

XSTDAPI size_t XCALLAPI xwcslen(const wchar_t *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	size_t len = vxwcslen(fmt, ap);
	va_end(ap);
	return len;
}

XSTDAPI int XCALLAPI xwcscmp(const wchar_t * s1, const wchar_t ** s2, size_t n) {
	if (!s2) return 0;  // array pointer itself is NULL
	for (size_t i = 0; i < n; ++i) {
		const wchar_t *s2_str = s2[i];
		if (s1 == NULL && s2_str == NULL) return 1;
		else if (s1 == NULL || s2_str == NULL) continue;
		if (wcscmp(s1, s2_str) == 0) return 1;
	}
	return 0; // no match
}

XSTDAPI int XCALLAPI xwcscasecmp(const wchar_t *s1, const wchar_t *s2) {
#ifdef _WIN32
	return _wcsicmp(s1,s2);
#else
	return wcscasecmp(s1,s2);
#endif
}

XSTDAPI int XCALLAPI xwcscasecmp_l(const wchar_t *s1, const wchar_t *s2,xlocale_t l) {
#ifdef _WIN32
	return _wcsicmp_l(s1,s2,l);
#else
	return wcscasecmp_l(s1,s2,l);
#endif
}

XSTDAPI int XCALLAPI xwcsicmp(const wchar_t *s1, const wchar_t *s2) {
	return xwcscasecmp(s1,s2);
}

XSTDAPI int XCALLAPI xwcsicmp_l(const wchar_t *s1, const wchar_t *s2,xlocale_t l) {
	return xwcscasecmp_l(s1,s2,l);
}

XSTDAPI wchar_t* XCALLAPI xwcsdup(const wchar_t *s) {
	if (!s) return NULL;
	size_t len = wcslen(s);
	wchar_t *d = (wchar_t*)xmalloc((len + 1) * sizeof(wchar_t));
	if (!d) return NULL;
	memcpy(d, s, (len + 1) * sizeof(wchar_t));
	return d;
}

XSTDAPI wchar_t* XCALLAPI xwcscmb(const wchar_t *s1, const wchar_t *s2) {
	if (!s1 && !s2) return NULL;
	if (!s1) return xwcsdup(s2);
	if (!s2) return xwcsdup(s1);
	size_t len1 = wcslen(s1);
	size_t len2 = wcslen(s2);
	if (len2 == SIZE_MAX || len1 > SIZE_MAX - len2 - 1) {
		errno = EOVERFLOW;
		return NULL;
	}
	const size_t len = len1 + len2;
	if (len + 1 > SIZE_MAX / sizeof(wchar_t)) {
		errno = EOVERFLOW;
		return NULL;
	}
	wchar_t *result = (wchar_t *)xmalloc((len + 1) * sizeof(wchar_t));
	if (!result) return NULL;
	xmemcpy(result, s1, len1 * sizeof(wchar_t));
	xmemcpy(result + len1, s2, len2 * sizeof(wchar_t));
	result[len] = L'\0';
	return result;
}

XSTDAPI wchar_t* XCALLAPI xwcscmbs(const wchar_t** ss, size_t n) {
	if (ss == NULL || n == 0) return NULL;
	size_t total = 0;
	for (size_t i = 0; i < n; ++i) {
		if (ss[i] != NULL) total += wcslen(ss[i]);
	}
	if (total > (SIZE_MAX / sizeof(wchar_t)) - 1) return NULL;
	wchar_t* result = (wchar_t*)xmalloc((total + 1) * sizeof(wchar_t));
	if (result == NULL) return NULL;
	size_t pos = 0;
	for (size_t i = 0; i < n; ++i) {
		if (ss[i] == NULL) continue;
		size_t len = wcslen(ss[i]);
		if (len != 0) {
			xmemcpy(result + pos, ss[i],len * sizeof(wchar_t));
			pos += len;
		}
	}
	result[pos] = L'\0';
	return result;
}

XSTDAPI const wchar_t* XCALLAPI xwbasename(const wchar_t* path) {
	const wchar_t *p1 = wcsrchr(path,L'/');
	const wchar_t *p2 = wcsrchr(path,L'\\');
	const wchar_t *p = p1 > p2 ? p1 : p2;
	return p ? p + 1 : path;
}

XSTDAPI wchar_t** XCALLAPI xwcssdup(const wchar_t** strs, size_t n) {
	wchar_t **result;
	size_t i;

	if (n > SIZE_MAX / sizeof(*result) - 1) {
		errno = ENOMEM;
		return NULL;
	}

	result = (wchar_t **)xmalloc((n + 1) * sizeof(*result));
	if (result == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	for (i = 0; i <= n; ++i)
		result[i] = NULL;

	if (n != 0 && strs == NULL) {
		xfree(result);
		errno = EINVAL;
		return NULL;
	}

	for (i = 0; i < n; ++i) {
		size_t len;

		if (strs[i] == NULL) {
			xfrees((void **)result, n);
			errno = EINVAL;
			return NULL;
		}

		len = wcslen(strs[i]);
		if (len >= SIZE_MAX / sizeof(wchar_t)) {
			xfrees((void **)result, n);
			errno = ENOMEM;
			return NULL;
		}
		++len;

		result[i] = (wchar_t *)xmalloc(len * sizeof(wchar_t));
		if (result[i] == NULL) {
			xfrees((void **)result, n);
			errno = ENOMEM;
			return NULL;
		}

		xmemcpy(result[i], strs[i], len * sizeof(wchar_t));
	}

	result[n] = NULL;
	return result;
}

#ifdef __cplusplus
}
#endif

#endif // __XWCHAR_H__
