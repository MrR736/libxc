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
#include "xmemory.h"

#ifndef XRANDOM_FLAG
#define XRANDOM_FLAG 256
#endif

#define xrandom(size) xrand(size,XRANDOM_FLAG)

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
	void *ret = xmalloc(size);
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
	void *ret = xmalloc(size);
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}
#ifdef __APPLE__
	unsigned char* p = (unsigned char*)(ret);
	size_t remaining = size;
	while (remaining != 0) {
		size_t n = remaining > 256 ? 256 : remaining;
		if (getentropy(p, n) != 0) {
			int error = errno;
			xfree(ret);
			errno = error;
			return NULL;
		}
		p += n;
		remaining -= n;
	}
#elif defined(__linux__)
	unsigned char* p = (unsigned char*)ret;
	size_t remaining = size;

	while (remaining != 0) {
		ssize_t n = getrandom(p, remaining, 0);

		if (n < 0) {
			int error = errno;
			xfree(ret);
			errno = error;
			return NULL;
		}

		if (n == 0) {
			xfree(ret);
			errno = EIO;
			return NULL;
		}

		p += n;
		remaining -= (size_t)n;
	}
#else
	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < size; ++i) ((unsigned char *)ret)[i] = (unsigned char)(rand() % flags);
#endif
	return ret;
#endif
}

// Convert wide character string (wchar_t*) to UTF-8 multi-byte string (char*)
XSTDAPI char* XCALLAPI xwcstombs_len(const wchar_t *xrestrict wcs,size_t* out_size) {
	if (out_size) *out_size = 0;
	if (!wcs) {
		errno = EINVAL;
		return NULL;
	}

	if (wcs[0] == L'\0') {
		char *m = (char *)xmalloc(1);
		if (!m) {
			errno = ENOMEM;
			return NULL;
		}
		m[0] = '\0';
		if (out_size) *out_size = 1;
		return m;
	}

#ifdef _WIN32
	// -------- Windows UTF-8 conversion --------
	int len = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (len <= 0) {
		errno = EILSEQ;
		return NULL;
	}

	char *mbs = (char*)xmalloc((size_t)len);
	if (!mbs) {
		errno = ENOMEM;
		return NULL;
	}

	if (WideCharToMultiByte(CP_UTF8, 0, wcs, -1,mbs, len, NULL, NULL) <= 0) {
		xfree(mbs);
		errno = EILSEQ;
		return NULL;
	}
	if (out_size) *out_size = (size_t)len;
	return mbs;
#else
	// -------- Linux/Unix UTF-8 conversion --------
	size_t len = wcstombs(NULL, wcs, 0);
	if (len <= 0) {
		errno = EILSEQ;
		return NULL;
	}
	char *mbs = (char*)xmalloc(len + 1);
	if (!mbs) {
		errno = ENOMEM;
		return NULL;
	}
	if (wcstombs(mbs, wcs, len + 1) == (size_t)-1) {
		xfree(mbs);
		errno = EILSEQ;
		return NULL;
	}
	if (out_size) *out_size = len + 1;
	return mbs;
#endif
}

XSTDAPI char* XCALLAPI xwcstombs(const wchar_t *xrestrict wcs) {
	return xwcstombs_len(wcs,NULL);
}

XSTDAPI char** XCALLAPI xwcsstombss(const wchar_t **xrestrict wcs,size_t count) {
	char **mbs;

	if (!wcs) {
		errno = EINVAL;
		return NULL;
	}

	if (count > SIZE_MAX / sizeof(*mbs) - 1) {
		errno = EOVERFLOW;
		return NULL;
	}

	mbs = (char **)xmalloc((count + 1) * sizeof(*mbs));
	if (!mbs) {
		errno = ENOMEM;
		return NULL;
	}

	for (size_t i = 0; i <= count; i++) mbs[i] = NULL;

	for (size_t i = 0; i < count; i++) {
		if (!wcs[i]) {
			errno = EINVAL;
			xfrees((void **)mbs, count);
			return NULL;
		}
		mbs[i] = xwcstombs(wcs[i]);
		if (!mbs[i]) {
			int saved_errno = errno;
			xfrees((void **)mbs, count);
			errno = saved_errno;
			return NULL;
		}
	}

	return mbs;
}

// Convert UTF-8 multi-byte string (char*) to wide character string (wchar_t*)
XSTDAPI wchar_t* XCALLAPI xmbstowcs_len(const char *xrestrict mbs, size_t *out_size) {
	if (out_size) *out_size = 0;

	if (!mbs) {
		errno = EINVAL;
		return NULL;
	}

	if (mbs[0] == '\0') {
		wchar_t *w = (wchar_t *)xmalloc(sizeof(wchar_t));
		if (!w) {
			errno = ENOMEM;
			return NULL;
		}
		w[0] = L'\0';
		if (out_size) *out_size = 1;
		return w;
	}

#ifdef _WIN32
	// -------- Windows UTF-8 conversion --------
	int len = MultiByteToWideChar(CP_UTF8, 0, mbs, -1, NULL, 0);
	if (len <= 0) {
		errno = EILSEQ;
		return NULL;
	}

	wchar_t *wcs = (wchar_t *)xmalloc((size_t)len * sizeof(wchar_t));
	if (!wcs) {
		errno = ENOMEM;
		return NULL;
	}

	if (MultiByteToWideChar(CP_UTF8, 0, mbs, -1, wcs, len) <= 0) {
		xfree(wcs);
		errno = EILSEQ;
		return NULL;
	}
	if (out_size) *out_size = (size_t)len;
	return wcs;
#else
	// -------- POSIX locale-based conversion --------
	size_t len = mbstowcs(NULL, mbs, 0);
	if (len == (size_t)-1) {
		errno = EILSEQ;
		return NULL;
	}

	wchar_t *wcs = (wchar_t *)xmalloc((len + 1) * sizeof(wchar_t));
	if (!wcs) {
		errno = ENOMEM;
		return NULL;
	}

	if (mbstowcs(wcs, mbs, len + 1) == (size_t)-1) {
		xfree(wcs);
		errno = EILSEQ;
		return NULL;
	}

	if (out_size)
		*out_size = len + 1;

	return wcs;
#endif
}

XSTDAPI wchar_t* XCALLAPI xmbstowcs(const char *xrestrict mbs) {
	return xmbstowcs_len(mbs,NULL);
}

XSTDAPI wchar_t** XCALLAPI xmbsstowcss(const char **xrestrict mbs,size_t count) {
	wchar_t **wcs;

	if (!mbs) {
		errno = EINVAL;
		return NULL;
	}

	if (count > SIZE_MAX / sizeof(*wcs) - 1) {
		errno = EOVERFLOW;
		return NULL;
	}

	wcs = (wchar_t **)xmalloc((count + 1) * sizeof(*wcs));
	if (!wcs) {
		errno = ENOMEM;
		return NULL;
	}

	for (size_t i = 0; i <= count; i++) wcs[i] = NULL;

	for (size_t i = 0; i < count; i++) {
		if (!mbs[i]) {
			errno = EINVAL;
			xfrees((void **)wcs, count);
			return NULL;
		}

		wcs[i] = xmbstowcs_len(mbs[i], NULL);
		if (!wcs[i]) {
			int saved_errno = errno;
			xfrees((void **)wcs, count);
			errno = saved_errno;
			return NULL;
		}
	}

	return wcs;
}

XSTDAPI int XCALLAPI wsystem(const wchar_t *xrestrict command) {
#ifdef _WIN32
	return _wsystem(command);
#else
	char *mbs = xwcstombs(command);
	if (!mbs) return -1;
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
	char *cmd = (char *)xmalloc(len);
	if (!cmd) return -2;
	va_copy(apc,ap);
	int wret = vsnprintf(cmd, len, command, apc);
	va_end(apc);
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
	va_list apf;
	size_t len = vxstrlen(command,ap);
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
	wchar_t *wcmd = (wchar_t *)xmalloc(len * sizeof(wchar_t));
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
	va_list apf;
	size_t len = vxwcslen(command,ap);
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
	char *buf = (char *)xmalloc(len);
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
	va_list apf;
	size_t len = vxstrlen(fmt,ap) + 1;
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
	wchar_t *wcmd = (wchar_t*)xmalloc(len * sizeof(wchar_t));
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
	va_list apf;
	size_t len = vxwcslen(s,ap);
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
	if (rc == -1) perror("csystem: _spawnlp failed");
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

	if (WIFEXITED(status)) return WEXITSTATUS(status);
	if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
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
	char *cmd = (char *)xmalloc(len);
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
	va_list apf;
	size_t len = vxstrlen(command,ap);
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
	wchar_t *wcmd = (wchar_t *)xmalloc(len * sizeof(wchar_t));
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
	va_list apf;
	size_t len = vxwcslen(command,ap);
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

XSTDAPI int XCALLAPI xwcsstrcmp(const wchar_t* s1, const char* s2) {
	if (!s1 && !s2) return 1;
	if (!s1 || !s2) return 0;
	wchar_t* s2_str = xmbstowcs(s2);
	if (!s2_str) return 0;
	int ret = wcscmp(s1,s2_str) == 0 ? 1 : 0;
	xfree(s2_str);
	return ret;
}

XSTDAPI int XCALLAPI xstrwcscmp(const char* s1, const wchar_t* s2) {
	if (!s1 && !s2) return 1;
	if (!s1 || !s2) return 0;
	char* s2_str = xwcstombs(s2);
	if (!s2_str) return 0;
	int ret = strcmp(s1,s2_str) == 0 ? 1 : 0;
	xfree(s2_str);
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif // __XSTDLIB_H__
