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
XSTDAPI size_t XCALLAPI name##len(const type* s); \
XSTDAPI size_t XCALLAPI name##nlen(const type *string, size_t maxlen); \
XSTDAPI type* XCALLAPI name##cpy(type *xrestrict dest, const type *xrestrict src); \
XSTDAPI type* XCALLAPI name##ncpy(type *xrestrict dest,const type *xrestrict src,size_t n); \
XSTDAPI type* XCALLAPI name##cat(type *xrestrict dest, const type *xrestrict src); \
XSTDAPI type* XCALLAPI name##ncat(type *xrestrict dest,const type *xrestrict src,size_t n); \
XSTDAPI int XCALLAPI name##cmp(const type *s1, const type *s2); \
XSTDAPI int XCALLAPI name##ncmp(const type *s1, const type *s2, size_t n); \
XSTDAPI int XCALLAPI name##coll(const type *s1, const type *s2); \
XSTDAPI type* XCALLAPI name##dup(const type *s); \
XSTDAPI type* XCALLAPI name##ndup(const type *s, size_t n); \
XSTDAPI type* XCALLAPI name##chr(const type *s, type1 c); \
XSTDAPI type* XCALLAPI name##chrnul(const type *s, type1 c); \
XSTDAPI size_t XCALLAPI name##cspn(const type *s, const type *reject); \
XSTDAPI size_t XCALLAPI name##spn(const type *s, const type *accept); \
XSTDAPI type* XCALLAPI name##tok_r(type *xrestrict s,const type *xrestrict delim,type **xrestrict save_ptr); \
XSTDAPI type* XCALLAPI name##tok(type *xrestrict s, const type *xrestrict delim); \
XSTDAPI type* XCALLAPI name##pbrk(const type *s, const type *accept); \
XSTDAPI type* XCALLAPI name##str(const type *haystack, const type *needle); \
XSTDAPI type* XCALLAPI name##casestr(const type *haystack, const type *needle); \
XSTDAPI int XCALLAPI name##casecmp(const type *s1, const type *s2); \
XSTDAPI int XCALLAPI name##ncasecmp(const type *s1, const type *s2, size_t n); \
XSTDAPI size_t XCALLAPI name##xfrm(type *xrestrict dest,const type *xrestrict src, size_t n);

XSTD_USTRS(ustr8,char8_t,int)
XSTD_USTRS(ustr16,char16_t,int)
XSTD_USTRS(ustr32,char32_t,int)

#undef XSTD_USTRS

XSTDAPI char* XCALLAPI xmbrtoc8_len(const char8_t *xrestrict pc8,size_t *out_size);

XSTDAPI char* XCALLAPI xmbrtoc8(const char8_t *xrestrict pc8);

XSTDAPI char8_t* XCALLAPI xc8rtomb_len(const char *xrestrict pc8,size_t *out_size);

XSTDAPI char8_t* XCALLAPI xc8rtomb(const char *xrestrict pc8);


XSTDAPI char* XCALLAPI xmbrtoc16_len(const char16_t *xrestrict pc8,size_t *out_size);

XSTDAPI char* XCALLAPI xmbrtoc16(const char16_t *xrestrict pc8);

XSTDAPI char16_t* XCALLAPI xc16rtomb_len(const char *xrestrict pc8,size_t *out_size);

XSTDAPI char16_t* XCALLAPI xc16rtomb(const char *xrestrict pc8);


XSTDAPI char* XCALLAPI xmbrtoc32_len(const char32_t *xrestrict pc8,size_t *out_size);

XSTDAPI char* XCALLAPI xmbrtoc32(const char32_t *xrestrict pc8);

XSTDAPI char32_t* XCALLAPI xc32rtomb_len(const char *xrestrict pc8,size_t *out_size);

XSTDAPI char32_t* XCALLAPI xc32rtomb(const char *xrestrict pc8);

#ifdef __cplusplus
}
#endif

#endif