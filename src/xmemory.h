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

XSTDMETHOD(void*) xmalloc(size_t size) {
	return __imalloc(size);
}

XSTDMETHOD(void*) xcalloc(size_t nmemb, size_t size) {
	return __icalloc(nmemb,size);
}

XSTDMETHOD(void*) xrealloc(void *ptr, size_t size) {
	return __irealloc(ptr,size);
}

XSTDMETHOD(void) xfree(void *ptr) {
	__ifree(ptr);
}

XSTDMETHOD(void) xfrees(void **ptrs, size_t n) {
	if (!ptrs) return;
	for (size_t i = 0; i < n; i++) __ifree(ptrs[i]);
	__ifree(ptrs);
}


XSTDMETHOD(void*) xmemset(void *s, int c, size_t n) {
	return __imemset(s,c,n);
}

XSTDMETHOD(void*) xmemcpy(void *xrestrict dest, const void *xrestrict src,size_t n) {
	return __imemcpy(dest,src,n);
}

XSTDMETHOD(void*) xmemccpy(void *xrestrict dest, const void *xrestrict src,int c, size_t n) {
	return __imemccpy(dest,src,c,n);
}

XSTDMETHOD(void*) xmemmove(void *dest, const void *src, size_t n) {
	return __imemmove(dest,src,n);
}

XSTDMETHOD(int) xmemcmp(const void *s1,const void *s2,size_t n) {
	return __imemcmp(s1,s2,n);
}

XSTDMETHOD(void*) xmempcpy(void *xrestrict dest,const void *xrestrict src, size_t n) {
#ifdef __APPLE__
	return (unsigned char*)__imemcpy(dest,src,n) + n;
#else
	return __imempcpy(dest,src,n);
#endif
}

XSTDMETHOD(void*) xmemchr(const void* s,int c,size_t n) {
#ifdef __cplusplus
	return const_cast<void*>(__imemchr(s,c,n));
#else
	return __imemchr(s,c,n);
#endif
}


XSTDMETHOD(wchar_t*) xwmemset(wchar_t *s, int c, size_t n) {
	return __iwmemset(s,c,n);
}

XSTDMETHOD(wchar_t*) xwmemcpy(wchar_t *xrestrict dest, const wchar_t *xrestrict src,size_t n) {
	return __iwmemcpy(dest,src,n);
}

XSTDMETHOD(wchar_t*) xwmemccpy(wchar_t *xrestrict dest,const wchar_t *xrestrict src,int c,size_t n) {
	wchar_t wc = (wchar_t)c;
	for (size_t i = 0; i < n; ++i) {
		dest[i] = src[i];
		if (src[i] == wc) return dest + i + 1;
	}
	return NULL;
}

XSTDMETHOD(wchar_t*) xwmemmove(wchar_t *dest, const wchar_t *src, size_t n) {
	return __iwmemmove(dest,src,n);
}

XSTDMETHOD(int) xwmemcmp(const wchar_t *s1,const wchar_t *s2,size_t n) {
	return __iwmemcmp(s1,s2,n);
}

XSTDMETHOD(wchar_t*) xwmempcpy(wchar_t *xrestrict dest,const wchar_t *xrestrict src, size_t n) {
#ifdef __APPLE__
	return __iwmemcpy(dest,src,n) + n;
#else
	return __iwmempcpy(dest,src,n);
#endif
}

XSTDMETHOD(wchar_t*) xwmemchr(const wchar_t* s,int c,size_t n) {
#ifdef __cplusplus
	return const_cast<wchar_t*>(__iwmemchr(s,c,n));
#else
	return __iwmemchr(s,c,n);
#endif
}

XSTDMETHOD(int) xbcmp(const void *s1, const void *s2, size_t n) {
	return __imemcmp(s1,s2,n);
}

XSTDMETHOD(void) xbcopy(const void *src, void *dest, size_t n) {
	__imemcpy(dest,src,n);
}

XSTDMETHOD(void) xbzero(void *s, size_t n) {
	__imemset(s,0,n);
}

#ifdef __cplusplus
}
#endif

#endif // __XMEMORY_H__
