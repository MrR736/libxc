/**
 * xstdlib.h: Extern stdlib.h
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

#ifndef __XSTDLIB_H__
#define __XSTDLIB_H__

#include "xstring.h"
#include "xwchar.h"
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

XSTDDEF_INLINE_API void* xrand(size_t __size, unsigned int __flags) {
	if (__size <= 0 || __flags == 0) {
		errno = EINVAL;
		return NULL;
	}
	void* ret = malloc(__size);
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}
	srand((unsigned int)clock());
	for (size_t i = 0; i < __size; i++)
		((unsigned char*)ret)[i] = (unsigned char)(rand() % __flags);
	return ret;
}

// Convert wide character string (wchar_t*) to UTF-8 multi-byte string (char*)
XSTDDEF_INLINE_API char* xwcstombs(const wchar_t *wcs) {
	if (!wcs) return NULL;

#if defined(_WIN32) || defined(_WIN64)
	// -------- Windows UTF-8 conversion --------
	int len = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (len <= 0) return NULL;

	char *mbs = (char*)malloc((size_t)len);
	if (!mbs) return NULL;

	WideCharToMultiByte(CP_UTF8, 0, wcs, -1, mbs, len, NULL, NULL);
	return mbs;

#else
	size_t len = wcstombs(NULL, wcs, 0);
	if (len <= 0) return NULL;
	char *mbs = (char*)malloc(len + 1);
	if (!mbs) return NULL;
	wcstombs(mbs, wcs, len + 1);
	return mbs;
#endif
}

// Convert UTF-8 multi-byte string (char*) to wide character string (wchar_t*)
XSTDDEF_INLINE_API wchar_t* xmbstowcs(const char *mbs) {
	if (!mbs) return NULL;

#if defined(_WIN32) || defined(_WIN64)
	// -------- Windows UTF-8 conversion --------
	int len = MultiByteToWideChar(CP_UTF8, 0, mbs, -1, NULL, 0);
	if (len <= 0) return NULL;

	wchar_t *wcs = (wchar_t*)malloc(len * sizeof(wchar_t));
	if (!wcs) return NULL;

	MultiByteToWideChar(CP_UTF8, 0, mbs, -1, wcs, len);
	return wcs;

#else
	// -------- Linux/Unix UTF-8 conversion --------
	size_t len = mbstowcs(NULL, mbs, 0);
	if (len == (size_t)-1) return NULL;

	wchar_t *wcs = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!wcs) return NULL;

	mbstowcs(wcs, mbs, len + 1);
	return wcs;
#endif
}

XSTDDEF_INLINE_API int wsystem(const wchar_t *__restrict __command) {
#if defined(_WIN32) || defined(_WIN64)
	return _wsystem(__command);
#else
	char *mbs = xwcstombs(__command);
	if (!mbs)
		return -1;
	int ret = system(mbs);
	free(mbs);
	if (ret != -1) {
		if (WIFEXITED(ret)) ret = WEXITSTATUS(ret);
		else if (WIFSIGNALED(ret)) ret = 128 + WTERMSIG(ret);
	}
	return ret;
#endif
}

XSTDDEF_INLINE_API void wperror(const wchar_t *__restrict __s) {
#if defined(_WIN32) || defined(_WIN64)
	_wperror(__s);
#else
	char *mbs = xwcstombs(__s);
	if (!mbs) return;
	perror(mbs);
	free(mbs);
#endif
}

XSTDDEF_INLINE_API int vnxsystem(size_t __len, const char *__restrict command, va_list ap) {
	va_list apc;
	if (__len == 0) return -1;
	char *cmd = (char *)malloc(__len);
	if (!cmd) return -2;
	int wret = vsnprintf(cmd, __len, command, ap);
	if (wret < 0 || (size_t)wret > __len) {
		free(cmd);
		return -3;
	}
	int ret = system(cmd);
	free(cmd);
	return ret;
}

XSTDDEF_INLINE_API int nxsystem(size_t __len,const char *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxsystem(__len, command, ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vxsystem(const char *__restrict command, va_list ap) {
	va_list apc, apf;
	va_copy(apc,ap);
	size_t len = vxstrlen(command,apc);
	va_end(apc);
	if (len == (size_t)-1) return -1;

	va_copy(apf,ap);
	int ret = vnxsystem((len + 1),command,apf);
	va_end(apf);
	return ret;
}

XSTDDEF_INLINE_API int xsystem(const char *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxsystem(command,ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vnxwsystem(size_t __len, const wchar_t *__restrict command, va_list ap) {
	va_list apc,apf;
	if (__len <= 0) return -1;
	// Allocate memory for the formatted command
	wchar_t *wcmd = (wchar_t *)malloc(__len * sizeof(wchar_t));
	if (!wcmd) return -2;
	va_copy(apf, ap);
	int wret = vswprintf(wcmd, __len, command, apf);
	va_end(apf);
	if (wret < 0 || (size_t)wret >= __len) {
		free(wcmd);
		return -3;
	}
	int ret = wsystem(wcmd);
	free(wcmd);
	return ret;
}

XSTDDEF_INLINE_API int nxwsystem(size_t len,const wchar_t *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxwsystem(len,command,ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vxwsystem(const wchar_t *__restrict command, va_list ap) {
	va_list apc,apf;
	va_copy(apc,ap);
	size_t len = vxwcslen(command,apc);
	va_end(apc);
	if (len == (size_t)-1) return -1;
	va_copy(apf,ap);
	int ret = vnxwsystem((len + 1),command,apf);
	va_end(apf);
	return ret;
}

XSTDDEF_INLINE_API int xwsystem(const wchar_t *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxwsystem(command,ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API void vnxperror(size_t len, const char* __restrict fmt, va_list ap) {
	if (len == 0 || !fmt) return;

	char *buf = (char *)malloc(len);
	if (!buf) return;

	int n = vsnprintf(buf, len, fmt, ap);
	if (n < 0)
		buf[0] = '\0';
	else if ((size_t)n >= len)
		buf[len - 1] = '\0';
	perror(buf);
	free(buf);
}

XSTDDEF_INLINE_API void nxperror(size_t len, const char* __restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vnxperror(len, fmt, ap);
	va_end(ap);
}

XSTDDEF_INLINE_API void vxperror(const char* __restrict fmt, va_list ap) {
	if (!fmt) return;
	va_list apc, apf;
	va_copy(apc, ap);
	size_t len = vxstrlen(fmt, apc) + 1;
	va_end(apc);
	va_copy(apf, ap);
	vnxperror(len, fmt, apf);
	va_end(apf);
}

XSTDDEF_INLINE_API void xperror(const char* __restrict fmt, ...) {
	if (!fmt) return;
	va_list ap;
	va_start(ap, fmt);
	vxperror(fmt, ap);
	va_end(ap);
}

XSTDDEF_INLINE_API void vnxwperror(size_t __len,const wchar_t *__restrict __s, va_list ap) {
	va_list apc;
	if (__len <= 0) return;
	wchar_t *wcmd = (wchar_t*)malloc(__len * sizeof(wchar_t));
	if (!wcmd) return;
	va_copy(apc, ap);
	int ret = vswprintf(wcmd, __len, __s, apc);
	va_end(apc);
	if (ret < 0 || (size_t)ret >= __len) {
		free(wcmd);
		return;
	}
	wperror(wcmd);
	free(wcmd);
}

XSTDDEF_INLINE_API void nxwperror(size_t __len,const wchar_t *__restrict __s, ...) {
	va_list ap;
	va_start(ap,__s);
	vnxwperror(__len,__s,ap);
	va_end(ap);
}

XSTDDEF_INLINE_API void vxwperror(const wchar_t *__restrict __s, va_list ap) {
	va_list apc,apf;
	va_copy(apc, ap);
	size_t len = vxwcslen(__s, apc);
	va_end(apc);
	va_copy(apf, ap);
	vnxwperror((len + 1),__s,apf);
	va_end(apf);
}

XSTDDEF_INLINE_API void xwperror(const wchar_t *__restrict __s, ...) {
	va_list ap;
	va_start(ap,__s);
	vxwperror(__s,ap);
	va_end(ap);
}

/* child system */
XSTDDEF_INLINE_API int csystem(const char *__restrict cmd) {
	if (!cmd || !*cmd) {
		errno = EINVAL;
		perror("csystem: empty command");
		return -1;
	}

#ifdef _WIN32
	/* Windows version using cmd.exe */
	int rc = _spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", "/C", cmd, (char*)0);
	if (rc == -1)
		perror("csystem: _spawnlp failed");
	return rc;

#else /* POSIX systems: Linux, BSDs, Android, etc. */
	pid_t pid = fork();
	if (pid < 0) {
		perror("csystem: fork failed");
		return -1;
	}
	if (pid == 0) {
		/* Child process */
#ifdef __ANDROID__
		execl("/system/bin/sh","sh","-c",cmd,NULL);
#else
		execl("/bin/sh","sh","-c",cmd,NULL);
#endif
		perror("csystem: execl failed");
		_exit(127);
	}
	/* Parent process */
	int status = 0;
	pid_t w = waitpid(pid, &status, 0);
	if (w == -1) {
		perror("csystem: waitpid failed");
		return -1;
	}

	if (WIFEXITED(status))
		return WEXITSTATUS(status);

	if (WIFSIGNALED(status))
		return 128 + WTERMSIG(status);

	return status;
#endif /* _WIN32 */
}

XSTDDEF_INLINE_API int wcsystem(const wchar_t *__restrict __command) {
#if defined(_WIN32) || defined(_WIN64)
	return _wsystem(__command);
#else
	char *mbs = xwcstombs(__command);
	if (!mbs) return -1;
	int ret = csystem(mbs);
	free(mbs);
	if (ret != -1) {
		if (WIFEXITED(ret)) ret = WEXITSTATUS(ret);
		else if (WIFSIGNALED(ret)) ret = 128 + WTERMSIG(ret);
	}
	return ret;
#endif
}

XSTDDEF_INLINE_API int vnxcsystem(size_t __len, const char *__restrict command, va_list ap) {
	va_list apc;
	if (__len == 0) return -1;
	char *cmd = (char *)malloc(__len);
	if (!cmd) return -2;
	va_copy(apc,ap);
	int wret = vsnprintf(cmd, __len, command, apc);
	va_end(apc);
	if (wret < 0 || (size_t)wret > __len) {
		free(cmd);
		return -3;
	}
	int ret = csystem(cmd);
	free(cmd);
	return ret;
}

XSTDDEF_INLINE_API int nxcsystem(size_t __len,const char *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxcsystem(__len, command, ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vxcsystem(const char *__restrict command, va_list ap) {
	va_list apc, apf;
	va_copy(apc,ap);
	size_t len = vxstrlen(command,apc);
	va_end(apc);
	if (len == (size_t)-1) return -1;

	va_copy(apf,ap);
	int ret = vnxcsystem((len + 1),command,apf);
	va_end(apf);
	return ret;
}

XSTDDEF_INLINE_API int xcsystem(const char *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxcsystem(command,ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vnxwcsystem(size_t __len, const wchar_t *__restrict command, va_list ap) {
	va_list apc,apf;
	if (__len <= 0) return -1;
	// Allocate memory for the formatted command
	wchar_t *wcmd = (wchar_t *)malloc(__len * sizeof(wchar_t));
	if (!wcmd) return -2;
	va_copy(apf, ap);
	int wret = vswprintf(wcmd, __len, command, apf);
	va_end(apf);
	if (wret < 0 || (size_t)wret >= __len) {
		free(wcmd);
		return -3;
	}
	int ret = wcsystem(wcmd);
	free(wcmd);
	return ret;
}

XSTDDEF_INLINE_API int nxwcsystem(size_t len,const wchar_t *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxwcsystem(len,command,ap);
	va_end(ap);
	return ret;
}

XSTDDEF_INLINE_API int vxwcsystem(const wchar_t *__restrict command, va_list ap) {
	va_list apc,apf;
	va_copy(apc,ap);
	size_t len = vxwcslen(command,apc);
	va_end(apc);
	if (len == (size_t)-1) return -1;
	va_copy(apf,ap);
	int ret = vnxwcsystem((len + 1),command,apf);
	va_end(apf);
	return ret;
}

XSTDDEF_INLINE_API int xwcsystem(const wchar_t *__restrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxwcsystem(command,ap);
	va_end(ap);
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif // __XSTDLIB_H__
