/**
 * xmap.h: Extern Map Library
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

#ifndef __XMAP_H__
#define __XMAP_H__

#include "xstdlib.h"
#include "xmemory.h"
#include "xthread.h"

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

	xmutex_t mutex;
	int useMutex;
} xmap_t;

/* Types for `void *' pointers.  */
#define xmap_intptr(p)		((intptr_t)(p))
#define xmap_uintptr(p)		((uintptr_t)(p))
#define sxmap_c_cast(t,p)	((t)(intptr_t)(p))
#define xmap_c_cast(t,p)	((t)(uintptr_t)(p))

#ifdef __cplusplus /* C++ versions: pointer casts */
#define sxmap_cast(t,p)		(static_cast<t>((p)))
#define xmap_cast(t,p)		(reinterpret_cast<t>((p)))
#define xmap_deref(t,p)		(*reinterpret_cast<t *>((p)))
#else /* C versions: forward the arguments! */
#define sxmap_cast(t,p)		sxmap_c_cast(t,p)
#define xmap_cast(t,p)		xmap_c_cast(t,p)
#define xmap_deref(t,p)		(*(t *)(p))
#endif

#define xmap_intget(t,xm,i) xmap_deref(t,xmap_get(xm,i))

#define xmap_lock(xm) if (xm->useMutex) xmutex_lock(&xm->mutex)
#define xmap_unlock(xm) if (xm->useMutex) xmutex_unlock(&xm->mutex)

# ifdef XMAP_UNSAFE
#define XMAP_ASSUME(x) __builtin_assume(x)
#else
#define XMAP_ASSUME(x) do { if (!(x)) abort(); } while(0)
#endif

#define XMAP_MUTEX_FALSE 0
#define XMAP_MUTEX_TRUE  1

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize */
XSTDAPI void XCALLAPI xmap_init_mutex(xmap_t *xm,int useMutex) {
	xm->map = NULL;
	xm->str = NULL;
	xm->wstr = NULL;
	xm->count = 0;
	xm->capacity = 0;
	xm->cstr = 0;
	xm->cstr_capacity = 0;
	xm->cwstr = 0;
	xm->cwstr_capacity = 0;
	if (useMutex) {
		int rc = xmutex_init(&xm->mutex);
		if (rc != 0) {
			xm->useMutex = 0;
			errno = rc;
			return;
		}
		xm->useMutex = 1;
	} else {
		xm->useMutex = 0;
	}
}

XSTDAPI void XCALLAPI xmap_init(xmap_t *xm) {
	xmap_init_mutex(xm,XMAP_MUTEX_FALSE);
}

/*
 * Helper: ensure map capacity (holds/assumes caller has not locked mutex:
 * this routine will not lock. Returns 1 on success, 0 on failure).
 * It updates xm->map and xm->capacity atomically (no mutex) — caller
 * MUST hold the mutex if concurrent use is possible.
*/
XSTDAPI int XCALLAPI xmap_ensure_capacity_nolock(xmap_t *xm, size_t mincap) {
	if (xm->capacity >= mincap) return 1;
	size_t newcap = (xm->capacity == 0) ? 4 : xm->capacity;
	while (newcap < mincap) {
		if (newcap > SIZE_MAX / 2) {
			errno = ENOMEM;
			return 0;
		}
		newcap *= 2;
	}
	void **tmp = (void **)xrealloc(xm->map, newcap * sizeof(void *));
	if (!tmp) {
		errno = ENOMEM;
		return 0;
	}
	/* initialize new slots to NULL (helps later logic) */
	for (size_t i = xm->capacity; i < newcap; ++i) tmp[i] = NULL;
	xm->map = tmp;
	xm->capacity = newcap;
	return 1;
}

/* Helpers for str/wstr arrays (caller must hold mutex) */
XSTDAPI int XCALLAPI xmap_ensure_str_capacity_locked(xmap_t *xm, size_t mincap) {
	if (xm->cstr_capacity >= mincap) return 1;
	size_t newcap = (xm->cstr_capacity == 0) ? 4 : xm->cstr_capacity;
	while (newcap < mincap) {
		if (newcap > SIZE_MAX / 2) {
			errno = ENOMEM;
			return 0;
		}
		newcap *= 2;
	}
	size_t *tmp = (size_t *)xrealloc(xm->str, newcap * sizeof(size_t));
	if (!tmp) {
		errno = ENOMEM;
		return 0;
	}
	xm->str = tmp;
	xm->cstr_capacity = newcap;
	return 1;
}

XSTDAPI int XCALLAPI xmap_ensure_wstr_capacity_locked(xmap_t *xm, size_t mincap) {
	if (xm->cwstr_capacity >= mincap) return 1;
	size_t newcap = (xm->cwstr_capacity == 0) ? 4 : xm->cwstr_capacity;
	while (newcap < mincap) {
		if (newcap > SIZE_MAX / 2) {
			errno = ENOMEM;
			return 0;
		}
		newcap *= 2;
	}
	size_t *tmp = (size_t *)xrealloc(xm->wstr, newcap * sizeof(size_t));
	if (!tmp) {
		errno = ENOMEM;
		return 0;
	}
	xm->wstr = tmp;
	xm->cwstr_capacity = newcap;
	return 1;
}

/* Exists? (thread-safe) */
XSTDAPI int XCALLAPI xmap_exists(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	int exists = (i < xm->count && xm->map[i] != NULL);
	xmap_unlock(xm);
	return exists;
}

/* String/wstring existence by string-index (thread-safe) */
XSTDAPI int XCALLAPI xmap_strexists(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cstr) {
		xmap_unlock(xm);
		return 0;
	}
	size_t ret = xm->str[i];
	int exists = (ret < xm->count && xm->map[ret] != NULL);
	xmap_unlock(xm);
	return exists;
}

XSTDAPI int XCALLAPI xmap_wcsexists(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cwstr) {
		xmap_unlock(xm);
		return 0;
	}
	size_t ret = xm->wstr[i];
	int exists = (ret < xm->count && xm->map[ret] != NULL);
	xmap_unlock(xm);
	return exists;
}

/* Insert data pointer (thread-safe) */
XSTDAPI void XCALLAPI xmap_insert(xmap_t *xm, void *data) {
	xmap_lock(xm);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		/* errno set by ensure; leave map unchanged */
		xmap_unlock(xm);
		return;
	}
	xm->map[xm->count++] = data;
	xmap_unlock(xm);
}

XSTDAPI int XCALLAPI xmap_insert_safe(xmap_t *xm, void *data) {
	xmap_lock(xm);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		xmap_unlock(xm);
		return 0;
	}
	xm->map[xm->count++] = data;
	xmap_unlock(xm);
	return 1;
}

/* String Insert data pointer (thread-safe) */
XSTDAPI void XCALLAPI xmap_strinsert(xmap_t *xm, const char* str) {
	if (!str) return;
	char* copy = xstrdup(str);
	if (!copy) {
		errno = ENOMEM;
		return;
	}

	xmap_lock(xm);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		xmap_unlock(xm);
		xfree(copy);
		return;
	}
	xm->map[xm->count] = copy;
	size_t map_index = xm->count;
	xm->count++;

	if (!xmap_ensure_str_capacity_locked(xm, xm->cstr + 1)) {
		/* cannot expand str index array: rollback insertion */
		xfree(copy);
		xm->count--;
		xm->map[map_index] = NULL;
		xmap_unlock(xm);
		return;
	}
	xm->str[xm->cstr++] = map_index;
	xmap_unlock(xm);
}

/* String getter (thread-safe) */
XSTDAPI char* XCALLAPI xmap_strget(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cstr) {
		xmap_unlock(xm);
		return NULL;
	}
	size_t ret = xm->str[i];
	if (ret >= xm->count) {
		xmap_unlock(xm);
		return NULL;
	}
	void *ptr = xm->map[ret];
	xmap_unlock(xm);
	if (!ptr) return NULL;
	return xmap_cast(char*, ptr);
}

/* Wchar Insert data pointer (thread-safe) */
XSTDAPI void XCALLAPI xmap_wcsinsert(xmap_t *xm, const wchar_t *str) {
	if (!str) return;
	wchar_t *copy = xwcsdup(str);
	if (!copy) {
		errno = ENOMEM;
		return;
	}

	xmap_lock(xm);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		xmap_unlock(xm);
		xfree(copy);
		return;
	}
	xm->map[xm->count] = copy;
	size_t map_index = xm->count;
	xm->count++;

	if (!xmap_ensure_wstr_capacity_locked(xm, xm->cwstr + 1)) {
		xfree(copy);
		xm->count--;
		xm->map[map_index] = NULL;
		xmap_unlock(xm);
		return;
	}
	xm->wstr[xm->cwstr++] = map_index;
	xmap_unlock(xm);
}

/* Wchar getter (thread-safe) */
XSTDAPI wchar_t* XCALLAPI xmap_wcsget(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cwstr) {
		xmap_unlock(xm);
		return NULL;
	}
	size_t ret = xm->wstr[i];
	if (ret >= xm->count) {
		xmap_unlock(xm);
		return NULL;
	}
	void *ptr = xm->map[ret];
	xmap_unlock(xm);
	if (!ptr) return NULL;
	return xmap_cast(wchar_t*, ptr);
}

/* Generic get (thread-safe) - avoids nested locking by checking directly */
XSTDAPI void* XCALLAPI xmap_get(xmap_t* xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->count) {
		xmap_unlock(xm);
		return NULL;
	}
	void *data = xm->map[i];
	xmap_unlock(xm);
	return data;
}

/* Erase entry and shift (thread-safe). Frees stored pointer. */
XSTDAPI void XCALLAPI xmap_erase(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->count) {
		xmap_unlock(xm);
		return;
	}
	if (!xm->map[i]) {
		xmap_unlock(xm);
		return;
	}

	xfree(xm->map[i]);

	/* shift map entries left */
	for (size_t j = i; j + 1 < xm->count; ++j) xm->map[j] = xm->map[j + 1];
	xm->map[xm->count - 1] = NULL;

	/* fix str index list */
	for (size_t k = 0; k < xm->cstr;) {
		if (xm->str[k] == i) {
			/* remove this mapping */
			for (size_t m = k; m + 1 < xm->cstr; ++m) xm->str[m] = xm->str[m + 1];
			xm->cstr--;
			/* don't increment k: new element at k must be checked */
			continue;
		}
		if (xm->str[k] > i) xm->str[k]--;
		k++;
	}

	/* fix wstr index list */
	for (size_t k = 0; k < xm->cwstr;) {
		if (xm->wstr[k] == i) {
			for (size_t m = k; m + 1 < xm->cwstr; ++m) xm->wstr[m] = xm->wstr[m + 1];
			xm->cwstr--;
			continue;
		}
		if (xm->wstr[k] > i) xm->wstr[k]--;
		k++;
	}

	xm->count--;
	xmap_unlock(xm);
}

/* Erase entry but do not shift map (thread-safe). Frees pointer and sets slot NULL. */
XSTDAPI void XCALLAPI xmap_erase_no_shift(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->count) {
		xmap_unlock(xm);
		return;
	}
	if (!xm->map[i]) {
		xmap_unlock(xm);
		return;
	}
	xfree(xm->map[i]);
	xm->map[i] = NULL;
	xmap_unlock(xm);
}

/* String erase by string-index (thread-safe): removes mapping and frees stored string, shifts arrays */
XSTDAPI void XCALLAPI xmap_strerase(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cstr) {
		xmap_unlock(xm);
		return;
	}
	size_t ret = xm->str[i];
	if (ret < xm->count && xm->map[ret]) {
		xfree(xm->map[ret]);
	}

	/* shift map entries starting at ret */
	for (size_t j = ret; j + 1 < xm->count; ++j) xm->map[j] = xm->map[j + 1];
	xm->map[xm->count - 1] = NULL;

	/* remove str index at i and shift left */
	for (size_t j = i; j + 1 < xm->cstr; ++j) xm->str[j] = xm->str[j + 1];
	xm->cstr--;

	/* Fix all str/wstr indices that referenced indices after ret */
	for (size_t k = 0; k < xm->cstr; ++k) if (xm->str[k] > ret) xm->str[k]--;
	for (size_t k = 0; k < xm->cwstr; ++k) if (xm->wstr[k] > ret) xm->wstr[k]--;

	xm->count--;
	xmap_unlock(xm);
}

/* String erase by string-index (thread-safe): removes mapping and frees stored string, shifts arrays */
XSTDAPI void XCALLAPI xmap_wcserase(xmap_t *xm, size_t i) {
	xmap_lock(xm);

	if (i >= xm->cwstr) {
		xmap_unlock(xm);
		return;
	}

	size_t ret = xm->wstr[i];

	/* Free map[ret] *before* shifting */
	if (ret < xm->count && xm->map[ret]) {
		xfree(xm->map[ret]);
		xm->map[ret] = NULL;
	}

	/* Shift map entries left */
	for (size_t j = ret; j + 1 < xm->count; ++j)
		xm->map[j] = xm->map[j + 1];
	xm->map[xm->count - 1] = NULL;

	/* Remove wstr entry and shift left */
	for (size_t j = i; j + 1 < xm->cwstr; ++j)
		xm->wstr[j] = xm->wstr[j + 1];
	xm->cwstr--;

	/* Decrement all references above ret */
	for (size_t k = 0; k < xm->cstr; ++k) if (xm->str[k] > ret) xm->str[k]--;
	for (size_t k = 0; k < xm->cwstr; ++k) if (xm->wstr[k] > ret) xm->wstr[k]--;

	xm->count--;
	xmap_unlock(xm);
}


/* String erase no shift (thread-safe): free stored string and clear map slot; keep str index list */
XSTDAPI void XCALLAPI xmap_strerase_no_shift(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cstr) {
		xmap_unlock(xm);
		return;
	}
	size_t ret = xm->str[i];
	if (ret < xm->count && xm->map[ret]) {
		xfree(xm->map[ret]);
		xm->map[ret] = NULL;
	}
	xmap_unlock(xm);
}

/* Wchar erase no shift (thread-safe) */
XSTDAPI void XCALLAPI xmap_wcserase_no_shift(xmap_t *xm, size_t i) {
	xmap_lock(xm);
	if (i >= xm->cwstr) {
		xmap_unlock(xm);
		return;
	}
	size_t ret = xm->wstr[i];
	if (ret < xm->count && xm->map[ret]) {
		xfree(xm->map[ret]);
		xm->map[ret] = NULL;
	}
	xmap_unlock(xm);
}

/* Destroy: free elements and arrays (thread-safe). After this call xm is unusable. */
XSTDAPI void XCALLAPI xmap_destroy(xmap_t *xm) {
	xmap_lock(xm);
	for (size_t i = 0; i < xm->count; ++i) {
		if (xm->map && xm->map[i]) xfree(xm->map[i]);
	}
	xfree(xm->map);
	xm->map = NULL;
	xm->count = 0;
	xm->capacity = 0;

	xfree(xm->str);
	xm->str = NULL;
	xm->cstr = 0;
	xm->cstr_capacity = 0;

	xfree(xm->wstr);
	xm->wstr = NULL;
	xm->cwstr = 0;
	xm->cwstr_capacity = 0;

	xmap_unlock(xm);
	if (xm->useMutex) xmutex_destroy(&xm->mutex);
	xm->useMutex = 0;
}

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

XSTDAPI void XCALLAPI xxmap_strinsert(xmap_t *xm, const std::string& str) {
	xmap_strinsert(xm, str.c_str());
}

XSTDAPI int XCALLAPI xxmap_strinsert_safe(xmap_t *xm, const std::string& str) {
	/* safe variant name preserved, routing to non-locking is intentionally omitted:
	   use xmap_strinsert which is thread-safe */
	xmap_strinsert(xm, str.c_str());
	return 1;
}

XSTDAPI std::string XCALLAPI xxmap_strget(xmap_t *xm, size_t i) {
	char *p = xmap_strget(xm,i);
	if (!p) return std::string();
	return std::string(p);
}

XSTDAPI int XCALLAPI xxmap_wcsinsert_safe(xmap_t *xm, const std::wstring& str) {
	/* safe variant name preserved, routing to non-locking is intentionally omitted:
	   use xmap_strinsert which is thread-safe */
	xmap_wcsinsert(xm, str.c_str());
	return 1;
}

XSTDAPI void XCALLAPI xxmap_wcsinsert(xmap_t *xm, const std::wstring& str) {
	xmap_wcsinsert(xm, str.c_str());
}

XSTDAPI std::wstring XCALLAPI xxmap_wcsget(xmap_t *xm, size_t i) {
	wchar_t *p = xmap_wcsget(xm,i);
	if (!p) return std::wstring();
	return std::wstring(p);
}

template<typename T>
XSTDAPI T* XCALLAPI xxmap_get(xmap_t* xm, size_t i) {
	return reinterpret_cast<T*>(xmap_get(xm, i)); /* calls C version */
}

template<typename T>
XSTDAPI T XCALLAPI xxmap_intget(xmap_t* xm, size_t i) {
	void* p = xmap_get(xm, i);
	if (!p) return T();
	return *reinterpret_cast<T*>(p);
}
#endif

#endif /* __XMAP_H__ */
