/**
 * xstdio.h: Extern stdio.h
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

#ifndef __XSTDIO_H__
#define __XSTDIO_H__

#include "xtypes.h"
#include "xstdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI fdputs(const char *xrestrict s, int fd) {
	if (!s) {
		errno = EINVAL;
		return -1;
	}
	size_t len   = strlen(s);
	size_t total = 0;
	while (total < len) {
		ssize_t n = write(fd, s + total, len - total);
		if (n <= 0) return -1;  // write failed
		total += (size_t)n;
	}
	return 0;
}

XSTDAPI FILE* XCALLAPI fopenm(const char *xrestrict filename, int oflag, ...) {
	char mode[16];
	size_t i = 0;
#ifndef _WIN32
	xmode_t permissions = 0;
	if (oflag & O_CREAT) {
		va_list ap;
		va_start(ap, oflag);
		permissions = va_arg(ap, xmode_t);
		va_end(ap);
	}
#endif

	switch (oflag & (O_RDONLY | O_WRONLY | O_RDWR)) {
		case O_RDONLY:
			mode[i++] = 'r';
			break;
		case O_WRONLY:
			mode[i++] = (oflag & O_APPEND) ? 'a' : 'w';
			break;
		case O_RDWR:
			mode[i++] = (oflag & O_APPEND) ? 'a' : 'w';
			mode[i++] = '+';
			break;
		default:
			errno = EINVAL;
			return NULL;
	}

	mode[i++] = 'b';
	mode[i] = '\0';

	/*
	 * fopen() already provides:
	 *
	 *   O_RDONLY             -> "r"
	 *   O_WRONLY             -> "w"
	 *   O_RDWR               -> "w+"
	 *   O_APPEND             -> "a" / "a+"
	 *   O_TRUNC              -> "w" / "w+"
	 *   O_CREAT              -> "w" / "w+"
	 *
	 * O_EXCL cannot be safely emulated by fopen().
	 */
	FILE *fp = fopen(filename, mode);
	if (!fp) return NULL;

#ifndef _WIN32
	if (oflag & O_CREAT) {
		if (fchmod(fileno(fp), permissions) != 0) {
			int saved_errno = errno;
			fclose(fp);
			errno = saved_errno;
			return NULL;
		}
	}
#endif
	return fp;
}

XSTDAPI FILE* XCALLAPI fdopenm(int fd, int oflag, ...) {
	char mode[16];
	size_t i = 0;
#ifndef _WIN32
	xmode_t permissions = 0;

	if (oflag & O_CREAT) {
		va_list ap;
		va_start(ap, oflag);
		permissions = (xmode_t)va_arg(ap, int);
		permissions = va_arg(ap, xmode_t);
		va_end(ap);
	}
#endif

	switch (oflag & (O_RDONLY | O_WRONLY | O_RDWR)) {
		case O_RDONLY:
			mode[i++] = 'r';
			break;
		case O_WRONLY:
			mode[i++] = (oflag & O_APPEND) ? 'a' : 'w';
			break;
		case O_RDWR:
			mode[i++] = (oflag & O_APPEND) ? 'a' : 'w';
			mode[i++] = '+';
			break;
		default:
			errno = EINVAL;
			return NULL;
	}

	mode[i++] = 'b';
	mode[i] = '\0';

	/*
	 * fopen() already provides:
	 *
	 *   O_RDONLY             -> "r"
	 *   O_WRONLY             -> "w"
	 *   O_RDWR               -> "w+"
	 *   O_APPEND             -> "a" / "a+"
	 *   O_TRUNC              -> "w" / "w+"
	 *   O_CREAT              -> "w" / "w+"
	 *
	 * O_EXCL cannot be safely emulated by fopen().
	 */
	FILE *fp = fdopen(fd, mode);
	if (!fp) return NULL;

#ifndef _WIN32
	if (oflag & O_CREAT) {
		if (fchmod(fileno(fp), permissions) != 0) {
			int saved_errno = errno;
			fclose(fp);
			errno = saved_errno;
			return NULL;
		}
	}
#endif
	return fp;
}

XSTDAPI FILE* XCALLAPI fdno(int fd) {
	if (fd < 0) {
		errno = EBADF;
		return NULL;
	}

	int oflag = 0;
#ifdef _WIN32
	intptr_t h = _get_osfhandle(fd);
	if (h == -1) return NULL;
	if (_isatty(fd)) oflag |= O_RDWR;
	else oflag |= O_RDONLY;
#else
	oflag = fcntl(fd, F_GETFL);
#endif
	if (oflag == -1) return NULL; /* fcntl() already set errno */

	int newfd = dup(fd);
	if (newfd == -1) return NULL;

	FILE *fp = fdopenm(newfd, oflag);
	if (!fp) {
		int saved_errno = errno;
		close(newfd);
		errno = saved_errno;
		return NULL;
	}

	return fp;
}

XSTDAPI void XCALLAPI xflockfile(FILE *fp) {
#ifdef _WIN32
	_lock_file(fp);
#else
	flockfile(fp);
#endif
}

XSTDAPI FILE* XCALLAPI fdno_unlocked(int fd) {
	if (fd < 0) return NULL;
	FILE *fp = fdno(fd);
	if (!fp) return NULL;
	xflockfile(fp);
	return fp;
}

XSTDAPI size_t XCALLAPI fpsize(FILE *fp) {
	if (!fp) {
		errno = EINVAL;
		return (size_t)-1;
	}

	long original_pos = ftell(fp);
	if (original_pos < 0) return (size_t)-1;
	if (fseek(fp, 0, SEEK_END) != 0) return (size_t)-1;

	long end_pos = ftell(fp);
	if (end_pos < 0) {
		fseek(fp, original_pos, SEEK_SET);
		return (size_t)-1;
	}

	if (fseek(fp, original_pos, SEEK_SET) != 0) return (size_t)-1;

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
XSTDAPI void* XCALLAPI furead(FILE *fp, size_t *out_size) {
	if (!fp) {
		errno = EINVAL;
		return NULL;
	}

	size_t size = fpsize(fp);
	if (size == (size_t)-1) return NULL;

	if (size == SIZE_MAX) {
		errno = EOVERFLOW;
		return NULL;
	}

	unsigned char *data = (unsigned char *)malloc(size + 1);
	if (!data) return NULL;

	size_t nread = fread(data, 1, size, fp);
	if (nread != size && ferror(fp)) {
		free(data);
		return NULL;
	}

	data[nread] = '\0';

	if (out_size) *out_size = nread;

	return data;
}

XSTDAPI size_t XCALLAPI fdsize(int fd) {
	if (fd < 0) {
		errno = EINVAL;
		return (size_t)-1;
	}

	off_t original_pos = lseek(fd, 0, SEEK_CUR);
	if (original_pos == (off_t)-1) return (size_t)-1;

	off_t end_pos = lseek(fd, 0, SEEK_END);
	if (end_pos == (off_t)-1) {
		lseek(fd, original_pos, SEEK_SET);
		return (size_t)-1;
	}

	if (lseek(fd, original_pos, SEEK_SET) == (off_t)-1) return (size_t)-1;

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

XSTDAPI void* XCALLAPI fduread(int fd, size_t *out_size) {
	if (fd < 0) {
		errno = EINVAL;
		return NULL;
	}

	if (out_size) *out_size = 0;

	size_t cap = fdsize(fd);
	if (cap == (size_t)-1) return NULL;

	if (cap == SIZE_MAX) {
		errno = EOVERFLOW;
		return NULL;
	}

	unsigned char *buf = (unsigned char *)malloc(cap + 1);
	if (!buf) return NULL;

	size_t len = 0;
	while (len < cap) {
		ssize_t n = read(fd, buf + len, cap - len);
		if (n < 0) {
			if (errno == EINTR) continue;
			free(buf);
			return NULL;
		}
		if (n == 0) break; /* EOF */
		len += (size_t)n;
	}

	buf[len] = '\0'; /* optional NUL terminator */

	/* optional shrink */
	if (len < cap) {
		unsigned char *tmp = (unsigned char *)realloc(buf, len + 1);
		if (tmp) buf = tmp;
	}

	if (out_size) *out_size = len;
	return buf;
}

XSTDAPI char* XCALLAPI vcprintf(const char *xrestrict fmt, va_list ap) {
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

XSTDAPI char* XCALLAPI cprintf(const char *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char *s = vcprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDAPI wchar_t* XCALLAPI vcwprintf(const char *xrestrict fmt, va_list ap) {
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

XSTDAPI wchar_t* XCALLAPI cwprintf(const char *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	wchar_t *s = vcwprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDAPI wchar_t* XCALLAPI vwcprintf(const wchar_t *xrestrict fmt, va_list ap) {
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

XSTDAPI wchar_t* XCALLAPI wcprintf(const wchar_t *xrestrict fmt, ...) {
	if (!fmt) return NULL;
	va_list ap;
	va_start(ap, fmt);
	wchar_t *s = vwcprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDAPI char* XCALLAPI vwccprintf(const wchar_t *xrestrict fmt, va_list ap) {
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

XSTDAPI char* XCALLAPI wccprintf(const wchar_t *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char *s = vwccprintf(fmt, ap);
	va_end(ap);
	return s;
}

XSTDAPI FILE* XCALLAPI wfopen(const wchar_t *xrestrict f, const wchar_t *xrestrict m) {
#ifdef _WIN32
	return _wfopen(f, m);
#else
	char *filename = xwcstombs(f);
	if (!filename) return NULL;
	char *modes = xwcstombs(m);
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


XSTDAPI FILE* XCALLAPI wfopenm(const wchar_t *xrestrict filename, int oflag, ...) {
	wchar_t mode[16];
	size_t i = 0;
#ifndef _WIN32
	xmode_t permissions = 0;

	if (oflag & O_CREAT) {
		va_list ap;
		va_start(ap, oflag);
		permissions = va_arg(ap, xmode_t);
		va_end(ap);
	}
#endif
	switch (oflag & (O_RDONLY | O_WRONLY | O_RDWR)) {
		case O_RDONLY:
			mode[i++] = L'r';
			break;

		case O_WRONLY:
			mode[i++] = (oflag & O_APPEND) ? L'a' : L'w';
			break;

		case O_RDWR:
			mode[i++] = (oflag & O_APPEND) ? L'a' : L'w';
			mode[i++] = L'+';
			break;

		default:
			errno = EINVAL;
			return NULL;
	}

	mode[i++] = L'b';
	mode[i] = L'\0';

	/*
	 * fopen() already provides:
	 *
	 *   O_RDONLY             -> "r"
	 *   O_WRONLY             -> "w"
	 *   O_RDWR               -> "w+"
	 *   O_APPEND             -> "a" / "a+"
	 *   O_TRUNC              -> "w" / "w+"
	 *   O_CREAT              -> "w" / "w+"
	 *
	 * O_EXCL cannot be safely emulated by fopen().
	 */
	FILE *fp = wfopen(filename, mode);
	if (!fp) return NULL;

#ifndef _WIN32
	if (oflag & O_CREAT) {
		if (fchmod(fileno(fp), permissions) != 0) {
			int saved_errno = errno;
			fclose(fp);
			errno = saved_errno;
			return NULL;
		}
	}
#endif
	return fp;
}

XSTDAPI int XCALLAPI wremove(const wchar_t *xrestrict fmt) {
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

XSTDAPI int XCALLAPI vxremove(const char *xrestrict fmt, va_list ap) {
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

XSTDAPI int XCALLAPI xremove(const char *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int ret = vxremove(fmt, ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxwremove(const wchar_t *xrestrict fmt, va_list ap) {
	if (!fmt) return -1;
	va_list apc;
	va_copy(apc, ap);
	wchar_t *s = vwcprintf(fmt,apc);
	va_end(apc);
	int ret = wremove(s);
	free(s);
	return ret;
}

XSTDAPI int XCALLAPI xwremove(const wchar_t *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	int ret = vxwremove(fmt,ap);
	va_end(ap);
	return ret;
}

XSTDAPI char* XCALLAPI getcurrentdirectory_size(size_t n) {
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
		if (result < current_size) break;
#else
	ssize_t result = 0;
	while (1) {
		result = readlink("/proc/self/exe", abs_path, current_size - 1);
		if (result == -1) {
			free(abs_path);
			return NULL;
		}
		if (result < current_size - 1) break;
#endif
		current_size *= 2;
		abs_path = (char*)realloc(abs_path, current_size);
		if (!abs_path) {
			errno = ENOMEM;
			return NULL;
		}
	}
	abs_path[result] = '\0';
	char *last_sep = strrchr(abs_path, '/');
#ifdef _WIN32
	char *last_win = strrchr(abs_path, '\\');
	if (last_win && (!last_sep || last_win > last_sep)) last_sep = last_win;
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

XSTDAPI char* XCALLAPI getcurrentdirectory(void) {
	return getcurrentdirectory_size(XPATH_MAX + 1);
}

XSTDAPI wchar_t* XCALLAPI wgetcurrentdirectory_size(size_t n) {
	char *s = getcurrentdirectory_size(n);
	if (!s) return NULL;
	wchar_t* ws = xmbstowcs(s);
	if (!ws) return NULL;
	free(s);
	return ws;
}

XSTDAPI wchar_t* XCALLAPI wgetcurrentdirectory(void) {
	return wgetcurrentdirectory_size(XPATH_MAX + 1);
}

#ifdef __cplusplus
}
#endif

#endif // __EXTSTDIO_H__
