/**
 * xstdlib.h: Extern stdlib.h
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

#ifndef __XSTDLIB_H__
#define __XSTDLIB_H__

#include "xtypes.h"
#include "xstring.h"
#include "xwchar.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI void* XCALLAPI xrand(size_t size, unsigned int flags) {
#ifdef _WIN32
	(void)flags;
	if (size == 0) {
		errno = EINVAL;
		return NULL;
	}
	void *ret = malloc(size);
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}
	if (size > ULONG_MAX) {
		free(ret);
		errno = EOVERFLOW;
		return NULL;
	}
	NTSTATUS status = BCryptGenRandom(NULL,(PUCHAR)ret,(ULONG)size,BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (status != 0) {
		free(ret);
		errno = EIO;
		return NULL;
	}
	return ret;
#else
	if (size == 0 || flags == 0) {
		errno = EINVAL;
		return NULL;
	}
	void *ret = malloc(size);
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}
	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < size; ++i) ((unsigned char *)ret)[i] = (unsigned char)(rand() % flags);
	return ret;
#endif
}

// Convert wide character string (wchar_t*) to UTF-8 multi-byte string (char*)
XSTDAPI char* XCALLAPI xwcstombs(const wchar_t *wcs) {
	if (!wcs) return NULL;

#ifdef _WIN32
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
XSTDAPI wchar_t* XCALLAPI xmbstowcs(const char *mbs) {
	if (!mbs) return NULL;

#ifdef _WIN32
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

XSTDAPI int XCALLAPI wsystem(const wchar_t *xrestrict command) {
#ifdef _WIN32
	return _wsystem(command);
#else
	char *mbs = xwcstombs(command);
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

XSTDAPI void XCALLAPI wperror(const wchar_t *xrestrict s) {
#ifdef _WIN32
	_wperror(s);
#else
	char *mbs = xwcstombs(s);
	if (!mbs) return;
	perror(mbs);
	free(mbs);
#endif
}

XSTDAPI int XCALLAPI vnxsystem(size_t len, const char *xrestrict command, va_list ap) {
	va_list apc;
	if (len == 0) return -1;
	char *cmd = (char *)malloc(len);
	if (!cmd) return -2;
	int wret = vsnprintf(cmd, len, command, ap);
	if (wret < 0 || (size_t)wret > len) {
		free(cmd);
		return -3;
	}
	int ret = system(cmd);
	free(cmd);
	return ret;
}

XSTDAPI int XCALLAPI nxsystem(size_t len,const char *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxsystem(len, command, ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxsystem(const char *xrestrict command, va_list ap) {
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

XSTDAPI int XCALLAPI xsystem(const char *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxsystem(command,ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vnxwsystem(size_t len, const wchar_t *xrestrict command, va_list ap) {
	va_list apc,apf;
	if (len <= 0) return -1;
	// Allocate memory for the formatted command
	wchar_t *wcmd = (wchar_t *)malloc(len * sizeof(wchar_t));
	if (!wcmd) return -2;
	va_copy(apf, ap);
	int wret = vswprintf(wcmd, len, command, apf);
	va_end(apf);
	if (wret < 0 || (size_t)wret >= len) {
		free(wcmd);
		return -3;
	}
	int ret = wsystem(wcmd);
	free(wcmd);
	return ret;
}

XSTDAPI int XCALLAPI nxwsystem(size_t len,const wchar_t *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxwsystem(len,command,ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxwsystem(const wchar_t *xrestrict command, va_list ap) {
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

XSTDAPI int XCALLAPI xwsystem(const wchar_t *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxwsystem(command,ap);
	va_end(ap);
	return ret;
}

XSTDAPI void XCALLAPI vnxperror(size_t len, const char* xrestrict fmt, va_list ap) {
	if (len == 0 || !fmt) return;

	char *buf = (char *)malloc(len);
	if (!buf) return;

	int n = vsnprintf(buf, len, fmt, ap);
	if (n < 0) buf[0] = '\0';
	else if ((size_t)n >= len) buf[len - 1] = '\0';
	perror(buf);
	free(buf);
}

XSTDAPI void XCALLAPI nxperror(size_t len, const char* xrestrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vnxperror(len, fmt, ap);
	va_end(ap);
}

XSTDAPI void XCALLAPI vxperror(const char* xrestrict fmt, va_list ap) {
	if (!fmt) return;
	va_list apc, apf;
	va_copy(apc, ap);
	size_t len = vxstrlen(fmt, apc) + 1;
	va_end(apc);
	va_copy(apf, ap);
	vnxperror(len, fmt, apf);
	va_end(apf);
}

XSTDAPI void XCALLAPI xperror(const char* xrestrict fmt, ...) {
	if (!fmt) return;
	va_list ap;
	va_start(ap, fmt);
	vxperror(fmt, ap);
	va_end(ap);
}

XSTDAPI void XCALLAPI vnxwperror(size_t len,const wchar_t *xrestrict s, va_list ap) {
	va_list apc;
	if (len <= 0) return;
	wchar_t *wcmd = (wchar_t*)malloc(len * sizeof(wchar_t));
	if (!wcmd) return;
	va_copy(apc, ap);
	int ret = vswprintf(wcmd, len, s, apc);
	va_end(apc);
	if (ret < 0 || (size_t)ret >= len) {
		free(wcmd);
		return;
	}
	wperror(wcmd);
	free(wcmd);
}

XSTDAPI void XCALLAPI nxwperror(size_t len,const wchar_t *xrestrict s, ...) {
	va_list ap;
	va_start(ap,s);
	vnxwperror(len,s,ap);
	va_end(ap);
}

XSTDAPI void XCALLAPI vxwperror(const wchar_t *xrestrict s, va_list ap) {
	va_list apc,apf;
	va_copy(apc, ap);
	size_t len = vxwcslen(s, apc);
	va_end(apc);
	va_copy(apf, ap);
	vnxwperror((len + 1),s,apf);
	va_end(apf);
}

XSTDAPI void XCALLAPI xwperror(const wchar_t *xrestrict s, ...) {
	va_list ap;
	va_start(ap,s);
	vxwperror(s,ap);
	va_end(ap);
}

/* child system */
XSTDAPI int XCALLAPI csystem(const char *xrestrict cmd) {
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

XSTDAPI int XCALLAPI wcsystem(const wchar_t *xrestrict command) {
#ifdef _WIN32
	return _wsystem(command);
#else
	char *mbs = xwcstombs(command);
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

XSTDAPI int XCALLAPI vnxcsystem(size_t len, const char *xrestrict command, va_list ap) {
	va_list apc;
	if (len == 0) return -1;
	char *cmd = (char *)malloc(len);
	if (!cmd) return -2;
	va_copy(apc,ap);
	int wret = vsnprintf(cmd, len, command, apc);
	va_end(apc);
	if (wret < 0 || (size_t)wret > len) {
		free(cmd);
		return -3;
	}
	int ret = csystem(cmd);
	free(cmd);
	return ret;
}

XSTDAPI int XCALLAPI nxcsystem(size_t len,const char *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxcsystem(len, command, ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxcsystem(const char *xrestrict command, va_list ap) {
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

XSTDAPI int XCALLAPI xcsystem(const char *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vxcsystem(command,ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vnxwcsystem(size_t len, const wchar_t *xrestrict command, va_list ap) {
	va_list apc,apf;
	if (len <= 0) return -1;
	// Allocate memory for the formatted command
	wchar_t *wcmd = (wchar_t *)malloc(len * sizeof(wchar_t));
	if (!wcmd) return -2;
	va_copy(apf, ap);
	int wret = vswprintf(wcmd, len, command, apf);
	va_end(apf);
	if (wret < 0 || (size_t)wret >= len) {
		free(wcmd);
		return -3;
	}
	int ret = wcsystem(wcmd);
	free(wcmd);
	return ret;
}

XSTDAPI int XCALLAPI nxwcsystem(size_t len,const wchar_t *xrestrict command, ...) {
	va_list ap;
	va_start(ap,command);
	int ret = vnxwcsystem(len,command,ap);
	va_end(ap);
	return ret;
}

XSTDAPI int XCALLAPI vxwcsystem(const wchar_t *xrestrict command, va_list ap) {
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

XSTDAPI int XCALLAPI xwcsystem(const wchar_t *xrestrict command, ...) {
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
