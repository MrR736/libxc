/**
 * xmap.h: Extern Map Library
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

#ifndef __XMAP_H__
#define __XMAP_H__

#include "xstddef.h"
#include <pthread.h>

#ifdef __cplusplus
#include <string>
#endif

typedef struct {
    void **map;
    size_t count;
    size_t capacity;

    /* track string-list capacity separately to avoid conflation */
    size_t *str; /* indices into map for char* entries */
    size_t cstr;
    size_t cstr_capacity;

    size_t *wstr; /* indices into map for wchar_t* entries */
    size_t cwstr;
    size_t cwstr_capacity;

    pthread_mutex_t mutex;
} xmap_t;

/* Types for `void *' pointers.  */
#define xmap_intptr(p)      ((intptr_t)(p))
#define xmap_uintptr(p)     ((uintptr_t)(p))
#define sxmap_c_cast(t,p)   ((t)(intptr_t)(p))
#define xmap_c_cast(t,p)    ((t)(uintptr_t)(p))

#ifdef __cplusplus /* C++ versions: pointer casts */
#define sxmap_cast(t,p)     (static_cast<t>((p)))
#define xmap_cast(t,p)      (reinterpret_cast<t>((p)))
#define xmap_deref(t,p)     (*reinterpret_cast<t *>((p)))
#else /* C versions: forward the arguments! */
#define sxmap_cast(t,p)     sxmap_c_cast(t,p)
#define xmap_cast(t,p)      xmap_c_cast(t,p)
#define xmap_deref(t,p)     (*(t *)(p))
#endif

#define xmap_intget(t,xm,i) xmap_deref(t,xmap_get(xm,i))

#ifdef XMAP_UNSAFE
#define XMAP_ASSUME(x) __builtin_assume(x)
#else
#define XMAP_ASSUME(x) do { if (!(x)) abort(); } while(0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize */
XSTDDEF_IMPORT_API void xmap_init(xmap_t *xm);

/* Helper: ensure map capacity (holds/assumes caller has not locked mutex:
   this routine will not lock. Returns 1 on success, 0 on failure).
   It updates xm->map and xm->capacity atomically (no mutex) — caller
   MUST hold the mutex if concurrent use is possible.
*/
XSTDDEF_IMPORT_API int xmap_ensure_capacity_nolock(xmap_t *xm, size_t mincap);

/* Helpers for str/wstr arrays (caller must hold mutex) */
XSTDDEF_IMPORT_API int xmap_ensure_str_capacity_locked(xmap_t *xm, size_t mincap);

XSTDDEF_IMPORT_API int xmap_ensure_wstr_capacity_locked(xmap_t *xm, size_t mincap);

/* Exists? (thread-safe) */
XSTDDEF_IMPORT_API int xmap_exists(xmap_t *xm, size_t i);

/* String/wstring existence by string-index (thread-safe) */
XSTDDEF_IMPORT_API int xmap_strexists(xmap_t *xm, size_t i);

XSTDDEF_IMPORT_API int xmap_wcsexists(xmap_t *xm, size_t i);

/* Insert data pointer (thread-safe) */
XSTDDEF_IMPORT_API void xmap_insert(xmap_t *xm, void *data);

/* Non-locking insert (caller must hold mutex) */
XSTDDEF_IMPORT_API int xmap_insert_nolock(xmap_t *xm, void *data);

/* String Insert data pointer (thread-safe) */
XSTDDEF_IMPORT_API void xmap_strinsert(xmap_t *xm, const char* str);

/* Non-locking variant: caller must hold mutex */
XSTDDEF_IMPORT_API int xmap_strinsert_nolock(xmap_t *xm, const char* str);

/* String getter (thread-safe) */
XSTDDEF_IMPORT_API char* xmap_strget(xmap_t *xm, size_t i);

/* Wchar Insert data pointer (thread-safe) */
XSTDDEF_IMPORT_API void xmap_wcsinsert(xmap_t *xm, const wchar_t *str);

/* Wchar getter (thread-safe) */
XSTDDEF_IMPORT_API wchar_t* xmap_wcsget(xmap_t *xm, size_t i);

/* Generic get (thread-safe) - avoids nested locking by checking directly */
XSTDDEF_IMPORT_API void* xmap_get(xmap_t* xm, size_t i);

/* Erase entry and shift (thread-safe). Frees stored pointer. */
XSTDDEF_IMPORT_API void xmap_erase(xmap_t *xm, size_t i);

/* String erase by string-index (thread-safe): removes mapping and frees stored string, shifts arrays */
XSTDDEF_IMPORT_API void xmap_strerase(xmap_t *xm, size_t i);

/* String erase by string-index (thread-safe): removes mapping and frees stored string, shifts arrays */
XSTDDEF_IMPORT_API void xmap_wcserase(xmap_t *xm, size_t i);

/* Erase entry but do not shift map (thread-safe). Frees pointer and sets slot NULL. */
XSTDDEF_IMPORT_API void xmap_erase_no_shift(xmap_t *xm, size_t i);

/* String erase no shift (thread-safe): free stored string and clear map slot; keep str index list */
XSTDDEF_IMPORT_API void xmap_strerase_no_shift(xmap_t *xm, size_t i);

/* Wchar erase no shift (thread-safe) */
XSTDDEF_IMPORT_API void xmap_wcserase_no_shift(xmap_t *xm, size_t i);

/* Destroy: free elements and arrays (thread-safe). After this call xm is unusable. */
XSTDDEF_IMPORT_API void xmap_destroy(xmap_t *xm);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template<typename T>
class xmap_iterator {
public:
    xmap_iterator(xmap_t *map, size_t idx) : xm(map), index(idx) {}
    T& operator*() { return *reinterpret_cast<T*>(xm->map[index]); }
    xmap_iterator& operator++() { ++index; return *this; }
    bool operator!=(const xmap_iterator& other) const { return index != other.index; }

private:
    xmap_t *xm;
    size_t index;
};

template<typename T>
xmap_iterator<T> xmap_begin(xmap_t* xm) { return xmap_iterator<T>(xm, 0); }

template<typename T>
xmap_iterator<T> xmap_end(xmap_t* xm) { return xmap_iterator<T>(xm, xm->count); }

/* C++ convenience wrappers */

XSTDDEF_IMPORT_API void xxmap_strinsert(xmap_t *xm, const std::string& str);
XSTDDEF_IMPORT_API int xxmap_strinsert_safe(xmap_t *xm, const std::string& str);
XSTDDEF_IMPORT_API std::string xxmap_strget(xmap_t *xm, size_t i);

XSTDDEF_IMPORT_API void xxmap_wcsinsert(xmap_t *xm, const std::wstring& str);
XSTDDEF_IMPORT_API int xxmap_wcsinsert_safe(xmap_t *xm, const std::wstring& str);
XSTDDEF_IMPORT_API std::wstring xxmap_wcsget(xmap_t *xm, size_t i);

template<typename T>
T* xxmap_get(xmap_t* xm, size_t i) {
    return reinterpret_cast<T*>(xmap_get(xm, i)); /* calls C version */
}

template<typename T>
T xxmap_intget(xmap_t* xm, size_t i) {
    void* p = xmap_get(xm, i);
    if (!p) return T();
    return *reinterpret_cast<T*>(p);
}
#endif

#endif /* __XMAP_H__ */
