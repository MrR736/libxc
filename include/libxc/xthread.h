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

#ifdef _WIN32
typedef DWORD xthread_result_t;
typedef HANDLE xthread_t;
typedef CRITICAL_SECTION xmutex_t;

# define XTHREAD_CANCEL_ENABLE  1
# define XTHREAD_CANCEL_DISABLE 0
#else
# include <pthread.h>

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
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI xmutex_init(xmutex_t *m);

XSTDAPI int XCALLAPI recursive_xmutex_init(xmutex_t *m);

XSTDAPI void XCALLAPI xmutex_lock(xmutex_t *m);

XSTDAPI xthread_result_t XCALLAPI xmutex_trylock(xmutex_t *mutex);

XSTDAPI void XCALLAPI xmutex_unlock(xmutex_t *m);

XSTDAPI void XCALLAPI xmutex_destroy(xmutex_t *m);

XSTDAPI xthread_t XCALLAPI xthread_self(void);

XSTDAPI xthread_result_t XCALLAPI xthread_create(xthread_t *thread,void *(XCALLAPI *start_routine)(void *),void *arg);

XSTDAPI xthread_result_t XCALLAPI xthread_createw(xthread_t* thread,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter);

XSTDAPI xthread_result_t XCALLAPI xthread_join(xthread_t thread, void **retval);

XSTDAPI xthread_result_t XCALLAPI xthread_detach(xthread_t thread);

XSTDAPI xthread_result_t XCALLAPI xthread_equal(xthread_t t1, xthread_t t2);

XSTDAPI xthread_result_t XCALLAPI xthread_cancel(xthread_t th);

XSTDAPI xthread_result_t XCALLAPI xthread_setcanceltype(int type, int *oldtype);

XSTDAPI int XCALLAPI xthread_setcancelstate(int state, int *oldstate);

XSTDAPI void XCALLAPI xthread_testcancel(void);

#ifdef __cplusplus
}
#endif

#endif