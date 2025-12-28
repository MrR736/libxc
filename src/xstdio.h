/**
 * xstdio.h: Extern stdio.h
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
#include <cstddef>

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
#ifdef _WIN32
	_lock_file(fp);
#else
	flockfile(fp);
#endif
	return fp;
}

XSTDDEF_INLINE_API size_t fpsize(FILE *fp) {
	if (!fp) {
		errno = EINVAL;
		return (size_t)-1;
	}

	long original_pos = ftell(fp);
	if (original_pos < 0)
		return (size_t)-1;

	if (fseek(fp, 0, SEEK_END) != 0)
		return (size_t)-1;

	long end_pos = ftell(fp);
	if (end_pos < 0) {
		fseek(fp, original_pos, SEEK_SET);
		return (size_t)-1;
	}

	if (fseek(fp, original_pos, SEEK_SET) != 0)
		return (size_t)-1;

	if (end_pos < original_pos) {
		errno = EIO;
		return (size_t)-1;
	}

	long diff = end_pos - original_pos;
	if (diff < 0 || (unsigned long)diff > SIZE_MAX) {
		errno = EOVERFLOW;
		return (size_t)-1;
	}

	return (size_t)diff;
}

/* full read */
XSTDDEF_INLINE_API void *furead(FILE *fp, size_t *out_size) {
	if (!fp) {
		errno = EINVAL;
		return NULL;
	}

	size_t size = fpsize(fp);
	if (size == (size_t)-1)
		return NULL;

	if (size == SIZE_MAX) {
		errno = EOVERFLOW;
		return NULL;
	}

	unsigned char *data = (unsigned char *)malloc(size + 1);
	if (!data)
		return NULL;

	size_t nread = fread(data, 1, size, fp);
	if (nread != size && ferror(fp)) {
		free(data);
		return NULL;
	}

	data[nread] = '\0';

	if (out_size)
		*out_size = nread;

	return data;
}

XSTDDEF_INLINE_API size_t fdsize(int fd) {
	if (fd < 0) {
		errno = EINVAL;
		return (size_t)-1;
	}

	off_t original_pos = lseek(fd, 0, SEEK_CUR);
	if (original_pos == (off_t)-1)
		return (size_t)-1;

	off_t end_pos = lseek(fd, 0, SEEK_END);
	if (end_pos == (off_t)-1) {
		lseek(fd, original_pos, SEEK_SET);
		return (size_t)-1;
	}

	if (lseek(fd, original_pos, SEEK_SET) == (off_t)-1)
		return (size_t)-1;

	if (end_pos < original_pos) {
		errno = EIO;
		return (size_t)-1;
	}

	off_t diff = end_pos - original_pos;
	if (diff < 0 || (unsigned long long)diff > SIZE_MAX) {
		errno = EOVERFLOW;
		return (size_t)-1;
	}

	return (size_t)diff;
}

XSTDDEF_INLINE_API void *fduread(int fd, size_t *out_size) {
	if (fd < 0) {
		errno = EINVAL;
		return NULL;
	}

	if (out_size)
		*out_size = 0;

	size_t cap = fdsize(fd);
	if (cap == (size_t)-1)
		return NULL;

	if (cap == SIZE_MAX) {
		errno = EOVERFLOW;
		return NULL;
	}

	unsigned char *buf = (unsigned char *)malloc(cap + 1);
	if (!buf)
		return NULL;

	size_t len = 0;
	while (len < cap) {
		ssize_t n = read(fd, buf + len, cap - len);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			free(buf);
			return NULL;
		}
		if (n == 0)
			break; /* EOF */
		len += (size_t)n;
	}

	buf[len] = '\0'; /* optional NUL terminator */

	/* optional shrink */
	if (len < cap) {
		unsigned char *tmp = (unsigned char *)realloc(buf, len + 1);
		if (tmp)
			buf = tmp;
	}

	if (out_size)
		*out_size = len;

	return buf;
}

XSTDDEF_INLINE_API char* vcprintf(const char *__restrict fmt, va_list ap) {
	if (!fmt) return NULL;
	va_list apc;

	size_t len = vxstrlen(fmt, ap);
	if (len == (size_t)-1) return NULL;

	char *s = (char*)malloc(len + 1);
	if (!s) return NULL;

	va_copy(apc, ap);
	int written = vsnprintf(s, len + 1, fmt, apc);
	va_end(apc);

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
	char* s = vcprintf(fmt,ap);
	if (!s) return NULL;
	wchar_t* ret = xmbstowcs(s);
	if (!ret) {
		free(s);
		return NULL;
	}
	free(s);
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
	wchar_t* s = vwcprintf(fmt, ap);
	if (!s) return NULL;
	char* ret = xwcstombs(s);
	if (!ret) {
		free(s);
		return NULL;
	}
	free(s);
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
#ifdef _WIN32
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
#ifdef _WIN32
	return _wremove(fmt);
#else
	if (!fmt) return -1;
	char *s = xwcstombs(fmt);
	if (!s) return -1;
	int ret = remove(s);
	free(s);
	return ret;
#endif
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

XSTDDEF_INLINE_API char* getcurrentdirectory_size(size_t n) {
	char *abs_path = (char*)malloc(n);
	if (!abs_path) {
		errno = ENOMEM;
		return NULL;
	}
	size_t current_size = n;
	size_t len;
#ifdef _WIN32
	DWORD result = 0;
	while (1) {
		result = GetModuleFileNameA(NULL, abs_path, (DWORD)current_size);
		if (result == 0) {
			free(abs_path);
			errno = EINVAL;
			return NULL;
		}
		if (result < current_size)
			break;
		current_size *= 2;
		abs_path = (char*)realloc(abs_path, current_size);
		if (!abs_path) {
			errno = ENOMEM;
			return NULL;
		}
	}

#else
	ssize_t result = 0;
	while (1) {
		result = readlink("/proc/self/exe", abs_path, current_size - 1);
		if (result == -1) {
			free(abs_path);
			return NULL;
		}

		if (result < current_size - 1)
			break;

		current_size *= 2;
		abs_path = (char*)realloc(abs_path, current_size);
		if (!abs_path) {
			errno = ENOMEM;
			return NULL;
		}
	}
#endif
	abs_path[result] = '\0';
	char *last_sep = strrchr(abs_path, '/');
#ifdef _WIN32
	char *last_win = strrchr(abs_path, '\\');
	if (last_win && (!last_sep || last_win > last_sep))
		last_sep = last_win;
#endif
	if (!last_sep) {
		free(abs_path);
		char *dot = (char*)malloc(2);
		if (!dot) {
			errno = ENOMEM;
			return NULL;
		}
		strcpy(dot, ".");
		return dot;
	}
	size_t dir_len = (size_t)(last_sep - abs_path);
	char *dir = (char*)malloc(dir_len + 1);
	if (!dir) {
		free(abs_path);
		errno = ENOMEM;
		return NULL;
	}
	memcpy(dir, abs_path, dir_len);
	dir[dir_len] = '\0';
	free(abs_path);
	return dir;
}

XSTDDEF_INLINE_API char* getcurrentdirectory(void) {
	return getcurrentdirectory_size(XPATH_MAX + 1);
}

XSTDDEF_INLINE_API wchar_t* wgetcurrentdirectory_size(size_t n) {
	char *s = getcurrentdirectory_size(n);
	if (!s) return NULL;
	wchar_t* ws = xmbstowcs(s);
	if (!ws) return NULL;
	free(s);
	return ws;
}

XSTDDEF_INLINE_API wchar_t* wgetcurrentdirectory(void) {
	return wgetcurrentdirectory_size(XPATH_MAX + 1);
}

#ifdef __cplusplus
}
#endif

#endif // __EXTSTDIO_H__
