# XMAP — Extern Map Library

*A thread-safe dynamic pointer map with indexed string/wstring support*
**Author:** MrR736
**License:** GNU GPLv3+

---

## Overview

`xmap` is a generic, thread-safe container implemented in C that stores `void*` pointers in a dynamically-sized array with additional indexed lists for:

* **char*** strings
* **wchar_t*** wide strings

It includes:

* Automatic memory management for inserted strings/wstrings
* Mutex-based thread safety
* Optional non-shifting erase operations
* Iterator support for C++ consumers

This document describes the data structures, API, and usage patterns.

---

# 1. Data Structure

```c
typedef struct {
    void **map;          // main storage array
    size_t count;        // number of entries
    size_t capacity;     // allocated capacity

    size_t *str;         // indices into map[] for char* entries
    size_t cstr;
    size_t cstr_capacity;

    size_t *wstr;        // indices into map[] for wchar_t* entries
    size_t cwstr;
    size_t cwstr_capacity;

    pthread_mutex_t mutex; // thread safety
} xmap_t;
```

---

# 2. Initialization & Destruction

### `void xmap_init(xmap_t *xm)`

Initializes all counters and allocates no memory. Initializes the mutex.

### `void xmap_destroy(xmap_t *xm)`

Frees all stored objects, the map array, string index lists, and destroys the mutex.
After this call the `xmap_t` must not be reused unless reinitialized.

---

# 3. Generic Pointer API

## Insert

### `void xmap_insert(xmap_t *xm, void *data)`

Thread-safe insertion. Does **not** copy the data — you must allocate it.

### `int xmap_insert_nolock(xmap_t *xm, void *data)`

Same as above but caller must hold the mutex.

---

## Get

### `void* xmap_get(xmap_t *xm, size_t i)`

Returns the raw pointer at index `i` or `NULL`.

---

## Exists

### `int xmap_exists(xmap_t *xm, size_t i)`

Returns 1 if entry at index `i` exists and is non-NULL.

---

## Erase

### `void xmap_erase(xmap_t *xm, size_t i)`

Thread-safe.
Frees the stored pointer and **shifts** all elements left.

This maintains contiguous order but causes index movement.

---

### `void xmap_erase_no_shift(xmap_t *xm, size_t i)`

Thread-safe.
Frees the stored pointer and sets the slot to NULL without shifting.

---

# 4. String API (`char*`)

Strings are **automatically duplicated** via `strdup()` on insert and freed on erase.

## Insert

### `void xmap_strinsert(xmap_t *xm, const char *str)`

Thread-safe. Inserts a copy of the string and records its index in `xm->str`.

### `int xmap_strinsert_nolock(xmap_t *xm, const char* str)`

Non-locking variant.

---

## Get

### `char* xmap_strget(xmap_t *xm, size_t i)`

Returns the `i`-th string (by string index), or `NULL`.

Note: `i` refers to the **string list**, *not* the main map index.

---

## Exists

### `int xmap_strexists(xmap_t *xm, size_t i)`

---

## Erase

### `void xmap_strerase(xmap_t *xm, size_t i)`

Removes the string indexed by `i` (string list index), frees memory, and shifts:

* map entries
* string index list
* wstring index list

---

### `void xmap_strerase_no_shift(xmap_t *xm, size_t i)`

Frees string and leaves map slot as `NULL` without shifting.

---

# 5. Wide-string API (`wchar_t*`)

Equivalent to char* API, but uses `wcsdup`.

### `void xmap_wcsinsert(xmap_t *xm, const wchar_t *str)`

### `wchar_t* xmap_wcsget(xmap_t *xm, size_t i)`

### `int xmap_wcsexists(xmap_t *xm, size_t i)`

### `void xmap_wcserase(xmap_t *xm, size_t i)`

### `void xmap_wcserase_no_shift(xmap_t *xm, size_t i)`

---

# 6. Internal Helper Functions

These are available but should only be used when manually controlling the mutex:

* `xmap_ensure_capacity_nolock`
* `xmap_ensure_str_capacity_locked`
* `xmap_ensure_wstr_capacity_locked`

These functions reallocate arrays and adjust capacities.

---

# 7. C++ Interface

C++ wrappers provide:

* Strongly-typed insertion/getting
* Iterators
* std::string / std::wstring convenience functions

## Iterators

```cpp
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
```

### `xmap_begin<T>(xmap_t*)`

### `xmap_end<T>(xmap_t*)`

Example:

```cpp
xmap_t xm;
xmap_init(&xm);

int* a = new int(5);
xmap_insert(&xm, a);

for (auto it = xmap_begin<int>(&xm); it != xmap_end<int>(&xm); ++it) {
    std::cout << *it << "\n";
}
```

---

## C++ string wrappers:

### `void xxmap_strinsert(xmap_t*, const std::string&)`

### `std::string xxmap_strget(xmap_t*, size_t)`

## Wide string wrappers:

### `void xxmap_wcsinsert(xmap_t*, const std::wstring&)`

### `std::wstring xxmap_wcsget(xmap_t*, size_t)`

## Typed pointer retrieval:

### `template<typename T> T* xxmap_get(xmap_t*, size_t)`

### `template<typename T> T xxmap_intget(xmap_t*, size_t)`

---

# 8. Memory Ownership Rules

| Operation        | Ownership                                              |
| ---------------- | ------------------------------------------------------ |
| `xmap_insert`    | User allocates, library frees on erase/destroy         |
| `xmap_strinsert` | Library allocates via `strdup`, frees on erase/destroy |
| `xmap_wcsinsert` | Library allocates via `wcsdup`, frees on erase/destroy |
| `xmap_destroy`   | Frees all stored objects + internal arrays             |

---

# 9. Thread Safety

All public API functions are thread-safe unless marked `_nolock`.

* Mutex must be held by caller for any `_nolock` function.
* Iterators in C++ **are not thread-safe** unless the user externally locks the map during iteration.

---

# 10. Example Usage

```c
xmap_t xm;
xmap_init(&xm);

xmap_strinsert(&xm, "hello");
xmap_strinsert(&xm, "world");

printf("%s\n", xmap_strget(&xm, 0)); // "hello"

xmap_destroy(&xm);
```

Wide string example:

```c
xmap_wcsinsert(&xm, L"wide text");
wchar_t *w = xmap_wcsget(&xm, 0);
```

---

# 11. Known Caveats

* `erase` operations that *shift* indices can invalidate saved indexes.
* Not a hash map — this is an indexed dynamic array.
* After `xmap_destroy()`, the mutex is invalid and the structure cannot be reused without re-init.

---

# 12. License

Released under the **GNU General Public License v3 or later**.
