/**
 * xstring.h: Extern string.h
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
#include "xmemory.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI size_t XCALLAPI vxstrlen(const char *xrestrict fmt, va_list ap) {
	if (fmt == NULL) return 0;
	va_list apc;
	int len = 0;
	va_copy(apc, ap);
#ifdef _WIN32
	len = _vscprintf(fmt, apc);
#else
	len = vsnprintf(NULL, 0, fmt, apc);
#endif
	va_end(apc);
	return len < 0 ? (size_t)-1 : (size_t)len;
}

XSTDAPI size_t XCALLAPI xstrlen(const char *xrestrict s, ...) {
	va_list ap;
	va_start(ap,s);
	size_t len = vxstrlen(s, ap);
	va_end(ap);
	return len;
}

XSTDAPI int XCALLAPI xstrcmp(const char * s1, const char ** s2, size_t n) {
	if (!s2) return 0;  // array pointer itself is NULL

	for (size_t i = 0; i < n; ++i) {
		const char *s2_str = s2[i];
		if (s1 == NULL && s2_str == NULL) return 1;
		else if (s1 == NULL || s2_str == NULL) continue;
		if (strcmp(s1, s2_str) == 0) return 1;
	}

	return 0; // no match
}

XSTDAPI int XCALLAPI xstrcasecmp(const char *s1, const char *s2) {
#ifdef _WIN32
	return _stricmp(s1, s2);
#else
	return strcasecmp(s1, s2);
#endif
}

XSTDAPI int XCALLAPI xstrcasecmp_l(const char *s1, const char *s2,xlocale_t l) {
#ifdef _WIN32
	return _stricmp_l(s1, s2,l);
#else
	return strcasecmp_l(s1, s2,l);
#endif
}

XSTDAPI int XCALLAPI xstricmp(const char *s1, const char *s2) {
	return xstrcasecmp(s1, s2);
}

XSTDAPI int XCALLAPI xstricmp_l(const char *s1, const char *s2,xlocale_t l) {
	return xstrcasecmp_l(s1, s2,l);
}

XSTDAPI char* XCALLAPI xstrdup(const char *s) {
	if (!s) return NULL;
	size_t len = strlen(s);
	char *d = (char*)xmalloc(len + 1);
	if (!d) return NULL;
	memcpy(d, s, len + 1);
	return d;
}

XSTDAPI char* XCALLAPI xstrcmb(const char * s1, const char * s2) {
	if (!s1 && !s2) return NULL;
	if (!s1) return xstrdup(s2);
	if (!s2) return xstrdup(s1);
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	if (len2 == SIZE_MAX || len1 > SIZE_MAX - len2 - 1) {
		errno = EOVERFLOW;
		return NULL;
	}
	const size_t len = len1 + len2;
	if (len + 1 > SIZE_MAX / sizeof(wchar_t)) {
		errno = EOVERFLOW;
		return NULL;
	}
	char *result = (char *)xmalloc(len + 1);
	if (!result) return NULL;
	xmemcpy(result, s1, len1);
	xmemcpy(result + len1, s2, len2);
	result[len] = '\0';
	return result;
}

XSTDAPI char* XCALLAPI xstrcmbs(const char** ss, size_t n) {
	if (ss == NULL || n == 0) return NULL;
	size_t total = 0;

	/* Calculate the required buffer size. */
	for (size_t i = 0; i < n; ++i) {
		if (ss[i] != NULL) total += strlen(ss[i]);
	}

	if (total == 0) return NULL;
	char* result = (char*)xmalloc(total + 1);
	if (result == NULL) return NULL;
	size_t pos = 0;
	for (size_t i = 0; i < n; ++i) {
		if (ss[i] == NULL) continue;
		size_t len = strlen(ss[i]);
		if (len != 0) {
			xmemcpy(result + pos, ss[i], len);
			pos += len;
		}
	}
	result[pos] = '\0';
	return result;
}

XSTDAPI const char* XCALLAPI xbasename(const char* path) {
	const char *p1 = strrchr(path, '/');
	const char *p2 = strrchr(path, '\\');
	const char *p = p1 > p2 ? p1 : p2;
	return p ? p + 1 : path;
}

XSTDAPI char** XCALLAPI xstrsdup(const char** strs, size_t n) {
	char **result;
	size_t i;

	if (n > SIZE_MAX / sizeof(*result) - 1) {
		errno = ENOMEM;
		return NULL;
	}

	result = (char **)xmalloc((n + 1) * sizeof(*result));
	if (result == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	for (i = 0; i <= n; ++i) result[i] = NULL;

	if (n != 0 && strs == NULL) {
		xfree(result);
		errno = EINVAL;
		return NULL;
	}

	for (i = 0; i < n; ++i) {
		size_t len;

		if (strs[i] == NULL) {
			xfrees((void**)result, n);
			errno = EINVAL;
			return NULL;
		}

		len = strlen(strs[i]) + 1;
		result[i] = (char *)xmalloc(len);
		if (result[i] == NULL) {
			xfrees((void **)result, n);
			errno = ENOMEM;
			return NULL;
		}

		xmemcpy(result[i], strs[i], len);
	}

	result[n] = NULL;
	return result;
}

#ifdef __cplusplus
}
#endif

#endif // __XSTRING_H__
