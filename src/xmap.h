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

# ifdef XMAP_UNSAFE
#define XMAP_ASSUME(x) __builtin_assume(x)
#else
#define XMAP_ASSUME(x) do { if (!(x)) abort(); } while(0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize */
XSTDDEF_INLINE_API void xmap_init(xmap_t *xm) {
	xm->map = NULL;
	xm->str = NULL;
	xm->wstr = NULL;
	xm->count = 0;
	xm->capacity = 0;
	xm->cstr = 0;
	xm->cstr_capacity = 0;
	xm->cwstr = 0;
	xm->cwstr_capacity = 0;
	pthread_mutex_init(&xm->mutex, NULL);
}

/* Helper: ensure map capacity (holds/assumes caller has not locked mutex:
   this routine will not lock. Returns 1 on success, 0 on failure).
   It updates xm->map and xm->capacity atomically (no mutex) — caller
   MUST hold the mutex if concurrent use is possible.
*/
XSTDDEF_INLINE_API int xmap_ensure_capacity_nolock(xmap_t *xm, size_t mincap) {
	if (xm->capacity >= mincap) return 1;
	size_t newcap = (xm->capacity == 0) ? 4 : xm->capacity;
	while (newcap < mincap) newcap *= 2;
	void **tmp = (void **)realloc(xm->map, newcap * sizeof(void *));
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
XSTDDEF_INLINE_API int xmap_ensure_str_capacity_locked(xmap_t *xm, size_t mincap) {
	if (xm->cstr_capacity >= mincap) return 1;
	size_t newcap = (xm->cstr_capacity == 0) ? 4 : xm->cstr_capacity;
	while (newcap < mincap) newcap *= 2;
	size_t *tmp = (size_t *)realloc(xm->str, newcap * sizeof(size_t));
	if (!tmp) {
		errno = ENOMEM;
		return 0;
	}
	xm->str = tmp;
	xm->cstr_capacity = newcap;
	return 1;
}

XSTDDEF_INLINE_API int xmap_ensure_wstr_capacity_locked(xmap_t *xm, size_t mincap) {
	if (xm->cwstr_capacity >= mincap) return 1;
	size_t newcap = (xm->cwstr_capacity == 0) ? 4 : xm->cwstr_capacity;
	while (newcap < mincap) newcap *= 2;
	size_t *tmp = (size_t *)realloc(xm->wstr, newcap * sizeof(size_t));
	if (!tmp) {
		errno = ENOMEM;
		return 0;
	}
	xm->wstr = tmp;
	xm->cwstr_capacity = newcap;
	return 1;
}

/* Exists? (thread-safe) */
XSTDDEF_INLINE_API int xmap_exists(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	int exists = (i < xm->count && xm->map[i] != NULL);
	pthread_mutex_unlock(&xm->mutex);
	return exists;
}

/* String/wstring existence by string-index (thread-safe) */
XSTDDEF_INLINE_API int xmap_strexists(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cstr) {
		pthread_mutex_unlock(&xm->mutex);
		return 0;
	}
	size_t ret = xm->str[i];
	int exists = (ret < xm->count && xm->map[ret] != NULL);
	pthread_mutex_unlock(&xm->mutex);
	return exists;
}

XSTDDEF_INLINE_API int xmap_wcsexists(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cwstr) {
		pthread_mutex_unlock(&xm->mutex);
		return 0;
	}
	size_t ret = xm->wstr[i];
	int exists = (ret < xm->count && xm->map[ret] != NULL);
	pthread_mutex_unlock(&xm->mutex);
	return exists;
}

/* Insert data pointer (thread-safe) */
XSTDDEF_INLINE_API void xmap_insert(xmap_t *xm, void *data) {
	pthread_mutex_lock(&xm->mutex);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		/* errno set by ensure; leave map unchanged */
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	xm->map[xm->count++] = data;
	pthread_mutex_unlock(&xm->mutex);
}

/* Non-locking insert (caller must hold mutex) */
XSTDDEF_INLINE_API int xmap_insert_nolock(xmap_t *xm, void *data) {
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) return 0;
	xm->map[xm->count++] = data;
	return 1;
}

/* String Insert data pointer (thread-safe) */
XSTDDEF_INLINE_API void xmap_strinsert(xmap_t *xm, const char* str) {
	if (!str) return;
	char* copy = strdup(str);
	if (!copy) {
		errno = ENOMEM;
		return;
	}

	pthread_mutex_lock(&xm->mutex);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		pthread_mutex_unlock(&xm->mutex);
		free(copy);
		return;
	}
	xm->map[xm->count] = copy;
	size_t map_index = xm->count;
	xm->count++;

	if (!xmap_ensure_str_capacity_locked(xm, xm->cstr + 1)) {
		/* cannot expand str index array: rollback insertion */
		free(copy);
		xm->count--;
		xm->map[map_index] = NULL;
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	xm->str[xm->cstr++] = map_index;
	pthread_mutex_unlock(&xm->mutex);
}

/* Non-locking variant: caller must hold mutex */
XSTDDEF_INLINE_API int xmap_strinsert_nolock(xmap_t *xm, const char* str) {
	if (!str) return 0;
	char* copy = strdup(str);
	if (!copy) {
		errno = ENOMEM;
		return 0;
	}
	if (!xmap_insert_nolock(xm, copy)) {
		free(copy);
		return 0;
	}
	if (!xmap_ensure_str_capacity_locked(xm, xm->cstr + 1)) {
		/* rollback */
		xm->count--;
		free(copy);
		return 0;
	}
	xm->str[xm->cstr++] = xm->count - 1;
	return 1;
}

/* String getter (thread-safe) */
XSTDDEF_INLINE_API char* xmap_strget(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cstr) {
		pthread_mutex_unlock(&xm->mutex);
		return NULL;
	}
	size_t ret = xm->str[i];
	if (ret >= xm->count) {
		pthread_mutex_unlock(&xm->mutex);
		return NULL;
	}
	void *ptr = xm->map[ret];
	pthread_mutex_unlock(&xm->mutex);
	if (!ptr) return NULL;
	return xmap_cast(char*, ptr);
}

/* Wchar Insert data pointer (thread-safe) */
XSTDDEF_INLINE_API void xmap_wcsinsert(xmap_t *xm, const wchar_t *str) {
	if (!str) return;
	wchar_t *copy = wcsdup(str);
	if (!copy) {
		errno = ENOMEM;
		return;
	}

	pthread_mutex_lock(&xm->mutex);
	if (!xmap_ensure_capacity_nolock(xm, xm->count + 1)) {
		pthread_mutex_unlock(&xm->mutex);
		free(copy);
		return;
	}
	xm->map[xm->count] = copy;
	size_t map_index = xm->count;
	xm->count++;

	if (!xmap_ensure_wstr_capacity_locked(xm, xm->cwstr + 1)) {
		free(copy);
		xm->count--;
		xm->map[map_index] = NULL;
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	xm->wstr[xm->cwstr++] = map_index;
	pthread_mutex_unlock(&xm->mutex);
}

/* Wchar getter (thread-safe) */
XSTDDEF_INLINE_API wchar_t* xmap_wcsget(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cwstr) {
		pthread_mutex_unlock(&xm->mutex);
		return NULL;
	}
	size_t ret = xm->wstr[i];
	if (ret >= xm->count) {
		pthread_mutex_unlock(&xm->mutex);
		return NULL;
	}
	void *ptr = xm->map[ret];
	pthread_mutex_unlock(&xm->mutex);
	if (!ptr) return NULL;
	return xmap_cast(wchar_t*, ptr);
}

/* Generic get (thread-safe) - avoids nested locking by checking directly */
XSTDDEF_INLINE_API void* xmap_get(xmap_t* xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->count) {
		pthread_mutex_unlock(&xm->mutex);
		return NULL;
	}
	void *data = xm->map[i];
	pthread_mutex_unlock(&xm->mutex);
	return data;
}

/* Erase entry and shift (thread-safe). Frees stored pointer. */
XSTDDEF_INLINE_API void xmap_erase(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->count) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	if (!xm->map[i]) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}

	free(xm->map[i]);

	/* shift map entries left */
	for (size_t j = i; j + 1 < xm->count; ++j)
		xm->map[j] = xm->map[j + 1];
	xm->map[xm->count - 1] = NULL;

	/* fix str index list */
	for (size_t k = 0; k < xm->cstr;) {
		if (xm->str[k] == i) {
			/* remove this mapping */
			for (size_t m = k; m + 1 < xm->cstr; ++m)
				xm->str[m] = xm->str[m + 1];
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
			for (size_t m = k; m + 1 < xm->cwstr; ++m)
				xm->wstr[m] = xm->wstr[m + 1];
			xm->cwstr--;
			continue;
		}
		if (xm->wstr[k] > i) xm->wstr[k]--;
		k++;
	}

	xm->count--;
	pthread_mutex_unlock(&xm->mutex);
}

/* Erase entry but do not shift map (thread-safe). Frees pointer and sets slot NULL. */
XSTDDEF_INLINE_API void xmap_erase_no_shift(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->count) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	if (!xm->map[i]) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	free(xm->map[i]);
	xm->map[i] = NULL;
	pthread_mutex_unlock(&xm->mutex);
}

/* String erase by string-index (thread-safe): removes mapping and frees stored string, shifts arrays */
XSTDDEF_INLINE_API void xmap_strerase(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cstr) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	size_t ret = xm->str[i];
	if (ret < xm->count && xm->map[ret]) {
		free(xm->map[ret]);
	}
	/* shift map entries starting at ret */
	for (size_t j = ret; j + 1 < xm->count; ++j)
		xm->map[j] = xm->map[j + 1];
	xm->map[xm->count - 1] = NULL;

	/* remove str index at i and shift left */
	for (size_t j = i; j + 1 < xm->cstr; ++j)
		xm->str[j] = xm->str[j + 1];
	xm->cstr--;

	/* Fix all str/wstr indices that referenced indices after ret */
	for (size_t k = 0; k < xm->cstr; ++k)
		if (xm->str[k] > ret) xm->str[k]--;
	for (size_t k = 0; k < xm->cwstr; ++k)
		if (xm->wstr[k] > ret) xm->wstr[k]--;

	xm->count--;
	pthread_mutex_unlock(&xm->mutex);
}

/* String erase by string-index (thread-safe): removes mapping and frees stored string, shifts arrays */
XSTDDEF_INLINE_API void xmap_wcserase(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);

	if (i >= xm->cwstr) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}

	size_t ret = xm->wstr[i];

	/* Free map[ret] *before* shifting */
	if (ret < xm->count && xm->map[ret]) {
		free(xm->map[ret]);
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
	for (size_t k = 0; k < xm->cstr; ++k)
		if (xm->str[k] > ret)
			xm->str[k]--;

	for (size_t k = 0; k < xm->cwstr; ++k)
		if (xm->wstr[k] > ret)
			xm->wstr[k]--;

	xm->count--;
	pthread_mutex_unlock(&xm->mutex);
}


/* String erase no shift (thread-safe): free stored string and clear map slot; keep str index list */
XSTDDEF_INLINE_API void xmap_strerase_no_shift(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cstr) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	size_t ret = xm->str[i];
	if (ret < xm->count && xm->map[ret]) {
		free(xm->map[ret]);
		xm->map[ret] = NULL;
	}
	pthread_mutex_unlock(&xm->mutex);
}

/* Wchar erase no shift (thread-safe) */
XSTDDEF_INLINE_API void xmap_wcserase_no_shift(xmap_t *xm, size_t i) {
	pthread_mutex_lock(&xm->mutex);
	if (i >= xm->cwstr) {
		pthread_mutex_unlock(&xm->mutex);
		return;
	}
	size_t ret = xm->wstr[i];
	if (ret < xm->count && xm->map[ret]) {
		free(xm->map[ret]);
		xm->map[ret] = NULL;
	}
	pthread_mutex_unlock(&xm->mutex);
}

/* Destroy: free elements and arrays (thread-safe). After this call xm is unusable. */
XSTDDEF_INLINE_API void xmap_destroy(xmap_t *xm) {
	pthread_mutex_lock(&xm->mutex);
	for (size_t i = 0; i < xm->count; ++i) {
		if (xm->map && xm->map[i]) free(xm->map[i]);
	}
	free(xm->map);
	xm->map = NULL;
	xm->count = 0;
	xm->capacity = 0;

	free(xm->str);
	xm->str = NULL;
	xm->cstr = 0;
	xm->cstr_capacity = 0;

	free(xm->wstr);
	xm->wstr = NULL;
	xm->cwstr = 0;
	xm->cwstr_capacity = 0;

	pthread_mutex_unlock(&xm->mutex);
	pthread_mutex_destroy(&xm->mutex);
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

XSTDDEF_INLINE_API void xxmap_strinsert(xmap_t *xm, const std::string& str) {
	xmap_strinsert(xm, str.c_str());
}

XSTDDEF_INLINE_API int xxmap_strinsert_safe(xmap_t *xm, const std::string& str) {
	/* safe variant name preserved, routing to non-locking is intentionally omitted:
	   use xmap_strinsert which is thread-safe */
	xmap_strinsert(xm, str.c_str());
	return 1;
}

XSTDDEF_INLINE_API std::string xxmap_strget(xmap_t *xm, size_t i) {
	char *p = xmap_strget(xm,i);
	if (!p) return std::string();
	return std::string(p);
}

XSTDDEF_INLINE_API int xxmap_wcsinsert_safe(xmap_t *xm, const std::wstring& str) {
	/* safe variant name preserved, routing to non-locking is intentionally omitted:
	   use xmap_strinsert which is thread-safe */
	xmap_wcsinsert(xm, str.c_str());
	return 1;
}

XSTDDEF_INLINE_API void xxmap_wcsinsert(xmap_t *xm, const std::wstring& str) {
	xmap_wcsinsert(xm, str.c_str());
}

XSTDDEF_INLINE_API std::wstring xxmap_wcsget(xmap_t *xm, size_t i) {
	wchar_t *p = xmap_wcsget(xm,i);
	if (!p) return std::wstring();
	return std::wstring(p);
}

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
