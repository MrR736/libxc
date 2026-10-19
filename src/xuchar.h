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
#include "xmemory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XSTD_USTRS(name,type,type1,tcnull) \
XSTDAPI size_t XCALLAPI name##len(const type* s) { \
	if (s == NULL) return 0; \
	const type *p = s; \
	while (*p != tcnull) ++p; \
	return (size_t)(p - s); \
} \
XSTDAPI size_t XCALLAPI name##nlen(const type *string, size_t maxlen) { \
	if (string == NULL) return 0; \
	size_t len = 0; \
	while (len < maxlen && string[len] != tcnull) ++len; \
	return len; \
} \
XSTDAPI type* XCALLAPI name##cpy(type *xrestrict dest, const type *xrestrict src) { \
	type *result = dest; \
	while ((*dest++ = *src++) != tcnull); \
	return result; \
} \
XSTDAPI type* XCALLAPI name##ncpy(type *xrestrict dest,const type *xrestrict src,size_t n) { \
	type *result = dest; \
	size_t i = 0; \
	for (; i < n && src[i] != tcnull; ++i) dest[i] = src[i]; \
	for (; i < n; ++i) dest[i] = tcnull; \
	return result; \
} \
XSTDAPI type* XCALLAPI name##cat(type *xrestrict dest, const type *xrestrict src) { \
	type *result = dest; \
	while (*dest != tcnull) ++dest; \
	while ((*dest++ = *src++) != tcnull); \
	return result; \
} \
XSTDAPI type* XCALLAPI name##ncat(type *xrestrict dest,const type *xrestrict src,size_t n) { \
	type *result = dest; \
	while (*dest != tcnull) ++dest; \
	while (n > 0 && *src != tcnull) { *dest++ = *src++; --n; } \
	*dest = tcnull; \
	return result; \
} \
XSTDAPI int XCALLAPI name##cmp(const type *s1, const type *s2) { \
	while (*s1 == *s2 && *s1 != tcnull) { ++s1; ++s2; } \
	return (*s1 > *s2) - (*s1 < *s2); \
} \
XSTDAPI int XCALLAPI name##ncmp(const type *s1, const type *s2, size_t n) { \
	while (n > 0 && *s1 == *s2 && *s1 != tcnull) { ++s1; ++s2; --n; } \
	if (n == 0) return 0; \
	return (*s1 > *s2) - (*s1 < *s2); \
} \
XSTDAPI int XCALLAPI name##coll(const type *s1, const type *s2) { \
	return name##cmp(s1, s2); \
} \
XSTDAPI size_t XCALLAPI name##xfrm(type *xrestrict dest,const type *xrestrict src, size_t n) { \
	if (src == NULL) return 0; \
	size_t len = name##len(src); \
	if (n > 0) { \
		size_t copy = (len < n - 1) ? len : n - 1; \
		for (size_t i = 0; i < copy; ++i) dest[i] = src[i]; \
		dest[copy] = tcnull; \
	} \
	return len; \
} \
XSTDAPI type* XCALLAPI name##dup(const type *s) { \
	if (s == NULL) return NULL; \
	size_t len = name##len(s); \
	if (len >= SIZE_MAX / sizeof(type)) return NULL; \
	type *result = (type*)xmalloc((len + 1) * sizeof(type)); \
	if (result == NULL) return NULL; \
	for (size_t i = 0; i <= len; ++i) result[i] = s[i]; \
	return result; \
} \
XSTDAPI type* XCALLAPI name##ndup(const type *s, size_t n) { \
	if (s == NULL) return NULL; \
	size_t len = 0; \
	while (len < n && s[len] != tcnull) ++len; \
	if (len >= SIZE_MAX / sizeof(type)) return NULL; \
	type *result = (type*)xmalloc((len + 1) * sizeof(type)); \
	if (result == NULL) return NULL; \
	for (size_t i = 0; i < len; ++i) result[i] = s[i]; \
	result[len] = tcnull; \
	return result; \
} \
XSTDAPI type* XCALLAPI name##chr(const type *s, type1 c) { \
	if (s == NULL) return NULL; \
	type ch = (type)c; \
	for (;;) { \
		if (*s == ch) return (type*)s; \
		if (*s == tcnull) return NULL; \
		++s; \
	} \
} \
XSTDAPI type* XCALLAPI name##chrnul(const type *s, type1 c) { \
	if (s == NULL) return NULL; \
	type ch = (type)c; \
	while (*s != ch && *s != tcnull) ++s; \
	return (type*)s; \
} \
XSTDAPI size_t XCALLAPI name##cspn(const type *s, const type *reject) { \
	if (s == NULL || reject == NULL) return 0; \
	const type *p = s; \
	while (*p != tcnull) { \
		const type *r = reject; \
		while (*r != tcnull && *r != *p) ++r; \
		if (*r != tcnull) break; \
		++p; \
	} \
	return (size_t)(p - s); \
} \
XSTDAPI size_t XCALLAPI name##spn(const type *s, const type *accept) { \
	if (s == NULL || accept == NULL) return 0; \
	const type *p = s; \
	while (*p != tcnull) { \
		const type *a = accept; \
		while (*a != tcnull && *a != *p) ++a; \
		if (*a == tcnull) break; \
		++p; \
	} \
	return (size_t)(p - s); \
} \
XSTDAPI type* XCALLAPI name##tok_r(type *xrestrict s,const type *xrestrict delim,type **xrestrict save_ptr) { \
	if (delim == NULL || save_ptr == NULL) return NULL; \
	if (s == NULL) s = *save_ptr; \
	if (s == NULL) return NULL; \
	while (*s != tcnull) { \
		const type *d = delim; \
		while (*d != tcnull && *d != *s) ++d; \
		if (*d == tcnull) break; \
		++s; \
	} \
	if (*s == tcnull) { \
		*save_ptr = NULL; \
		return NULL; \
	} \
	type *token = s; \
	while (*s != tcnull) { \
		const type *d = delim; \
		while (*d != tcnull && *d != *s) ++d; \
		if (*d != tcnull) { \
			*s = tcnull; \
			*save_ptr = s + 1; \
			return token; \
		} \
		++s; \
	} \
	*save_ptr = NULL; \
	return token; \
} \
XSTDAPI type* XCALLAPI name##tok(type *xrestrict s, const type *xrestrict delim) { \
	static type *save_ptr; \
	return name##tok_r(s, delim, &save_ptr); \
} \
XSTDAPI type* XCALLAPI name##pbrk(const type *s, const type *accept) { \
	if (s == NULL || accept == NULL) return NULL; \
	for (; *s != tcnull; ++s) { \
		const type *a = accept; \
		while (*a != tcnull && *a != *s) ++a; \
		if (*a != tcnull) return (type *)s; \
	} \
	return NULL; \
} \
XSTDAPI type* XCALLAPI name##str(const type *haystack, const type *needle) { \
	if (haystack == NULL || needle == NULL) return NULL; \
	if (*needle == tcnull) return (type*)haystack; \
	for (const type *h = haystack; *h != tcnull; ++h) { \
		const type *p = h; \
		const type *n = needle; \
		while (*p != tcnull && *n != tcnull && *p == *n) { ++p; ++n; } \
		if (*n == tcnull) return (type*)h; \
	} \
	return NULL; \
} \
XSTDAPI type* XCALLAPI name##casestr(const type *haystack, const type *needle) { \
	if (haystack == NULL || needle == NULL) return NULL; \
	if (*needle == tcnull) return (type*)haystack; \
	for (const type *h = haystack; *h != tcnull; ++h) { \
		const type *p = h; \
		const type *n = needle; \
		while (*p != tcnull && *n != tcnull && towlower((wint_t)*p) == towlower((wint_t)*n)) { ++p; ++n; } \
		if (*n == tcnull) return (type*)h; \
	} \
	return NULL; \
} \
XSTDAPI int XCALLAPI name##casecmp(const type *s1, const type *s2) { \
	if (s1 == NULL || s2 == NULL) return (s1 != NULL) - (s2 != NULL); \
	while (*s1 != tcnull && *s2 != tcnull) { \
		wint_t c1 = towlower((wint_t)*s1); \
		wint_t c2 = towlower((wint_t)*s2); \
		if (c1 != c2) return (c1 > c2) - (c1 < c2); \
		++s1; ++s2; \
	} \
	return (*s1 != tcnull) - (*s2 != tcnull); \
} \
XSTDAPI int XCALLAPI name##ncasecmp(const type *s1, const type *s2, size_t n) { \
	if (n == 0) return 0; \
	if (s1 == NULL || s2 == NULL) return (s1 != NULL) - (s2 != NULL); \
	while (n > 0 && *s1 != tcnull && *s2 != tcnull) { \
		wint_t c1 = towlower((wint_t)*s1); \
		wint_t c2 = towlower((wint_t)*s2); \
		if (c1 != c2) return (c1 > c2) - (c1 < c2); \
		++s1; ++s2; --n; \
	} \
	if (n == 0) return 0; \
	return (*s1 != tcnull) - (*s2 != tcnull); \
}

XSTD_USTRS(ustr8,char8_t,int,0)
XSTD_USTRS(ustr16,char16_t,int,u'\0')
XSTD_USTRS(ustr32,char32_t,int,U'\0')

#undef XSTD_USTRS

XSTDAPI char* XCALLAPI xmbrtoc8_len(const char8_t *xrestrict pc8,size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!pc8) {
		errno = EINVAL;
		return NULL;
	}

	size_t len = ustr8len(pc8);
	char *mbs = (char *)xmalloc(len + 1);
	if (!mbs) {
		errno = ENOMEM;
		return NULL;
	}
	for (size_t i = 0; i < len; ++i) mbs[i] = (char)pc8[i];
	mbs[len] = '\0';
	if (out_size) *out_size = len;
	return mbs;
}

XSTDAPI char* XCALLAPI xmbrtoc8(const char8_t *xrestrict pc8) {
	return xmbrtoc8_len(pc8,NULL);
}

XSTDAPI char8_t* XCALLAPI xc8rtomb_len(const char *xrestrict pc8, size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!pc8) {
		errno = EINVAL;
		return NULL;
	}

	const unsigned char *src = (const unsigned char *)pc8;
	size_t input_len = strlen(pc8);

	if (input_len > (SIZE_MAX - 1) / 4) {
		errno = EOVERFLOW;
		return NULL;
	}

	char8_t *utf8 = (char8_t *)xmalloc(input_len * 4 + 1);
	if (!utf8) {
		errno = ENOMEM;
		return NULL;
	}

	size_t in_pos = 0;
	size_t out_pos = 0;

	while (in_pos < input_len) {
		uint32_t cp;
		unsigned char c = src[in_pos++];
		size_t remaining = input_len - in_pos;

		if (c <= 0x7F) {
			cp = c;
		} else if (c >= 0xC2 && c <= 0xDF) {
			if (remaining < 1) goto invalid_utf8;
			unsigned char c1 = src[in_pos];
			if ((c1 & 0xC0) != 0x80) goto invalid_utf8;
			cp = ((uint32_t)(c & 0x1F) << 6) | (uint32_t)(c1 & 0x3F);
			in_pos += 1;
		} else if (c >= 0xE0 && c <= 0xEF) {
			if (remaining < 2) goto invalid_utf8;
			unsigned char c1 = src[in_pos];
			unsigned char c2 = src[in_pos + 1];
			if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80) goto invalid_utf8;

			/* Reject overlong encodings and UTF-16 surrogates. */
			if ((c == 0xE0 && c1 < 0xA0) || (c == 0xED && c1 >= 0xA0)) goto invalid_utf8;
			cp = ((uint32_t)(c & 0x0F) << 12) | ((uint32_t)(c1 & 0x3F) << 6) | (uint32_t)(c2 & 0x3F);
			in_pos += 2;
		} else if (c >= 0xF0 && c <= 0xF4) {
			if (remaining < 3) goto invalid_utf8;
			unsigned char c1 = src[in_pos];
			unsigned char c2 = src[in_pos + 1];
			unsigned char c3 = src[in_pos + 2];

			if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) goto invalid_utf8;

			/* Reject overlong encodings and values > U+10FFFF. */
			if ((c == 0xF0 && c1 < 0x90) || (c == 0xF4 && c1 > 0x8F)) goto invalid_utf8;
			cp = ((uint32_t)(c & 0x07) << 18) | ((uint32_t)(c1 & 0x3F) << 12) | ((uint32_t)(c2 & 0x3F) << 6) | (uint32_t)(c3 & 0x3F);
			in_pos += 3;
		} else {
			/* Reject continuation bytes, C0/C1, and F5-FF. */
			goto invalid_utf8;
		}

		/*
		 * The input is already valid UTF-8.
		 * Re-encode the decoded scalar value.
		 */
		if (cp <= 0x7F) {
			utf8[out_pos++] = (char8_t)cp;
		} else if (cp <= 0x7FF) {
			utf8[out_pos++] = (char8_t)(0xC0 | (cp >> 6));
			utf8[out_pos++] = (char8_t)(0x80 | (cp & 0x3F));
		} else if (cp <= 0xFFFF) {
			utf8[out_pos++] = (char8_t)(0xE0 | (cp >> 12));
			utf8[out_pos++] = (char8_t)(0x80 | ((cp >> 6) & 0x3F));
			utf8[out_pos++] = (char8_t)(0x80 | (cp & 0x3F));
		} else {
			utf8[out_pos++] = (char8_t)(0xF0 | (cp >> 18));
			utf8[out_pos++] = (char8_t)(0x80 | ((cp >> 12) & 0x3F));
			utf8[out_pos++] = (char8_t)(0x80 | ((cp >> 6) & 0x3F));
			utf8[out_pos++] = (char8_t)(0x80 | (cp & 0x3F));
		}
	}
	utf8[out_pos] = (char8_t)0;
	if (out_size) *out_size = out_pos;
	return utf8;

invalid_utf8:
	xfree(utf8);
	errno = EILSEQ;
	return NULL;
}

XSTDAPI char8_t* XCALLAPI xc8rtomb(const char *xrestrict pc8) {
	return xc8rtomb_len(pc8, NULL);
}


XSTDAPI char* XCALLAPI xmbrtoc16_len(const char16_t *xrestrict pc8,size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!pc8) {
		errno = EINVAL;
		return NULL;
	}

	size_t units = 0;
	size_t bytes = 0;

	while (pc8[units] != 0) {
		uint32_t cp = pc8[units++];
		if (cp >= 0xD800 && cp <= 0xDBFF) {
			uint32_t lo = pc8[units];
			if (lo < 0xDC00 || lo > 0xDFFF) {
				errno = EILSEQ;
				return NULL;
			}
			++units;
			cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
		} else if (cp >= 0xDC00 && cp <= 0xDFFF) {
			errno = EILSEQ;
			return NULL;
		}

		if (cp <= 0x7F) bytes += 1;
		else if (cp <= 0x7FF) bytes += 2;
		else if (cp <= 0xFFFF) bytes += 3;
		else bytes += 4;

		if (bytes > SIZE_MAX - 1) {
			errno = EOVERFLOW;
			return NULL;
		}
	}

	char *out = (char *)xmalloc(bytes + 1);
	if (!out) {
		errno = ENOMEM;
		return NULL;
	}

	size_t i = 0, j = 0;

	while (pc8[i] != 0) {
		uint32_t cp = pc8[i++];
		if (cp >= 0xD800 && cp <= 0xDBFF) {
			uint32_t lo = pc8[i++];
			cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
		}

		if (cp <= 0x7F) {
			out[j++] = (char)cp;
		} else if (cp <= 0x7FF) {
			out[j++] = (char)(0xC0 | (cp >> 6));
			out[j++] = (char)(0x80 | (cp & 0x3F));
		} else if (cp <= 0xFFFF) {
			out[j++] = (char)(0xE0 | (cp >> 12));
			out[j++] = (char)(0x80 | ((cp >> 6) & 0x3F));
			out[j++] = (char)(0x80 | (cp & 0x3F));
		} else {
			out[j++] = (char)(0xF0 | (cp >> 18));
			out[j++] = (char)(0x80 | ((cp >> 12) & 0x3F));
			out[j++] = (char)(0x80 | ((cp >> 6) & 0x3F));
			out[j++] = (char)(0x80 | (cp & 0x3F));
		}
	}

	out[j] = '\0';
	if (out_size) *out_size = j;
	return out;
}

XSTDAPI char* XCALLAPI xmbrtoc16(const char16_t *xrestrict pc8) {
	return xmbrtoc16_len(pc8, NULL);
}

XSTDAPI char16_t* XCALLAPI xc16rtomb_len(const char *xrestrict pc8,size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!pc8) {
		errno = EINVAL;
		return NULL;
	}

	size_t n = strlen(pc8);
	if (n > (SIZE_MAX / sizeof(char16_t)) - 1) {
		errno = EOVERFLOW;
		return NULL;
	}

	char16_t *out = (char16_t *)xmalloc((n + 1) * sizeof(char16_t));
	if (!out) {
		errno = ENOMEM;
		return NULL;
	}
	size_t i = 0, j = 0;
	while (i < n) {
		uint32_t cp;
		unsigned char c = (unsigned char)pc8[i++];
		if (c <= 0x7F) { cp = c; }
		else if (c >= 0xC2 && c <= 0xDF) {
			if (i >= n) goto invalid;
			unsigned char c1 = (unsigned char)pc8[i++];
			if ((c1 & 0xC0) != 0x80) goto invalid;
			cp = ((c & 0x1F) << 6) | (c1 & 0x3F);
		} else if (c >= 0xE0 && c <= 0xEF) {
			if (i + 1 >= n) goto invalid;
			unsigned char c1 = (unsigned char)pc8[i++];
			unsigned char c2 = (unsigned char)pc8[i++];
			if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80) goto invalid;
			if ((c == 0xE0 && c1 < 0xA0) || (c == 0xED && c1 >= 0xA0)) goto invalid;
			cp = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
		} else if (c >= 0xF0 && c <= 0xF4) {
			if (i + 2 >= n) goto invalid;
			unsigned char c1 = (unsigned char)pc8[i++];
			unsigned char c2 = (unsigned char)pc8[i++];
			unsigned char c3 = (unsigned char)pc8[i++];
			if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) goto invalid;
			if ((c == 0xF0 && c1 < 0x90) || (c == 0xF4 && c1 > 0x8F)) goto invalid;
			cp = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
		} else { goto invalid; }

		if (cp <= 0xFFFF) {
			out[j++] = (char16_t)cp;
		} else {
			cp -= 0x10000;
			out[j++] = (char16_t)(0xD800 + (cp >> 10));
			out[j++] = (char16_t)(0xDC00 + (cp & 0x3FF));
		}
	}

	out[j] = u'\0';

	if (out_size) *out_size = j; /* UTF-16 code units, including NUL */

	return out;

invalid:
	xfree(out);
	errno = EILSEQ;
	return NULL;
}

XSTDAPI char16_t* XCALLAPI xc16rtomb(const char *xrestrict pc8) {
	return xc16rtomb_len(pc8, NULL);
}



XSTDAPI char* XCALLAPI xmbrtoc32_len(const char32_t *xrestrict pc8,size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!pc8) {
		errno = EINVAL;
		return NULL;
	}

	size_t bytes = 0;

	for (size_t i = 0; pc8[i] != 0; ++i) {
		uint32_t cp = (uint32_t)pc8[i];
		if (cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) {
			errno = EILSEQ;
			return NULL;
		}

		size_t add = cp <= 0x7F ? 1 : cp <= 0x7FF ? 2 : cp <= 0xFFFF ? 3 : 4;
		if (bytes > SIZE_MAX - add - 1) {
			errno = EOVERFLOW;
			return NULL;
		}
		bytes += add;
	}

	char *out = (char *)xmalloc(bytes + 1);
	if (!out) {
		errno = ENOMEM;
		return NULL;
	}

	size_t j = 0;
	for (size_t i = 0; pc8[i] != 0; ++i) {
		uint32_t cp = (uint32_t)pc8[i];
		if (cp <= 0x7F) {
			out[j++] = (char)cp;
		} else if (cp <= 0x7FF) {
			out[j++] = (char)(0xC0 | (cp >> 6));
			out[j++] = (char)(0x80 | (cp & 0x3F));
		} else if (cp <= 0xFFFF) {
			out[j++] = (char)(0xE0 | (cp >> 12));
			out[j++] = (char)(0x80 | ((cp >> 6) & 0x3F));
			out[j++] = (char)(0x80 | (cp & 0x3F));
		} else {
			out[j++] = (char)(0xF0 | (cp >> 18));
			out[j++] = (char)(0x80 | ((cp >> 12) & 0x3F));
			out[j++] = (char)(0x80 | ((cp >> 6) & 0x3F));
			out[j++] = (char)(0x80 | (cp & 0x3F));
		}
	}
	out[j] = '\0';
	if (out_size) *out_size = j;
	return out;
}

XSTDAPI char* XCALLAPI xmbrtoc32(const char32_t *xrestrict pc8) {
	return xmbrtoc32_len(pc8, NULL);
}

XSTDAPI char32_t* XCALLAPI xc32rtomb_len(const char *xrestrict pc8,size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!pc8) {
		errno = EINVAL;
		return NULL;
	}

	size_t n = strlen(pc8);
	if (n > SIZE_MAX / sizeof(char32_t) - 1) {
		errno = EOVERFLOW;
		return NULL;
	}

	char32_t *out = (char32_t*)xmalloc((n + 1) * sizeof(char32_t));
	if (!out) {
		errno = ENOMEM;
		return NULL;
	}

	size_t i = 0, j = 0;
	while (i < n) {
		uint32_t cp;
		unsigned char c = (unsigned char)pc8[i++];
		if (c <= 0x7F) {
			cp = c;
		} else if (c >= 0xC2 && c <= 0xDF) {
			if (i >= n) goto invalid;
			unsigned char c1 = (unsigned char)pc8[i++];
			if ((c1 & 0xC0) != 0x80) goto invalid;
			cp = ((uint32_t)(c & 0x1F) << 6) | (uint32_t)(c1 & 0x3F);
		} else if (c >= 0xE0 && c <= 0xEF) {
			if (i + 1 >= n) goto invalid;
			unsigned char c1 = (unsigned char)pc8[i++];
			unsigned char c2 = (unsigned char)pc8[i++];
			if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 ||
				(c == 0xE0 && c1 < 0xA0) || (c == 0xED && c1 >= 0xA0)) goto invalid;
			cp = ((uint32_t)(c & 0x0F) << 12) | ((uint32_t)(c1 & 0x3F) << 6) | (uint32_t)(c2 & 0x3F);
		} else if (c >= 0xF0 && c <= 0xF4) {
			if (i + 2 >= n) goto invalid;
			unsigned char c1 = (unsigned char)pc8[i++];
			unsigned char c2 = (unsigned char)pc8[i++];
			unsigned char c3 = (unsigned char)pc8[i++];
			if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80 ||
				(c == 0xF0 && c1 < 0x90) || (c == 0xF4 && c1 > 0x8F)) goto invalid;
			cp = ((uint32_t)(c & 0x07) << 18) | ((uint32_t)(c1 & 0x3F) << 12) |
				 ((uint32_t)(c2 & 0x3F) << 6) | (uint32_t)(c3 & 0x3F);
		} else { goto invalid; }
		out[j++] = (char32_t)cp;
	}
	out[j] = U'\0';
	if (out_size) *out_size = j;
	return out;

invalid:
	xfree(out);
	errno = EILSEQ;
	return NULL;
}

XSTDAPI char32_t* XCALLAPI xc32rtomb(const char *xrestrict pc8) {
	return xc32rtomb_len(pc8, NULL);
}

#ifdef __cplusplus
}
#endif

#endif