/**
 * xtypes.h: Extern sys/types.h
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

#ifndef __XTYPES_H__
#define __XTYPES_H__

#include "xstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef _mode_t xmode_t;
typedef _locale_t xlocale_t;
#else
typedef void *HMODULE;
typedef void *FARPROC;
typedef const char *LPCSTR;
typedef const wchar_t *LPCWSTR;

typedef mode_t xmode_t;
typedef locale_t xlocale_t;
#endif

typedef struct {
	const char *dli_fname;	/* File name of defining object.  */
	void *dli_fbase;		/* Load address of that object.  */
	const char *dli_sname;	/* Name of nearest symbol.  */
	void *dli_saddr;		/* Exact value of nearest symbol.  */
} XDl_info;

#ifdef __cplusplus
}
#endif

#endif