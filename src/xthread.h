/**
 * xthread.h: Cross-Platform Thread Library
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

#ifndef __XTHREAD_H__
#define __XTHREAD_H__

#include "xtypes.h"
#include "xmemory.h"

#ifdef _WIN32
typedef DWORD xthread_result_t;
typedef HANDLE xthread_t;
typedef CRITICAL_SECTION xmutex_t;

typedef struct {
	void *(XCALLAPI *lpStartAddress)(void *);
	void *lpParameter;
	void *result;
} __xthread_t;

# define XTHREAD_CANCEL_ENABLE  1
# define XTHREAD_CANCEL_DISABLE 0
#else
# include <pthread.h>
# ifdef __ANDROID__
#  include <signal.h>
#  define SETUP_SIG_HANDLER(sig) \
do { \
	struct sigaction sa; \
	sa.sa_handler = signal_handler; \
	sa.sa_flags = 0; \
	sigemptyset(&sa.sa_mask); \
	sigaction(sig, &sa, NULL); \
} while (0)

#  define ENABLE_SIGNAL(sig) \
do { \
	SETUP_SIG_HANDLER(sig);  /* Ensure SIG is handled by empty_signal_handler */ \
	sigset_t set; \
	sigemptyset(&set); \
	sigaddset(&set, sig); \
	pthread_sigmask(SIG_UNBLOCK, &set, NULL); \
} while (0)

#  define DISABLE_SIGNAL(sig) \
do { \
	sigset_t set; \
	sigemptyset(&set); \
	sigaddset(&set, sig); \
	pthread_sigmask(SIG_BLOCK, &set, NULL); \
} while (0)
# endif

# ifdef __ANDROID__
#  define XTHREAD_CANCEL_ENABLE  1
#  define XTHREAD_CANCEL_DISABLE 0
# else
#  define XTHREAD_CANCEL_ENABLE  PTHREAD_CANCEL_ENABLE
#  define XTHREAD_CANCEL_DISABLE PTHREAD_CANCEL_DISABLE
# endif

typedef int xthread_result_t;
typedef pthread_t xthread_t;
typedef pthread_mutex_t xmutex_t;

typedef struct {
	LPTHREAD_START_ROUTINE lpStartAddress;
	LPVOID lpParameter;
	DWORD result;
} __xthread_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
XSTDDEF_INLINE_API DWORD XCALLAPI __xthread_wrapper(LPVOID param) {
	__xthread_t *data = (__xthread_t *)param;
	data->result = data->lpStartAddress(data->lpParameter);
	return 0;
}
#else
XSTDDEF_INLINE_API void* XCALLAPI __xthread_wrapper(void *arg) {
	__xthread_t *data = (__xthread_t *)arg;
	data->result = data->lpStartAddress(data->lpParameter);
	return NULL;
}
#endif

XSTDAPI int XCALLAPI xmutex_init(xmutex_t *m) {
#ifdef _WIN32
	InitializeCriticalSection(m);
	return 0;
#else
	return pthread_mutex_init(m, NULL);
#endif
}

XSTDAPI int XCALLAPI recursive_xmutex_init(xmutex_t *m) {
#ifdef _WIN32
    InitializeCriticalSection(m);
    return 0;
#else
	pthread_mutexattr_t attr;
	int ret = pthread_mutexattr_init(&attr);
	if (ret != 0) return ret;
	ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	if (ret == 0) ret = pthread_mutex_init(m, &attr);
	pthread_mutexattr_destroy(&attr);
	return ret;
#endif
}

XSTDAPI void XCALLAPI xmutex_lock(xmutex_t *m) {
#ifdef _WIN32
	EnterCriticalSection(m);
#else
	pthread_mutex_lock(m);
#endif
}

XSTDAPI xthread_result_t XCALLAPI xmutex_trylock(xmutex_t *mutex) {
#ifdef _WIN32
	return TryEnterCriticalSection(mutex) ? 0 : ERROR_BUSY;
#else
	return pthread_mutex_trylock(mutex);
#endif
}

XSTDAPI void XCALLAPI xmutex_unlock(xmutex_t *m) {
#ifdef _WIN32
	LeaveCriticalSection(m);
#else
	pthread_mutex_unlock(m);
#endif
}

XSTDAPI void XCALLAPI xmutex_destroy(xmutex_t *m) {
#ifdef _WIN32
	DeleteCriticalSection(m);
#else
	pthread_mutex_destroy(m);
#endif
}

XSTDAPI xthread_t XCALLAPI xthread_self(void) {
#ifdef _WIN32
	return OpenThread(THREAD_ALL_ACCESS,FALSE,GetCurrentThreadId());
#else
	return pthread_self();
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_create(xthread_t *thread,void *(XCALLAPI *start_routine)(void *),void *arg) {
#ifdef _WIN32
	__xthread_t *data = (__xthread_t*)xmalloc(sizeof(__xthread_t));
	if (!data) return (xthread_result_t)-1;
	data->lpStartAddress = start_routine;
	data->lpParameter = arg;
	data->result = NULL;
	HANDLE h = CreateThread(NULL, 0, __xthread_wrapper, data, 0, NULL);
	if (!h) {
		xfree(data);
		return GetLastError();
	}
	*thread = h;
	return (xthread_result_t)0;
#else
	return pthread_create(thread, NULL, start_routine, arg);
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_createw(xthread_t* thread,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter) {
#ifdef _WIN32
	HANDLE h = CreateThread(NULL, 0, lpStartAddress, lpParameter, 0, NULL);
	if (!h) return (xthread_result_t)GetLastError();
	*thread = h;
	return (xthread_result_t)0;
#else
	__xthread_t* data = (__xthread_t*)xmalloc(sizeof(*data));
	if (!data) return (xthread_result_t)-1;
	data->lpStartAddress = lpStartAddress;
	data->lpParameter = lpParameter;
	data->result = 0;
	int rc = pthread_create(thread, NULL, __xthread_wrapper, data);
	if (rc != 0) {
		xfree(data);
		return (xthread_result_t)rc;
	}
	return (xthread_result_t)0;
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_join(xthread_t thread, void **retval) {
#ifdef _WIN32
	DWORD wait_res = WaitForSingleObject(thread, INFINITE);
	if (wait_res != WAIT_OBJECT_0) return (xthread_result_t)(wait_res == WAIT_FAILED ? GetLastError() : ERROR_GEN_FAILURE);
	DWORD exit_code;
	if (!GetExitCodeThread(thread, &exit_code)) return (xthread_result_t)GetLastError();
	if (retval != NULL) *retval = NULL;
	return 0;
#else
	return pthread_join(thread, retval);
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_detach(xthread_t thread) {
#ifdef _WIN32
	if (!CloseHandle(thread)) return GetLastError();
	return 0;
#else
	return pthread_detach(thread);
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_equal(xthread_t t1, xthread_t t2) {
#ifdef _WIN32
	return GetThreadId(t1) == GetThreadId(t2);
#else
	return pthread_equal(t1,t2);
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_cancel(xthread_t th) {
#ifdef _WIN32
	return ENOSYS;
#elif defined(__ANDROID__)
	return pthread_kill(th,SIGUSR1);
#else
	return pthread_cancel(th);
#endif
}

XSTDAPI xthread_result_t XCALLAPI xthread_setcanceltype(int type, int *oldtype) {
#ifdef _WIN32
	static xthread_local int cancel_state = XTHREAD_CANCEL_ENABLE;
	if (oldtype) *oldtype = cancel_state;
	cancel_state = type;
	return 0;
#elif defined(__ANDROID__)
	static xthread_local int cancel_type = XTHREAD_CANCEL_ENABLE;
	if (oldtype) *oldtype = cancel_type;
	cancel_type = type;
	if (type == XTHREAD_CANCEL_ENABLE) ENABLE_SIGNAL(SIGUSR1);
	else if (type == XTHREAD_CANCEL_DISABLE) DISABLE_SIGNAL(SIGUSR1);
	return 0;
#else
	return pthread_setcanceltype(type, oldtype);
#endif
}

XSTDAPI int XCALLAPI xthread_setcancelstate(int state, int *oldstate) {
	static xthread_local int cancel_state = XTHREAD_CANCEL_ENABLE; // per-thread
#ifdef __ANDROID__
	if (oldstate != NULL) *(int*)oldstate = thread_cancel_state;
	if (state == XTHREAD_CANCEL_ENABLE) {
		thread_cancel_state = XTHREAD_CANCEL_ENABLE;
		ENABLE_SIGNAL(SIGUSR1);
	} else if (state == XTHREAD_CANCEL_DISABLE) {
		thread_cancel_state = XTHREAD_CANCEL_DISABLE;
		DISABLE_SIGNAL(SIGUSR1);
	}
#elif defined (_WIN32)
	if (oldstate) *oldstate = cancel_state;
	cancel_state = state;
	return 0;
#else
	return pthread_setcancelstate(state,oldstate);
#endif
}

XSTDAPI void XCALLAPI xthread_testcancel(void) {
#ifdef _WIN32
	return;
#else
	pthread_testcancel();
#endif
}

#ifdef __ANDROID__
#undef SETUP_SIG_HANDLER
#undef ENABLE_SIGNAL
#undef DISABLE_SIGNAL
#endif

#ifdef __cplusplus
}
#endif

#endif