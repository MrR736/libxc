/**
 * xwchar.h: Extern Library
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

#if defined(_WIN32) || defined(_WIN64)
	int len = _vscwprintf(fmt, apc);
	va_end(apc);
	return (len < 0) ? 0 : (size_t)len;
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
			return 0;
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
			return 0;
		}

		bufSize *= 2;
	}

	errno = EOVERFLOW;
	return 0;
#endif
}

XSTDDEF_INLINE_API size_t xwcslen(const wchar_t *__restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	size_t len = vxwcslen(fmt, ap);
	va_end(ap);
	return len;
}

#ifdef __cplusplus
}
#endif

#endif // __XWCHAR_H__
