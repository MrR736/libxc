/**
 * xmemory.h: Cross-Platform Memory Utilities
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

#ifndef __XMEMORY_H__
#define __XMEMORY_H__

#include "xstddef.h"

#ifndef __imalloc
#define __imalloc(s) malloc(s)
#endif

#ifndef __icalloc
#define __icalloc(nmemb,size) calloc(nmemb,size)
#endif

#ifndef __irealloc
#define __irealloc(ptr,size) realloc(ptr,size)
#endif

#ifndef __ifree
#define __ifree(ptr) free(ptr)
#endif


#ifndef __imemset
#define __imemset(s,c,n) memset(s,c,n)
#endif

#ifndef __imemcpy
#define __imemcpy(dest,src,n) memcpy(dest,src,n)
#endif

#ifndef __imemccpy
#define __imemccpy(dest,src,c,n) memccpy(dest,src,c,n)
#endif

#ifndef __imemmove
#define __imemmove(dest,src,n) memmove(dest,src,n)
#endif

#ifndef __imemcmp
#define __imemcmp(s1,s2,n) memcmp(s1,s2,n)
#endif

#ifndef __imempcpy
#define __imempcpy(dest,src,n) mempcpy(dest,src,n)
#endif

#ifndef __imemchr
#define __imemchr(s,c,n) memchr(s,c,n)
#endif


#ifndef __iwmemset
#define __iwmemset(s,c,n) wmemset(s,c,n)
#endif

#ifndef __iwmemcpy
#define __iwmemcpy(dest,src,n) wmemcpy(dest,src,n)
#endif

#ifndef __iwmemmove
#define __iwmemmove(dest,src,n) wmemmove(dest,src,n)
#endif

#ifndef __iwmemcmp
#define __iwmemcmp(s1,s2,n) wmemcmp(s1,s2,n)
#endif

#ifndef __iwmempcpy
#define __iwmempcpy(dest,src,n) wmempcpy(dest,src,n)
#endif

#ifndef __iwmemchr
#define __iwmemchr(s,c,n) wmemchr(s,c,n)
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI void* XCALLAPI xmalloc(size_t size) {
	return __imalloc(size);
}

XSTDAPI void* XCALLAPI xcalloc(size_t nmemb, size_t size) {
	return __icalloc(nmemb,size);
}

XSTDAPI void* XCALLAPI xrealloc(void *ptr, size_t size) {
	return __irealloc(ptr,size);
}

XSTDAPI void XCALLAPI xfree(void *ptr) {
	__ifree(ptr);
}

XSTDAPI void XCALLAPI xfrees(void **ptrs, size_t n) {
	if (!ptrs) return;
	for (size_t i = 0; i < n; i++) __ifree(ptrs[i]);
	__ifree(ptrs);
}


XSTDAPI void* XCALLAPI xmemset(void *s, int c, size_t n) {
	return __imemset(s,c,n);
}

XSTDAPI void* XCALLAPI xmemcpy(void *xrestrict dest, const void *xrestrict src,size_t n) {
	return __imemcpy(dest,src,n);
}

XSTDAPI void* XCALLAPI xmemccpy(void *xrestrict dest, const void *xrestrict src,int c, size_t n) {
	return __imemccpy(dest,src,c,n);
}

XSTDAPI void* XCALLAPI xmemmove(void *dest, const void *src, size_t n) {
	return __imemmove(dest,src,n);
}

XSTDAPI int XCALLAPI xmemcmp(const void *s1,const void *s2,size_t n) {
	return __imemcmp(s1,s2,n);
}

XSTDAPI void* XCALLAPI xmempcpy(void *xrestrict dest,const void *xrestrict src, size_t n) {
#ifdef __APPLE__
	return (unsigned char*)__imemcpy(dest,src,n) + n;
#else
	return __imempcpy(dest,src,n);
#endif
}

XSTDAPI void* XCALLAPI xmemchr(const void* s,int c,size_t n) {
#ifdef __cplusplus
	return const_cast<void*>(__imemchr(s,c,n));
#else
	return __imemchr(s,c,n);
#endif
}


XSTDAPI wchar_t* XCALLAPI xwmemset(wchar_t *s, int c, size_t n) {
	return __iwmemset(s,c,n);
}

XSTDAPI wchar_t* XCALLAPI xwmemcpy(wchar_t *xrestrict dest, const wchar_t *xrestrict src,size_t n) {
	return __iwmemcpy(dest,src,n);
}

XSTDAPI wchar_t* XCALLAPI xwmemccpy(wchar_t *xrestrict dest,const wchar_t *xrestrict src,int c,size_t n) {
	wchar_t wc = (wchar_t)c;
	for (size_t i = 0; i < n; ++i) {
		dest[i] = src[i];
		if (src[i] == wc) return dest + i + 1;
	}
	return NULL;
}

XSTDAPI wchar_t* XCALLAPI xwmemmove(wchar_t *dest, const wchar_t *src, size_t n) {
	return __iwmemmove(dest,src,n);
}

XSTDAPI int XCALLAPI xwmemcmp(const wchar_t *s1,const wchar_t *s2,size_t n) {
	return __iwmemcmp(s1,s2,n);
}

XSTDAPI wchar_t* XCALLAPI xwmempcpy(wchar_t *xrestrict dest,const wchar_t *xrestrict src, size_t n) {
#ifdef __APPLE__
	return (unsigned char*)__iwmemcpy(dest,src,n) + n;
#else
	return __iwmempcpy(dest,src,n);
#endif
}

XSTDAPI wchar_t* XCALLAPI xwmemchr(const wchar_t* s,int c,size_t n) {
#ifdef __cplusplus
	return const_cast<wchar_t*>(__iwmemchr(s,c,n));
#else
	return __iwmemchr(s,c,n);
#endif
}

XSTDAPI int XCALLAPI xbcmp(const void *s1, const void *s2, size_t n) {
	return __imemcmp(s1,s2,n);
}

XSTDAPI void XCALLAPI xbcopy(const void *src, void *dest, size_t n) {
	__imemcpy(dest,src,n);
}

XSTDAPI void XCALLAPI xbzero(void *s, size_t n) {
	__imemset(s,0,n);
}

#ifdef __cplusplus
}
#endif

#endif // __XMEMORY_H__
