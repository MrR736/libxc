/**
 * xstat.h: Cross-Platform Directory Operations
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

#ifndef __XSTAT_H__
#define __XSTAT_H__

#include "xstdio.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI xchmod(const char *path, xmode_t m) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
#ifdef _WIN32
	return _chmod(path,(int)m);
#else
	return chmod(path,(mode_t)m);
#endif
}

XSTDAPI int XCALLAPI xwchmod(const wchar_t *path, xmode_t m) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}

#ifdef _WIN32
	return _wchmod(path, (int)m);
#else
	char *p = xwcstombs(path);
	if (!p) return -1;
	int ret = chmod(p, (mode_t)m);
	int saved_errno = errno;
	xfree(p);
	errno = saved_errno;
	return ret;
#endif
}

XSTDAPI int XCALLAPI cmkdir(const char *path, xmode_t m) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
#ifdef _WIN32
	int ret = _mkdir(path);
	if (ret != 0) return ret;
	return xchmod(path,m);
#else
	return mkdir(path,m);
#endif
}

XSTDAPI int XCALLAPI wmkdir(const wchar_t *path,xmode_t m) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
#ifdef _WIN32
	int ret = _wmkdir(path);
	if (ret != 0) return ret;
	return xwchmod(path,m);
#else
	char *s = xwcstombs(path);
	if (!s) return -1;
	int ret = mkdir(s,m);
	xfree(s);
	return ret;
#endif
}

XSTDAPI int XCALLAPI wchdir(const wchar_t *path) {
#ifdef _WIN32
	return _wchdir(path);
#else
	if (!path) return -1;
	char *s = xwcstombs(path);
	if (!s) return -1;
	int ret = chdir(s);
	xfree(s);
	return ret;
#endif
}

XSTDAPI int XCALLAPI wrmdir(const wchar_t *path) {
#ifdef _WIN32
	return _wrmdir(path);
#else
	if (!path) return -1;
	char *s = xwcstombs(path);
	if (!s) return -1;
	int ret = rmdir(s);
	xfree(s);
	return ret;
#endif
}

XSTDAPI int XCALLAPI vxchdir(const char *xrestrict path, va_list ap) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
	va_list apc;
	va_copy(apc, ap);
	char *p = vcprintf(path, apc);
	va_end(apc);
	if (p == NULL) return -1;
	int ret = chdir(p);
	xfree(p);
	return ret;
}

XSTDAPI int XCALLAPI xchdir(const char *xrestrict path, ...) {
	va_list ap;
	va_start(ap, path);
	int ret = vxchdir(path, ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxwchdir(const wchar_t *xrestrict path, va_list ap) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
	va_list apc;
	va_copy(apc, ap);
	wchar_t *p = vwcprintf(path, apc);
	va_end(apc);
	if (p == NULL) return -1;
	int ret = wchdir(p);
	xfree(p);
	return ret;
}

XSTDAPI int XCALLAPI xwchdir(const wchar_t *xrestrict path, ...) {
	va_list ap;
	va_start(ap, path);
	int ret = vxwchdir(path, ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxrmdir(const char *xrestrict path, va_list ap) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
	va_list apc;
	va_copy(apc, ap);
	char *p = vcprintf(path, apc);
	va_end(apc);
	if (p == NULL) return -1;
	int ret = rmdir(p);
	xfree(p);
	return ret;
}

XSTDAPI int XCALLAPI xrmdir(const char *xrestrict path, ...) {
	va_list ap;
	va_start(ap, path);
	int ret = vxrmdir(path, ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxwrmdir(const wchar_t *xrestrict path, va_list ap) {
	if (path == NULL) {
		errno = EINVAL;
		return -1;
	}
	va_list apc;
	va_copy(apc, ap);
	wchar_t *p = vwcprintf(path, apc);
	va_end(apc);
	if (p == NULL) return -1;
	int ret = wrmdir(p);
	xfree(p);
	return ret;
}

XSTDAPI int XCALLAPI xwrmdir(const wchar_t *xrestrict path, ...) {
	va_list ap;
	va_start(ap, path);
	int ret = vxwrmdir(path, ap);
	va_end(ap);
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif