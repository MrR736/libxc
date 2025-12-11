/**
 * xstddef.h: Extern stddef.h
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

#ifndef __XSTDDEF_H__
#define __XSTDDEF_H__

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <wctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32) || defined(_WIN64)
#include <process.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

#define XPATH_MAX 32767

#ifndef __LIBXC_VERSION_MAJOR
#define __LIBXC_VERSION_MAJOR 1
#endif
#ifndef __LIBXC_VERSION_MINOR
#define __LIBXC_VERSION_MINOR 0
#endif
#ifndef __LIBXC_VERSION_PATCH
#define __LIBXC_VERSION_PATCH 3
#endif

#define __LIBXC_VERSION ((__LIBXC_VERSION_MAJOR * 1000) + (__LIBXC_VERSION_MINOR * 100) + __LIBXC_VERSION_PATCH)

#ifndef __libxc_types_compatible_p
#ifdef __cplusplus
extern "C++" {
template <typename t1, typename t2>
struct __libxc_types_compatible_p {
	static const bool result = false;
};

template <typename t1>
struct __libxc_types_compatible_p<t1, t1> {
	static const bool result = true;
};

template <typename t1>
struct __libxc_types_compatible_p<const t1, t1> {
	static const bool result = true;
};

template <typename t1>
struct __libxc_types_compatible_p<t1, const t1> {
	static const bool result = true;
};
}

#define __libxc_types_compatible_p(t1, t2) __libxc_types_compatible_p<t1,t2>::result
#else
#define __libxc_types_compatible_p(t1, t2) __builtin_types_compatible_p(t1,t2)
#endif
#endif /* !__libxc_types_compatible_p */

#define xsizeof(x) (sizeof(x) / sizeof((x)[0]))

#define xcountof(a) (sizeof(a) / sizeof((a)[0]) + \
		     0 * sizeof(struct { int _ : ((void*)(a) == (void*)&(a)[0]); }))

#define countof(a) \
	(0 * sizeof(struct { \
		int _type_check: \
			!__libxc_types_compatible_p(typeof(a), typeof(&(a)[0])); \
	}) + (sizeof(a) / sizeof((a)[0])))

#if defined(_MSC_VER) && _MSC_VER < 1900 && !defined(inline)
#define inline __inline
#endif

#if defined(__GNUC__) || defined(__clang__)
#define __xattribute__(V) __attribute__( V )
#if __has_attribute(visibility)
#ifdef ARM
#define __xvisibility__(V) __attribute__((externally_visible,visibility(#V)))
#else
#define __xvisibility__(V) __attribute__((visibility(#V)))
#endif
#else
#define __xvisibility__(V)
#endif
#else
#define __xattribute__(V)
#define __xvisibility__(V)
#endif

#ifdef __GNUC__
#ifndef _MSC_VER
#define __restrict	__restrict
#else
#define __restrict	__restrict__
#endif /* !_MSC_VER */
#else
#define __restrict	/* nothing */
#endif /* !__GNUC__ */

#ifndef XSTDDEF_EXPORT_API
#define XSTDDEF_EXPORT_API	__xvisibility__(default)
#endif

#ifndef XSTDDEF_IMPORT_API
#define XSTDDEF_IMPORT_API	extern __xvisibility__(default)
#endif

#ifndef XSTDDEF_INLINE_API
#define XSTDDEF_INLINE_API	static inline
#endif

#endif // __XSTDDEF_H__
