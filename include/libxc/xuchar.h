/**
 * xuchar.h: Unicode Character String Utilities
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

#ifndef __XUCHAR_H__
#define __XUCHAR_H__

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XSTD_USTRS(name,type,type1) \
XSTDMETHOD(size_t) name##len(const type* s); \
XSTDMETHOD(size_t) name##nlen(const type *string, size_t maxlen); \
XSTDMETHOD(type*) name##cpy(type *xrestrict dest, const type *xrestrict src); \
XSTDMETHOD(type*) name##ncpy(type *xrestrict dest,const type *xrestrict src,size_t n); \
XSTDMETHOD(type*) name##cat(type *xrestrict dest, const type *xrestrict src); \
XSTDMETHOD(type*) name##ncat(type *xrestrict dest,const type *xrestrict src,size_t n); \
XSTDMETHOD(int) name##cmp(const type *s1, const type *s2); \
XSTDMETHOD(int) name##ncmp(const type *s1, const type *s2, size_t n); \
XSTDMETHOD(int) name##coll(const type *s1, const type *s2); \
XSTDMETHOD(type*) name##dup(const type *s); \
XSTDMETHOD(type*) name##ndup(const type *s, size_t n); \
XSTDMETHOD(type*) name##chr(const type *s, type1 c); \
XSTDMETHOD(type*) name##chrnul(const type *s, type1 c); \
XSTDMETHOD(size_t) name##cspn(const type *s, const type *reject); \
XSTDMETHOD(size_t) name##spn(const type *s, const type *accept); \
XSTDMETHOD(type*) name##tok_r(type *xrestrict s,const type *xrestrict delim,type **xrestrict save_ptr); \
XSTDMETHOD(type*) name##tok(type *xrestrict s, const type *xrestrict delim); \
XSTDMETHOD(type*) name##pbrk(const type *s, const type *accept); \
XSTDMETHOD(type*) name##str(const type *haystack, const type *needle); \
XSTDMETHOD(type*) name##casestr(const type *haystack, const type *needle); \
XSTDMETHOD(int) name##casecmp(const type *s1, const type *s2); \
XSTDMETHOD(int) name##ncasecmp(const type *s1, const type *s2, size_t n); \
XSTDMETHOD(size_t) name##xfrm(type *xrestrict dest,const type *xrestrict src, size_t n); \
XSTDMETHOD(const type*) name##basename(const type* path); \
XSTDMETHOD(type*) name##cmbs(const type** ss, size_t n); \
XSTDMETHOD(int) name##cmps(const type * s1, const type ** s2, size_t n);

XSTD_USTRS(ustr8,char8_t,int)
XSTD_USTRS(ustr16,char16_t,int)
XSTD_USTRS(ustr32,char32_t,int)

#undef XSTD_USTRS

XSTDMETHOD(char*) xmbrtoc8_len(const char8_t *xrestrict pc8,size_t *out_size);

XSTDMETHOD(char*) xmbrtoc8(const char8_t *xrestrict pc8);

XSTDMETHOD(char8_t*) xc8rtomb_len(const char *xrestrict pc8,size_t *out_size);

XSTDMETHOD(char8_t*) xc8rtomb(const char *xrestrict pc8);


XSTDMETHOD(char*) xmbrtoc16_len(const char16_t *xrestrict pc8,size_t *out_size);

XSTDMETHOD(char*) xmbrtoc16(const char16_t *xrestrict pc8);

XSTDMETHOD(char16_t*) xc16rtomb_len(const char *xrestrict pc8,size_t *out_size);

XSTDMETHOD(char16_t*) xc16rtomb(const char *xrestrict pc8);


XSTDMETHOD(char*) xmbrtoc32_len(const char32_t *xrestrict pc8,size_t *out_size);

XSTDMETHOD(char*) xmbrtoc32(const char32_t *xrestrict pc8);

XSTDMETHOD(char32_t*) xc32rtomb_len(const char *xrestrict pc8,size_t *out_size);

XSTDMETHOD(char32_t*) xc32rtomb(const char *xrestrict pc8);

#ifdef __cplusplus
}
#endif

#endif