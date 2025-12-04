/**
 * xstring.h: Extern Library
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

#ifndef __XLOCALE_H__
#define __XLOCALE_H__

#include "xstdlib.h"
#include <locale.h>

#define	XLC_ALL			LC_ALL
#define XLC_COLLATE		LC_COLLATE
#define XLC_CTYPE		LC_CTYPE
#define XLC_MONETARY		LC_MONETARY
#define XLC_NUMERIC		LC_NUMERIC
#define XLC_TIME		LC_TIME

#define XLC_MIN			XLC_ALL
#define XLC_MAX			XLC_TIME

#if defined(_WIN32) || defined(_WIN64)
typedef _locale_t xlocale_t;

#define XLC_ALL_MASK			XLC_ALL
#define XLC_COLLATE_MASK		XLC_COLLATE
#define XLC_CTYPE_MASK			XLC_CTYPE
#define XLC_MONETARY_MASK		XLC_MONETARY
#define XLC_NUMERIC_MASK		XLC_NUMERIC
#define XLC_TIME_MASK			XLC_TIME
#else
typedef locale_t xlocale_t;

#define XLC_MESSAGES		LC_MESSAGES
#define XLC_PAPER		LC_PAPER
#define XLC_NAME		LC_NAME
#define XLC_ADDRESS		LC_ADDRESS
#define XLC_TELEPHONE		LC_TELEPHONE
#define XLC_MEASUREMENT		LC_MEASUREMENT
#define XLC_IDENTIFICATION	LC_IDENTIFICATION

#define XLC_CTYPE_MASK		LC_CTYPE_MASK
#define XLC_NUMERIC_MASK	LC_NUMERIC_MASK
#define XLC_TIME_MASK		LC_TIME_MASK
#define XLC_COLLATE_MASK	LC_COLLATE_MASK
#define XLC_MONETARY_MASK	LC_MONETARY_MASK
#define XLC_MESSAGES_MASK	LC_MESSAGES_MASK
#define XLC_PAPER_MASK		LC_PAPER_MASK
#define XLC_NAME_MASK		LC_NAME_MASK
#define XLC_ADDRESS_MASK	LC_ADDRESS_MASK
#define XLC_TELEPHONE_MASK	LC_TELEPHONE_MASK
#define XLC_MEASUREMENT_MASK	LC_MEASUREMENT_MASK
#define XLC_IDENTIFICATION_MASK	LC_IDENTIFICATION_MASK
#define XLC_ALL_MASK		LC_ALL_MASK
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_INLINE_API wchar_t *wsetlocale(int category, const wchar_t *req_locale) {
#if defined(_WIN32) || defined(_WIN64)
	return _wsetlocale(category, req_locale);
#else
	static const char *utf8_aliases[] = {"C.UTF-8","en_US.UTF-8","UTF-8",".UTF8",".utf8","utf-8",".65001",NULL};

	char *mbs;
	char *ret;

	/* 1. Try requested locale first */
	if (req_locale != NULL) {
		mbs = xwcstombs(req_locale);
		if (mbs != NULL) {
			ret = setlocale(category, mbs);
			free(mbs);
			if (ret != NULL)
				return xmbstowcs(ret);
		}
	}

	/* 2. Try UTF-8 fallback aliases */
	for (int i = 0; utf8_aliases[i] != NULL; i++) {
		ret = setlocale(category, utf8_aliases[i]);
		if (ret != NULL)
			return xmbstowcs(ret);
	}

	/* 3. Try system default ("") */
	ret = setlocale(category, "");
	if (ret != NULL)
		return xmbstowcs(ret);

	/* 4. Final fallback: ANSI C locale */
	ret = setlocale(category, "C");
	if (ret != NULL)
		return xmbstowcs(ret);

	/* 5. Absolute failure */
	return NULL;
#endif
}



XSTDDEF_INLINE_API char *xsetlocale(int category, const char *req_locale) {
	static const char *utf8_aliases[] = {"C.UTF-8","en_US.UTF-8","UTF-8",".UTF8",".utf8","utf-8",".65001",NULL};

	// 1. Try the requested locale first
	if (req_locale && req_locale[0]) {
		char *ret = setlocale(category, req_locale);
		if (ret) return ret;
	}

	// 2. Try UTF-8 aliases
	for (int i = 0; utf8_aliases[i]; i++) {
		char *ret = setlocale(category, utf8_aliases[i]);
		if (ret) return ret;
	}

	// 3. Try system default locale ("")
	{
		char *ret = setlocale(category, "");
		if (ret) return ret;
	}

	// 4. Final fallback: guaranteed to exist on all systems
	return setlocale(category, "C");
}

XSTDDEF_INLINE_API wchar_t *xwsetlocale(int category, const wchar_t *req_locale) {
	static const wchar_t *utf8_aliases[] = { L"C.UTF-8",L"en_US.UTF-8",L"UTF-8",L".UTF8",L".utf8",L"utf-8",L".65001",NULL };

	// 1. Try the requested locale first
	if (req_locale && req_locale[0]) {
		wchar_t *ret = wsetlocale(category, req_locale);
		if (ret) return ret;
	}

	// 2. Try UTF-8 aliases
	for (int i = 0; utf8_aliases[i]; i++) {
		wchar_t *ret = wsetlocale(category, utf8_aliases[i]);
		if (ret) return ret;
	}

	// 3. Try system default locale ("")
	{
		wchar_t *ret = wsetlocale(category, L"");
		if (ret) return ret;
	}

	// 4. Final fallback: guaranteed to exist on all systems
	return wsetlocale(category, L"C");
}

XSTDDEF_INLINE_API void xfreelocale(xlocale_t loc) {
	if (!loc) return;
#if defined(_WIN32) || defined(_WIN64)
	_free_locale(loc);
#else
	freelocale(loc);
#endif
}

XSTDDEF_INLINE_API xlocale_t xnewlocale(int category_mask,const char *req_locale,xlocale_t base) {
	static const char *utf8_aliases[] = {"C.UTF-8","en_US.UTF-8","UTF-8",".UTF8",".utf8","utf-8",".65001",NULL};

#if defined(_WIN32) || defined(_WIN64)
	/* Windows path ----------------------------------------------------- */

	/* 1. Try user-provided locale first */
	if (req_locale && req_locale[0]) {
		xlocale_t loc = _create_locale(category_mask, req_locale);
		if (loc) return loc;
	}

	/* 2. Try UTF-8 aliases */
	for (int i = 0; utf8_aliases[i]; ++i) {
		xlocale_t loc = _create_locale(category_mask, utf8_aliases[i]);
		if (loc) return loc;
	}

	/* 3. Last resort: "C" locale */
	return _create_locale(category_mask, "C");

#else
	/* POSIX path -------------------------------------------------------- */

	/* 1. Try user-provided locale first */
	if (req_locale && req_locale[0]) {
		xlocale_t loc = newlocale(category_mask, req_locale, base);
		if (loc) return loc;
	}

	/* 2. Try UTF-8 aliases */
	for (int i = 0; utf8_aliases[i]; ++i) {
		xlocale_t loc = newlocale(category_mask, utf8_aliases[i], base);
		if (loc) return loc;
	}

	/* 3. Try system default */
	{
		xlocale_t loc = newlocale(category_mask, "", base);
		if (loc) return loc;
	}

	/* 4. Final fallback: "C" */
	return newlocale(category_mask, "C", base);
#endif
}

XSTDDEF_INLINE_API void xinit_console_utf8(void) {
#if defined(_WIN32) || defined(_WIN64)
	static int console_utf8_initialized = 0;
	if (console_utf8_initialized) return;
	console_utf8_initialized = 1;
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	_setmode(_fileno(stdout), _O_U8TEXT);
#endif
	xsetlocale(XLC_ALL,"C.UTF-8");
}

XSTDDEF_INLINE_API void xinit_console_wutf(void) {
#if defined(_WIN32) || defined(_WIN64)
	static int console_wutf_initialized = 0;
	if (console_wutf_initialized) return;
	console_wutf_initialized = 1;
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
#endif
	xsetlocale(XLC_ALL,"C.UTF-8");
}

#ifdef __cplusplus
}
#endif

#endif // __XLOCALE_H__
