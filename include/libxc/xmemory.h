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

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI void* XCALLAPI xmalloc(size_t size);

XSTDAPI void* XCALLAPI xcalloc(size_t nmemb, size_t size);

XSTDAPI void* XCALLAPI xrealloc(void *ptr, size_t size);

XSTDAPI void XCALLAPI xfree(void *ptr);

XSTDAPI void XCALLAPI xfrees(void **ptrs, size_t n);


XSTDAPI void* XCALLAPI xmemset(void *s, int c, size_t n);

XSTDAPI void* XCALLAPI xmemcpy(void *xrestrict dest, const void *xrestrict src,size_t n);

XSTDAPI void* XCALLAPI xmemccpy(void *xrestrict dest, const void *xrestrict src,int c, size_t n);

XSTDAPI void* XCALLAPI xmemmove(void *dest, const void *src, size_t n);

XSTDAPI int XCALLAPI xmemcmp(const void *s1,const void *s2,size_t n);

XSTDAPI void* XCALLAPI xmempcpy(void *xrestrict dest,const void *xrestrict src, size_t n);

XSTDAPI void* XCALLAPI xmemchr(const void* s,int c,size_t n);


XSTDAPI wchar_t* XCALLAPI xwmemset(wchar_t *s, int c, size_t n);

XSTDAPI wchar_t* XCALLAPI xwmemcpy(wchar_t *xrestrict dest, const wchar_t *xrestrict src,size_t n);

XSTDAPI wchar_t* XCALLAPI xwmemccpy(wchar_t *xrestrict dest,const wchar_t *xrestrict src,int c,size_t n);

XSTDAPI wchar_t* XCALLAPI xwmemmove(wchar_t *dest, const wchar_t *src, size_t n);

XSTDAPI int XCALLAPI xwmemcmp(const wchar_t *s1,const wchar_t *s2,size_t n);

XSTDAPI wchar_t* XCALLAPI xwmempcpy(wchar_t *xrestrict dest,const wchar_t *xrestrict src, size_t n);

XSTDAPI wchar_t* XCALLAPI xwmemchr(const wchar_t* s,int c,size_t n);


XSTDAPI int XCALLAPI xbcmp(const void *s1, const void *s2, size_t n);

XSTDAPI void XCALLAPI xbcopy(const void *src, void *dest, size_t n);

XSTDAPI void XCALLAPI xbzero(void *s, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __XSTRING_H__
