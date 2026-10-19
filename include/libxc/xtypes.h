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
#include <locale.h>

#ifdef __APPLE__
#include <xlocale.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef _mode_t xmode_t;
typedef _locale_t xlocale_t;
#else
typedef uint32_t DWORD;
typedef void *PVOID;
typedef void *LPVOID;
typedef float FLOAT;
typedef void *HMODULE;
typedef void *FARPROC;
typedef const char *LPCSTR;
typedef const wchar_t *LPCWSTR;
typedef int WINBOOL;

typedef DWORD (XCALLAPI *PTHREAD_START_ROUTINE) (LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef mode_t xmode_t;
typedef locale_t xlocale_t;
#endif

typedef HMODULE xhandle_t;
typedef FARPROC xproc_t;

#ifndef XSTD_CHAR_TYPES_DEFINED
#define XSTD_CHAR_TYPES_DEFINED
#ifdef __cplusplus
# if !CPLUSPLUS_GE(STDC11PP)
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
# endif

# if !CPLUSPLUS_GE(STDC20PP)
typedef unsigned char char8_t;
# endif
#else
typedef unsigned char char8_t;
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif