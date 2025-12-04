/**
 * xstdio.h: Extern Library
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

#ifndef __EXTSTDIO_H__
#define __EXTSTDIO_H__

#include "xstdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_INLINE_API int fdputs(const char *__restrict __s, int __fd) {
	if (!__s) return -1;
	size_t len   = strlen(__s);
	size_t total = 0;
	while (total < len) {
		ssize_t n = write(__fd, __s + total, len - total);
		if (n <= 0) return -1;  // write failed
		total += (size_t)n;
	}
	return 0;
}

XSTDDEF_INLINE_API FILE* fdno(int __fd) {
	if (__fd < 0) return NULL;
	return fdopen(dup(__fd), "r+");
}

XSTDDEF_INLINE_API FILE* fdno_unlocked(int __fd) {
	if (__fd < 0) return NULL;
	FILE *fp = fdno(__fd);
	if (!fp) return NULL;
#if defined(_WIN32) || defined(_WIN64)
	_lock_file(fp);
#else
	flockfile(fp);
#endif
	return fp;
}

// full read
XSTDDEF_INLINE_API void *furead(FILE *fp, size_t *out_size) {
	if (!fp) {
		errno = EINVAL;
		return NULL;
	}

	long original_pos = ftell(fp);
	if (original_pos < 0)
		return NULL; // non-seekable stream

	if (fseek(fp, 0, SEEK_END) != 0)
		return NULL;

	long end = ftell(fp);
	if (fseek(fp, original_pos, SEEK_SET) != 0)
		return NULL;

	if (end < 0 || end < original_pos) {
		errno = EIO;
		return NULL;
	}

	size_t size = (size_t)(end - original_pos);

	if (size == SIZE_MAX) {
		errno = EOVERFLOW;
		return NULL;
	}

	void *data = malloc(size + 1);
	if (!data)
		return NULL;

	size_t read = fread(data, 1, size, fp);
	((char *)data)[read] = '\0';

	if (out_size)
		*out_size = read;

	fseek(fp, original_pos, SEEK_SET);
	return data;
}

XSTDDEF_INLINE_API void *fduread(int __fd, size_t *out_size) {
	const size_t initial_cap = 4096;
	size_t cap = initial_cap;
	size_t len = 0;

	if (out_size)
		*out_size = 0;

	unsigned char *buf = (unsigned char *)malloc(cap);
	if (!buf)
		return NULL;

	for (;;) {
		// grow if needed
		if (len == cap) {
			size_t new_cap = cap * 2;
			unsigned char *new_buf = (unsigned char *)realloc(buf, new_cap);
			if (!new_buf) {
				free(buf);
				return NULL;  // realloc failed
			}
			buf = new_buf;
			cap = new_cap;
		}

		// read chunk
		ssize_t n = read(__fd, buf + len, cap - len);
		if (n < 0) {
			if (errno == EINTR)
				continue;  // retry
			free(buf);
			return NULL;  // real error
		}
		if (n == 0)
			break;
		len += (size_t)n;
	}

	// final shrink (optional)
	if (len != cap) {
		unsigned char *new_buf = (unsigned char *)realloc(buf, len);
		if (new_buf) buf = new_buf;  // ignore shrink failure
	}

	// Uncomment if you want NUL-termination to match furead():
	// {
	//	 unsigned char *new_buf = realloc(buf, len + 1);
	//	 if (!new_buf) { free(buf); return NULL; }
	//	 buf = new_buf;
	//	 buf[len] = '\0';
	// }

	if (out_size)
		*out_size = len;
	return buf;
}

XSTDDEF_INLINE_API char* vcprintf(const char *__restrict fmt, va_list ap) {
	if (!fmt) return NULL;
	va_list apc, apf;
	va_copy(apc, ap);
	size_t len = vxstrlen(fmt, apc);
	va_end(apc);
	if (len == (size_t)-1) return NULL;
	char *s = (char*)malloc(len + 1);
	if (!s) return NULL;
	va_copy(apf, ap);
	int written = vsnprintf(s, len + 1, fmt, apf);
	va_end(apf);
	if (written < 0) {
		free(s);
		return NULL;
	}
	return s;
}

XSTDDEF_INLINE_API char* cprintf(const char *__restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char *s = vcprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDDEF_INLINE_API wchar_t* vcwprintf(const char *__restrict fmt, va_list ap) {
	wchar_t* ret = xmbstowcs(vcprintf(fmt,ap));
	if (!ret) return NULL;
	return ret;
}

XSTDDEF_INLINE_API wchar_t* cwprintf(const char *__restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	wchar_t *s = vcwprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDDEF_INLINE_API wchar_t* vwcprintf(const wchar_t *__restrict fmt, va_list ap) {
	if (!fmt) return NULL;
	va_list apc, apf;
	va_copy(apc, ap);
	size_t len = vxwcslen(fmt, apc);
	va_end(apc);
	if (len == (size_t)-1) return NULL;
	wchar_t *s = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!s) return NULL;
	va_copy(apf, ap);
	int written = vswprintf(s, len + 1, fmt, apf);
	va_end(apf);
	if (written < 0) {
		free(s);
		return NULL;
	}
	return s;
}

XSTDDEF_INLINE_API wchar_t* wcprintf(const wchar_t *__restrict fmt, ...) {
	if (!fmt) return NULL;
	va_list ap;
	va_start(ap, fmt);
	wchar_t *s = vwcprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDDEF_INLINE_API char* vwccprintf(const wchar_t *__restrict fmt, va_list ap) {
	char* ret = xwcstombs(vwcprintf(fmt, ap));
	if (!ret) return NULL;
	return ret;
}

XSTDDEF_INLINE_API char* wccprintf(const wchar_t *__restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char *s = vwccprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDDEF_INLINE_API FILE* wfopen(const wchar_t *__restrict __filename, const wchar_t *__restrict __modes) {
#if defined(_WIN32) || defined(_WIN64)
	return _wfopen(__filename, __modes);
#else
	char *filename = xwcstombs(__filename);
	if (!filename) return NULL;
	char *modes = xwcstombs(__modes);
	if (!modes) {
		free(filename);
		return NULL;
	}
	FILE *ret = fopen(filename, modes);
	free(filename);
	free(modes);
	return ret;
#endif
}

XSTDDEF_INLINE_API int wremove(const wchar_t *__restrict fmt) {
	if (!fmt) return -1;
	char *s = xwcstombs(fmt);
	if (!s) return -1;
	int ret = remove(s);
	free(s);
	return ret;
}

XSTDDEF_INLINE_API int vxremove(const char *__restrict fmt, va_list ap) {
	if (!fmt) return -1;
	va_list apc;
	va_copy(apc,ap);
	char *s = vcprintf(fmt,apc);
	va_end(apc);
	if (!s) return -1;
	int ret = remove(s);
	free(s);
	return ret;
}

XSTDDEF_INLINE_API int xremove(const char *__restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int ret = vxremove(fmt, ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vxwremove(const wchar_t *__restrict fmt, va_list ap) {
	if (!fmt) return -1;
	va_list apc;
	va_copy(apc, ap);
	wchar_t *s = vwcprintf(fmt,apc);
	va_end(apc);
	int ret = wremove(s);
	free(s);
	return ret;
}

XSTDDEF_INLINE_API int xwremove(const wchar_t *__restrict fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	int ret = vxwremove(fmt,ap);
	va_end(ap);
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif // __EXTSTDIO_H__
