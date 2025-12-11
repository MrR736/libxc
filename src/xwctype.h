/**
 * xwctype.h: Extern wctype.h
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

#ifndef __XWCTYPE_H__
#define __XWCTYPE_H__

#include "xstddef.h"
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_INLINE_API wchar_t* xtowlower(wchar_t* str) {
	if (str == NULL) {
		errno = EINVAL;
		return NULL;
	}

	size_t len = wcslen(str);
	wchar_t* ret = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}

	for (size_t i = 0; i < len; i++)
		ret[i] = (wchar_t)towlower((wint_t)str[i]);

	ret[len] = L'\0';
	return ret;
}

XSTDDEF_INLINE_API wchar_t* xtowupper(wchar_t* str) {
	if (str == NULL) {
		errno = EINVAL;
		return NULL;
	}

	size_t len = wcslen(str);
	wchar_t* ret = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}

	for (size_t i = 0; i < len; i++)
		ret[i] = (wchar_t)towupper((wint_t)str[i]);

	ret[len] = L'\0';
	return ret;
}

XSTDDEF_INLINE_API wchar_t* xtowascii(wchar_t* str) {
	if (str == NULL) {
		errno = EINVAL;
		return NULL;
	}

	size_t len = wcslen(str);
	wchar_t* ret = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}

	size_t j = 0;
	for (size_t i = 0; i < len; i++) {
		if (str[i] >= 0 && str[i] <= UCHAR_MAX)
			ret[j++] = (wchar_t)str[i];
		else
			ret[j++] = L'?';
	}

	ret[j] = '\0';
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif // __XWCTYPE_H__
