/**
 * xxlocale.h: Extern locale.h
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

#ifndef __XXLOCALE_H__
#define __XXLOCALE_H__

#include "xstring.h"
#include "xtypes.h"
#include "xstdlib.h"
#include "xwchar.h"
#include <wchar.h>

#define	XLC_ALL					LC_ALL
#define XLC_COLLATE				LC_COLLATE
#define XLC_CTYPE				LC_CTYPE
#define XLC_MONETARY			LC_MONETARY
#define XLC_NUMERIC				LC_NUMERIC
#define XLC_TIME				LC_TIME

#define XLC_MIN					XLC_ALL
#define XLC_MAX					XLC_TIME

#ifdef _WIN32
#define XLC_ALL_MASK			XLC_ALL
#define XLC_COLLATE_MASK		XLC_COLLATE
#define XLC_CTYPE_MASK			XLC_CTYPE
#define XLC_MONETARY_MASK		XLC_MONETARY
#define XLC_NUMERIC_MASK		XLC_NUMERIC
#define XLC_TIME_MASK			XLC_TIME
#else
#define XLC_ALL_MASK			LC_ALL_MASK
#define XLC_COLLATE_MASK		LC_COLLATE_MASK
#define XLC_CTYPE_MASK			LC_CTYPE_MASK
#define XLC_MONETARY_MASK		LC_MONETARY_MASK
#define XLC_NUMERIC_MASK		LC_NUMERIC_MASK
#define XLC_TIME_MASK			LC_TIME_MASK

#ifndef __APPLE__
#define XLC_MESSAGES			LC_MESSAGES
#define XLC_PAPER				LC_PAPER
#define XLC_NAME				LC_NAME
#define XLC_ADDRESS				LC_ADDRESS
#define XLC_TELEPHONE			LC_TELEPHONE
#define XLC_MEASUREMENT			LC_MEASUREMENT
#define XLC_IDENTIFICATION		LC_IDENTIFICATION

#define XLC_MESSAGES_MASK		LC_MESSAGES_MASK
#define XLC_PAPER_MASK			LC_PAPER_MASK
#define XLC_NAME_MASK			LC_NAME_MASK
#define XLC_ADDRESS_MASK		LC_ADDRESS_MASK
#define XLC_TELEPHONE_MASK		LC_TELEPHONE_MASK
#define XLC_MEASUREMENT_MASK	LC_MEASUREMENT_MASK
#define XLC_IDENTIFICATION_MASK	LC_IDENTIFICATION_MASK
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI char** XCALLAPI xgetlocale(size_t* out_size) {
	if (out_size) *out_size = 0;
#ifdef _WIN32
	const char *xlocale_candidates[] = {
		"C",
		"",
		".ACP",
		".OCP",
		".65001",
		".1252",
		".437",
		"UTF-8",
		"UTF8",
		"C.UTF-8",
		"C.utf8",
		".UTF8",
		".UTF-8",
		"en_US.UTF-8"
	};

	if (out_size) *out_size = xsizeof(xlocale_candidates);
	return xstrsdup(xlocale_candidates, xsizeof(xlocale_candidates));
#else
	FILE *fp;
	char line[512];
	char **locales = NULL;
	size_t count = 0;

	fp = popen("locale -a", "r");
	if (fp == NULL) return NULL;

	while (fgets(line, sizeof(line), fp) != NULL) {
		size_t n = strcspn(line, "\r\n");
		char *name;
		char **tmp;

		line[n] = '\0';

		if (n == 0) continue;

		/* Avoid duplicate locale names. */
		{
			int found = 0;

			for (size_t i = 0; i < count; ++i) {
				if (xstricmp(line, locales[i]) == 0) {
					found = 1;
					break;
				}
			}
			if (found) continue;
		}

		name = (char *)xmalloc(n + 1);
		if (name == NULL) goto fail;
		xmemcpy(name, line, n + 1);
		tmp = (char **)xrealloc(locales, (count + 2) * sizeof(*locales));

		if (tmp == NULL) {
			xfree(name);
			goto fail;
		}

		locales = tmp;
		locales[count++] = name;
		locales[count] = NULL;
	}

	/* Add common encoding and locale aliases if missing. */
	{
		static const char *const aliases[] = {
			"UTF-8",
			"UTF8",
			"C.UTF-8",
			"C.utf8",
			".UTF8",
			".UTF-8",
			"en_US.UTF-8",
			NULL
		};

		for (size_t i = 0; aliases[i] != NULL; ++i) {
			size_t n;
			char *name;
			char **tmp;
			int found = 0;

			for (size_t j = 0; j < count; ++j) {
				if (xstricmp(aliases[i], locales[j]) == 0) {
					found = 1;
					break;
				}
			}

			if (found)
				continue;

			n = strlen(aliases[i]);

			name = (char *)xmalloc(n + 1);
			if (name == NULL)
				goto fail;

			xmemcpy(name, aliases[i], n + 1);

			tmp = (char **)xrealloc(
				locales, (count + 2) * sizeof(*locales));

			if (tmp == NULL) {
				xfree(name);
				goto fail;
			}

			locales = tmp;
			locales[count++] = name;
			locales[count] = NULL;
		}
	}

	if (out_size != NULL) *out_size = count;

	pclose(fp);
	return locales;

fail:
	pclose(fp);
	while (count > 0) xfree(locales[--count]);
	xfree(locales);
	return NULL;
#endif
}

/* Release the returned array. */
XSTDAPI void XCALLAPI xfreelocales(char **locales) {
	if (locales == NULL) return;
	for (size_t i = 0; locales[i] != NULL; ++i) xfree(locales[i]);
	xfree(locales);
}

XSTDAPI wchar_t** XCALLAPI xwgetlocale(size_t *out_size) {
	if (out_size) *out_size = 0;
#ifdef _WIN32
	const wchar_t *xlocale_candidates[] = {
		L"C",
		L"",
		L".ACP",
		L".OCP",
		L".65001",
		L".1252",
		L".437",
		L"UTF-8",
		L"UTF8",
		L"C.UTF-8",
		L"C.utf8",
		L".UTF8",
		L".UTF-8",
		L"en_US.UTF-8"
	};

	if (out_size) *out_size = xsizeof(xlocale_candidates);
	return xwcssdup(xlocale_candidates, xsizeof(xlocale_candidates));
#else
	size_t n = 0;
	char **ss;
	wchar_t **ww;
	ss = xgetlocale(&n);
	if (!ss) return NULL;
	ww = xmbsstowcss((const char**)ss, n);
	xfrees((void **)ss, n);
	if (!ww) return NULL;
	if (out_size) *out_size = n;
	return ww;
#endif
}

XSTDAPI void XCALLAPI xwfreelocales(wchar_t **locales) {
	if (locales == NULL) return;
	for (size_t i = 0; locales[i] != NULL; ++i) xfree(locales[i]);
	xfree(locales);
}

XSTDAPI wchar_t* XCALLAPI wsetlocale(int category, const wchar_t *req_locale) {
#ifdef _WIN32
	return _wsetlocale(category, req_locale);
#else
	char *lret;
	wchar_t *ret;
	char *req = NULL;

	if (req_locale != NULL) {
		req = xwcstombs(req_locale);
		if (!req)
			return NULL;
	}

	lret = setlocale(category, req);

	if (req)
		xfree(req);

	if (!lret)
		return NULL;

	ret = xmbstowcs(lret);
	return ret;
#endif
}

XSTDAPI char *XCALLAPI xsetlocale(int category, const char *req_locale) {
	char *ret;
	size_t n;

	if (req_locale == NULL) return setlocale(category, NULL);

	if (req_locale[0] == '\0') return setlocale(category, "");

	char **locales = xgetlocale(&n);
	if (locales == NULL) return setlocale(category, req_locale);

	for (size_t i = 0; i < n; i++) {
		if (xstricmp(req_locale,locales[i]) == 0) {
#ifdef _WIN32
			ret = setlocale(category, ".65001");
#else
			ret = setlocale(category, "C.utf8");
#endif
			xfreelocales(locales);
			return ret;
		}
	}

	xfreelocales(locales);
	return setlocale(category, req_locale);
}


XSTDAPI wchar_t* XCALLAPI xwsetlocale(int category,const wchar_t *req_locale) {
	wchar_t *ret;
	size_t n;

	if (req_locale == NULL) return wsetlocale(category, NULL);

	if (req_locale[0] == L'\0') return wsetlocale(category,L"");

	wchar_t **locales = xwgetlocale(&n);
	if (locales == NULL) return wsetlocale(category, req_locale);

	for (size_t i = 0; i < n; i++) {
		if (xwcsicmp(req_locale,locales[i]) == 0) {
#ifdef _WIN32
			ret = wsetlocale(category,L".65001");
#else
			ret = wsetlocale(category,L"C.utf8");
#endif
			xwfreelocales(locales);
			return ret;
		}
	}

	xwfreelocales(locales);
	return wsetlocale(category, req_locale);
}

XSTDAPI void XCALLAPI xfreelocale(xlocale_t loc) {
#ifdef _WIN32
	_free_locale(loc);
#else
	freelocale(loc);
#endif
}

XSTDAPI xlocale_t XCALLAPI xnewlocale(int category_mask,const char *req_locale,xlocale_t base) {
	static const char *utf8_aliases[] = {"C.UTF-8","en_US.UTF-8","UTF-8",".UTF8",".utf8","utf-8",".65001",NULL};

#ifdef _WIN32
	/* Windows path ----------------------------------------------------- */
	(void)base;

	/*
	 * Windows _create_locale() does not understand the POSIX
	 * category mask used by xnewlocale().
	 *
	 * Create one complete Windows locale object instead.
	 */

	/*
	 * If no locale name is supplied, use the base locale when
	 * available. Otherwise create the "C" locale.
	 */
	if (req_locale == NULL) {
		if (base != NULL) {
			/*
			 * _locale_t is already a locale object. Make a copy
			 * so the returned object can be independently freed.
			 */
			return _create_locale(LC_ALL, "C");
		}
		return _create_locale(LC_ALL, "C");
	}

	/* Try the requested locale first. */
	xlocale_t loc = _create_locale(LC_ALL, req_locale);
	if (loc != NULL) return loc;

	/* Try UTF-8 aliases. */
	for (int i = 0; utf8_aliases[i] != NULL; ++i) {
		loc = _create_locale(LC_ALL, utf8_aliases[i]);
		if (loc != NULL) return loc;
	}

	return NULL;
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

XSTDAPI xlocale_t XCALLAPI xwnewlocale(int category_mask, const wchar_t* req_locale, xlocale_t base) {
	char* mbs = xwcstombs(req_locale);
	if (!mbs) return NULL;
	xlocale_t ret = xnewlocale(category_mask, mbs, base);
	xfree(mbs);
	return ret;
}

XSTDAPI void XCALLAPI xinit_console_utf8(void) {
#ifdef _WIN32
	static int console_utf8_initialized = 0;
	if (console_utf8_initialized) return;
	console_utf8_initialized = 1;
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	_setmode(_fileno(stdout), _O_U8TEXT);
	_setmode(_fileno(stderr), _O_U8TEXT);
#endif
	xsetlocale(XLC_ALL,"C.UTF-8");
}

XSTDAPI void XCALLAPI xinit_console_wutf(void) {
#ifdef _WIN32
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

#endif // __XXLOCALE_H__
