/**
 * xstddef.h: Extern stddef.h
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

#ifndef __XSTDDEF_H__
#define __XSTDDEF_H__

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winnls.h>
#include <process.h>
#include <aclapi.h>
#include <synchapi.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")
#else
#ifdef __ANDROID__
#include <android/log.h>
#endif
#include <sys/random.h>
#include <unistd.h>
#include <sys/wait.h>
#endif

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <wctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <wchar.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <uchar.h>

#ifdef __cplusplus
#include <string>
#include <memory>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef XPATH_MAX
#define XPATH_MAX 32767
#endif

#ifndef __LIBXC_VERSION_MAJOR
#define __LIBXC_VERSION_MAJOR 1
#endif
#ifndef __LIBXC_VERSION_MINOR
#define __LIBXC_VERSION_MINOR 0
#endif
#ifndef __LIBXC_VERSION_PATCH
#define __LIBXC_VERSION_PATCH 8
#endif

#define __LIBXC_VERSION ((__LIBXC_VERSION_MAJOR * 1000) + (__LIBXC_VERSION_MINOR * 100) + __LIBXC_VERSION_PATCH)

#define __LIBXC_VERSION_AT_LEAST(major,minor,patch) \
	(((major * 1000) + (minor * 100) + patch) <= __LIBXC_VERSION)

#define __LIBXC_VERSION_AT_MOST(major,minor,patch) \
	(((major * 1000) + (minor * 100) + patch) >= __LIBXC_VERSION)

#define __LIBXC_VERSION_EQ(major,minor,patch) \
	(((major * 1000) + (minor * 100) + patch) == __LIBXC_VERSION)

#define __LIBXC_VERSION_NE(major,minor,patch) \
	(((major * 1000) + (minor * 100) + patch) != __LIBXC_VERSION)

/* ----------------- C Standard Versions ----------------- */
#define STDC94		199409L  /**< C90 amendment (ISO/IEC 9899:1990 + 1994) */
#define STDC99		199901L  /**< C99 (ISO/IEC 9899:1999) */
#define STDC11		201112L  /**< C11 (ISO/IEC 9899:2011) */
#define STDC1X		STDC11   /**< C++1x */
#define STDC17		201710L  /**< C17 / C18 (ISO/IEC 9899:2017/2018) */
#define STDC18		STDC17   /**< Alias for C17 */
#define STDC23		202311L  /**< C23 (ISO/IEC 9899:2023) */
#define STDC2X		STDC23   /**< Alias for C23 */

/* ----------------- C++ Standard Versions ----------------- */
#define STDC98PP	199711L  /**< C++98 */
#define STDC03PP	STDC98PP /**< C++03 */
#define STDC11PP	201103L  /**< C++11 */
#define STDC0XPP	STDC11PP /**< C++0x */
#define STDC14PP	201402L  /**< C++14 */
#define STDC1YPP	STDC14PP /**< C++1Y */
#define STDC17PP	201703L  /**< C++17 */
#define STDC20PP	202002L  /**< C++20 */
#define STDC2APP	STDC20PP /**< C++2A */

#if __cplusplus == 202100L   /**< C++23 */
#define STDC23PP	202100L
#elif __cplusplus == 202302L
#define STDC23PP	202302L
#else
#define STDC23PP	202300L
#endif

#define STDC2BPP	STDC23PP /**< C++2B */

#if __cplusplus == 202400L   /**< C++26 */
#define STDC26PP	202400L
#else
#define STDC26PP	202600L
#endif

/* ----------------- C Standard Version Comparison Macros ----------------- */
#ifdef __STDC_VERSION__
# define STDC_VERSION_GE(v) (__STDC_VERSION__ >= (v))
# define STDC_VERSION_LE(v) (__STDC_VERSION__ <= (v))
# define STDC_VERSION_EQ(v) (__STDC_VERSION__ == (v))
# define STDC_VERSION_NE(v) (__STDC_VERSION__ != (v))
# define STDC_VERSION_LT(v) (__STDC_VERSION__ < (v))
# define STDC_VERSION_GT(v) (__STDC_VERSION__ > (v))
#else
# define STDC_VERSION_GE(v) 0
# define STDC_VERSION_LE(v) 0
# define STDC_VERSION_EQ(v) 0
# define STDC_VERSION_NE(v) 1
# define STDC_VERSION_LT(v) 0
# define STDC_VERSION_GT(v) 0
#endif

/* ----------------- C++ Standard Version Comparison Macros ----------------- */
#ifdef __cplusplus
# define CPLUSPLUS_GE(v) (__cplusplus >= (v))
# define CPLUSPLUS_LE(v) (__cplusplus <= (v))
# define CPLUSPLUS_EQ(v) (__cplusplus == (v))
# define CPLUSPLUS_NE(v) (__cplusplus != (v))
# define CPLUSPLUS_LT(v) (__cplusplus < (v))
# define CPLUSPLUS_GT(v) (__cplusplus > (v))
#else
# define CPLUSPLUS_GE(v) 0
# define CPLUSPLUS_LE(v) 0
# define CPLUSPLUS_EQ(v) 0
# define CPLUSPLUS_NE(v) 1
# define CPLUSPLUS_LT(v) 0
# define CPLUSPLUS_GT(v) 0
#endif

/** @brief Use 64-bit pointer architecture */
#ifdef UINTPTR_MAX
# if UINTPTR_MAX == UINT64_MAX
#  define _XSTD_x64 1
# elif UINTPTR_MAX == UINT32_MAX
#  define _XSTD_x32 1
# else
#  error "Unknown pointer size! Cannot determine 32/64-bit architecture."
# endif
#else
# if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || \
defined(__aarch64__) || defined(__LP64__) || defined(_LP64) || defined(__amd64__) || \
(defined(__WORDSIZE) && __WORDSIZE == 64) || (defined(__riscv_xlen) && __riscv_xlen == 64) || defined(__powerpc64__)
#  define _XSTD_x64 1
# elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || (defined(__riscv_xlen) && __riscv_xlen == 32)
#  define _XSTD_x32 1
# endif
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__)
# define _XSTD_x86_64 1
#endif

#if defined(_M_I86) || defined(__i386__)
# define _XSTD_i386 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
# define _XSTD_ARM64 1
#endif

#if defined(__arm__) || defined(_M_ARM)
# define _XSTD_ARM 1
#endif

#if defined(__mips64)
# define _XSTD_MIPS64 1
#endif

#if defined(__mips__) || defined(__mips)
# define _XSTD_MIPS 1
#endif

#if defined(_XSTD_x64) && defined(__AVX2__)
# define _XSTD_SIMD_AVX2 1
#elif defined(_XSTD_x64) && defined(__AVX__)
# define _XSTD_SIMD_AVX 1
#elif defined(_XSTD_x64) && defined(__SSE4_2__)
# define _XSTD_SIMD_SSE42 1
#else
# define _XSTD_SIMD_NONE 1
#endif

#if !defined(_XSTD_SIMD_NONE)
# define _XSTD_MODERN_CPU 1
#else
# define _XSTD_LEGACY_CPU 1
#endif

#if defined(__has_feature)
# if __has_feature(address_sanitizer)
#  define _XSTD_ASAN 1
# endif
# if __has_feature(thread_sanitizer)
#  define _XSTD_TSAN 1
# endif
#elif defined(__SANITIZE_ADDRESS__)
# define _XSTD_ASAN 1
#elif defined(__SANITIZE_THREAD__)
# define _XSTD_TSAN 1
#endif

#ifndef __xstd_builtin
# ifdef __has_builtin
#  define __xstd_builtin(x) __has_builtin(x)
# else
#  define __xstd_builtin(x) 0
# endif
#endif

#ifndef __libxc_types_compatible_p
# ifdef __cplusplus
extern "C++" {
#  if CPLUSPLUS_GE(STDC20PP)
#   include <type_traits>
#   define __libxc_types_compatible_p(t1, t2) \
	std::is_same_v<std::remove_cvref_t<t1>, std::remove_cvref_t<t2>>
#  elif CPLUSPLUS_GE(STDC17PP)
#   include <type_traits>
#   define __libxc_types_compatible_p(t1, t2) \
	std::is_same_v<std::remove_cv_t<t1>, std::remove_cv_t<t2>>
#  elif CPLUSPLUS_GE(STDC11PP)
#   include <type_traits>
#   define __libxc_types_compatible_p(t1, t2) \
	std::is_same<typename std::remove_cv<t1>::type,typename std::remove_cv<t2>::type>::value
#  else
template <typename T1, typename T2>
struct __xstd_types_compatible_impl {
	static const bool value = false;
};
template <typename T>
struct __xstd_types_compatible_impl<T, T> {
	static const bool value = true;
};
template <typename T>
struct __xstd_types_compatible_impl<const T, T> {
	static const bool value = true;
};
template <typename T>
struct __xstd_types_compatible_impl<T, const T> {
	static const bool value = true;
};
template <typename T>
struct __xstd_types_compatible_impl<volatile T, T> {
	static const bool value = true;
};
template <typename T>
struct __xstd_types_compatible_impl<T, volatile T> {
	static const bool value = true;
};
template <typename T>
struct __xstd_types_compatible_impl<const volatile T, T> {
	static const bool value = true;
};
template <typename T>
struct __xstd_types_compatible_impl<T, const volatile T> {
	static const bool value = true;
};
#   define __libxc_types_compatible_p(t1, t2) __xstd_types_compatible_impl<t1, t2>::value
#  endif
} // extern "C++"
# else /* C */
#  if __xstd_builtin(__builtin_types_compatible_p)
#   define __libxc_types_compatible_p(t1, t2) __builtin_types_compatible_p(t1, t2)
#  else
#   define __libxc_types_compatible_p(t1, t2) 0
#  endif
# endif
#endif /* !__libxc_types_compatible_p */

#define xsizeof(x) (sizeof(x) / sizeof((x)[0]))

#define xcountof(a) (sizeof(a) / sizeof((a)[0]) + \
			0 * sizeof(struct { int _ : ((void*)(a) == (void*)&(a)[0]); }))

#define countof(a) \
	(0 * sizeof(struct { \
		int _type_check: !__libxc_types_compatible_p(typeof(a), typeof(&(a)[0])); \
	}) + (sizeof(a) / sizeof((a)[0])))

#if defined(_MSC_VER) && _MSC_VER < 1900 && !defined(__cplusplus) && !defined(inline)
# define inline __inline
#endif

#if defined(__GNUC__) || defined(__clang__)
# define __xattribute__(V) __attribute__( V )
# ifdef __has_attribute
#  define __has_xattribute(V) __has_attribute(V)
#  if __has_xattribute(visibility)
#   define __xvisibility__(V) __attribute__( (visibility( #V )) )
#  else
#   define __xvisibility__(V)
#  endif
# else
#  define __has_xattribute(V)
#  define __xvisibility__(V)
# endif
#else
# define __has_xattribute(V)
# define __xattribute__(V)
# define __xvisibility__(V)
#endif

#ifdef __GNUC__
#ifndef _MSC_VER
#define xrestrict	__restrict
#else
#define xrestrict	__restrict__
#endif /* !_MSC_VER */
#else
#define xrestrict	/* nothing */
#endif /* !__GNUC__ */

#ifndef __xrestrict
#define __xrestrict		xrestrict
#endif

#ifndef __xrestrict__
#define __xrestrict__	xrestrict
#endif

#ifndef XTHREAD_LOCAL
# if defined(__cplusplus)
#  if __cplusplus >= 201103L
#   define XTHREAD_LOCAL thread_local
#  elif defined(_MSC_VER)
#   define XTHREAD_LOCAL __declspec(thread)
#  elif defined(__GNUC__) || defined(__clang__)
#   define XTHREAD_LOCAL __thread
#  endif
# elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define XTHREAD_LOCAL _Thread_local
# elif defined(_MSC_VER)
#  define XTHREAD_LOCAL __declspec(thread)
# elif defined(__GNUC__) || defined(__clang__)
#  define XTHREAD_LOCAL __thread
# endif
#endif

#define xthread_local XTHREAD_LOCAL
#define __xthread XTHREAD_LOCAL

#ifndef XSTDDEF_EXPORT_API
#ifdef _WIN32
#define XSTDDEF_EXPORT_API	__declspec(dllexport)
#else
#define XSTDDEF_EXPORT_API	__xvisibility__(default)
#endif
#endif

#ifndef XSTDDEF_IMPORT_API
#ifdef _WIN32
#define XSTDDEF_IMPORT_API	__declspec(dllimport)
#else
#define XSTDDEF_IMPORT_API	__xvisibility__(default)
#endif
#endif

#ifndef XSTDDEF_HIDDEN_API
#define XSTDDEF_HIDDEN_API	__xvisibility__(hidden)
#endif

#ifndef XSTDDEF_INLINE_API
#define XSTDDEF_INLINE_API	static inline
#endif

#if defined(XSTDAPI_BUILD) && defined(XSTDAPI_STATIC_BUILD)
# error "XSTDAPI_BUILD and XSTDAPI_STATIC_BUILD cannot both be defined."
#endif

#if !defined(XSTDAPI_BUILD) && !defined(XSTDAPI_STATIC_BUILD)
# define XSTDAPI_STATIC_BUILD
#endif

#ifndef XSTDAPI
# ifdef XSTDAPI_STATIC
#  define XSTDAPI
# elif defined(XSTDAPI_STATIC_BUILD)
#  define XSTDAPI XSTDDEF_INLINE_API
# elif defined(XSTDAPI_BUILD)
#  define XSTDAPI XSTDDEF_EXPORT_API
# else
#  define XSTDAPI XSTDDEF_IMPORT_API
# endif
#endif

#ifndef XCALLAPI
#ifdef _WIN32
#define XCALLAPI	WINAPI
#else
#define XCALLAPI
#endif
#endif

#ifndef XNULLABLE
# if defined(__clang__)
#  if __has_feature(nullability)
#   define XNULLABLE _Nullable
#  else
#   define XNULLABLE
#  endif
# else
#  define XNULLABLE
# endif
#endif

#ifdef UNICODE
# define __LIBXC_UNICODE_MACROS 1
# define __LIBXC_NAME_AW(func) func##W
# define __LIBXC_STRING_AW(str) L##str
#else
# define __LIBXC_UNICODE_MACROS 2
# define __LIBXC_NAME_AW(func) func##A
# define __LIBXC_STRING_AW(str) str
#endif

#ifndef XTEXT
# define XTEXT(str) __LIBXC_STRING_AW(str)
#endif

#if __has_xattribute(always_inline)
# define LIBXC_ALWAYS_INLINE __xattribute__((__always_inline__))
#else
# define LIBXC_ALWAYS_INLINE
#endif

#if __has_xattribute(exclude_from_explicit_instantiation)
# define LIBXC_EXCLUDE_FROM_EXPLICIT_INSTANTIATION __xattribute__((__exclude_from_explicit_instantiation__))
#else
# define LIBXC_EXCLUDE_FROM_EXPLICIT_INSTANTIATION LIBXC_ALWAYS_INLINE
#endif

#if __has_xattribute(__using_if_exists__)
# define LIBXC_USING_IF_EXISTS __xattribute__((__using_if_exists__))
#else
# define LIBXC_USING_IF_EXISTS
#endif

#ifdef __cplusplus
# if CPLUSPLUS_NE(STDC03PP)
#  define LIBXC_NOEXCEPT noexcept
#  define LIBXC_NOEXCEPT_(...) noexcept(__VA_ARGS__)
# else
#  define LIBXC_NOEXCEPT throw()
#  define LIBXC_NOEXCEPT_(...)
# endif

# ifndef LIBXC_NODISCARD
#  if CPLUSPLUS_GE(STDC17PP)
#   define LIBXC_NODISCARD [[nodiscard]]
#  elif defined(__has_cpp_attribute)
#   if __has_cpp_attribute(nodiscard)
#    define LIBXC_NODISCARD [[nodiscard]]
#   else
#    define LIBXC_NODISCARD
#   endif
#  else
#   define LIBXC_NODISCARD
#  endif
# endif

# ifndef LIBXC_MAYBE_UNUSED
#  if CPLUSPLUS_GE(STDC17PP)
#   define LIBXC_MAYBE_UNUSED [[maybe_unused]]
#  else
#   define LIBXC_MAYBE_UNUSED
#  endif
# endif

# if CPLUSPLUS_GE(STDC20PP)
#  define LIBXC_CONCEPT concept
#  define LIBXC_REQUIRES(...) requires __VA_ARGS__
# else
#  define LIBXC_CONCEPT
#  define LIBXC_REQUIRES(...)
# endif

# if CPLUSPLUS_GE(STDC14PP) || CPLUSPLUS_GE(STDC11PP)
#  define LIBXC_CONSTEXPR constexpr
# else
#  define LIBXC_CONSTEXPR
# endif
#else
# define LIBXC_NOEXCEPT
# define LIBXC_NOEXCEPT_(...)
# define LIBXC_NODISCARD
# define LIBXC_MAYBE_UNUSED
# define LIBXC_CONCEPT
# define LIBXC_REQUIRES(...)
# define LIBXC_CONSTEXPR
#endif

#ifndef __libxc_builtin
# if defined(__has_builtin)
#  define __libxc_builtin(x) __has_builtin(x)
# elif defined(__GNUC__)
#  define __libxc_builtin(x) 1
# else
#  define __libxc_builtin(x) 0
# endif
#endif

#if defined(__cplusplus)
# if CPLUSPLUS_GE(STDC11PP)
#  define LIBXC_STATIC_ASSERT(...) static_assert(__VA_ARGS__)
# else
#  define LIBXC_STATIC_ASSERT(condition, ...) \
typedef char xstd_static_assertion[(condition) ? 1 : -1]
# endif
#else
# if STDC_VERSION_GE(STDC11)
#  define LIBXC_STATIC_ASSERT(...) _Static_assert(__VA_ARGS__)
# else
#  define LIBXC_STATIC_ASSERT(condition, ...) \
typedef char xstd_static_assertion[(condition) ? 1 : -1]
# endif
#endif

#endif // __XSTDDEF_H__
