# **XWCHAR — Extended Wide-Character Length Utilities**

*A portability layer for wide-character formatted length calculation*
**Author:** MrR736
**License:** GPL-3.0+

---

## Overview

`xwchar.h` provides helper functions for wide-character string handling:

* Measuring the length of wide-formatted strings **without producing output**:

  * **`vxwcslen()`** — computes length using a `va_list`
  * **`xwcslen()`** — variadic convenience wrapper

* Comparing wide strings against an array of wide strings:

  * **`xwcscmp()`** — boolean-style matcher

* Combining wide-character strings safely:

  * **`xwcscmb()`** — concatenates two wide strings into a newly allocated buffer

The formatted-length utilities behave similarly to `vswprintf()` /
`swprintf()` when called with a NULL buffer, but **do not write output**.

Portable across:

* Windows (`_vscwprintf()`)
* POSIX systems supporting `vswprintf(NULL, 0, ...)`
* Older or non-POSIX systems via fallback buffer probing

---

## 1. Function Summary

```c
size_t vxwcslen(const wchar_t *restrict fmt, va_list ap);
size_t xwcslen(const wchar_t *restrict fmt, ...);
int xwcscmp(const wchar_t * __s1, const wchar_t ** __s2, size_t n);
wchar_t *xwcscmb(const wchar_t *s1, const wchar_t *s2);
```

* `vxwcslen` / `xwcslen`: return the number of **wide characters** required to format a string.
* `xwcscmp`: returns `1` if `__s1` matches any element in `__s2`, `0` otherwise.

---

## 2. Detailed Function Behavior

### `vxwcslen()`

#### Windows

Uses `_vscwprintf(fmt, ap)` to return the number of wide characters required (excluding NUL).
Returns `0` on failure.

#### POSIX

Uses:

```c
vswprintf(NULL, 0, fmt, ap)
```

* If unsupported or negative → fallback allocation loop:

  1. Start with 256 `wchar_t` buffer
  2. Double until formatting succeeds, memory fails, or a safety limit (¼ of `SIZE_MAX`) is reached

* Failure conditions:

  * Allocation failure → `errno = ENOMEM`
  * Length exceeds limit → `errno = EOVERFLOW`

Return value excludes the terminating NUL.

---

### `xwcslen()`

Convenience wrapper:

```c
va_list ap;
va_start(ap, fmt);
len = vxwcslen(fmt, ap);
va_end(ap);
```

---

### `xwcscmp()`

**Null-safe** wide string matcher:

* Both `__s1` and an element of `__s2` NULL → match
* One NULL → skip
* Otherwise, compares with `wcscmp()`

Returns `1` if a match is found, `0` otherwise.

```c
const wchar_t *arr[] = {L"foo", L"bar", NULL};
int result = xwcscmp(L"bar", arr, 3); // returns 1
```

---

### `xwcscmb()`

Concatenates two wide-character C-style strings into a newly allocated
wide-character string.

The returned buffer is allocated using `malloc()` and must be freed by
the caller using `free()`.

#### Parameters

| Parameter | Type              | Description                        |
| --------- | ----------------- | ---------------------------------- |
| `s1`      | `const wchar_t *` | First string. May be `NULL`.       |
| `s2`      | `const wchar_t *` | Second string. May be `NULL`.      |

#### Behavior

* If both `s1` and `s2` are `NULL`, returns `NULL`
* If one argument is `NULL`, returns a duplicate of the other
* If both are non-`NULL`, returns a newly allocated string containing
  `s1` followed by `s2`
* On allocation failure, returns `NULL`

The function never modifies its input strings.

The allocation size is exactly:

---

## 3. Error Handling

| Condition                     | Return | errno                                  |
| ----------------------------- | ------ | -------------------------------------- |
| Success                       | length | unchanged                              |
| Allocation failure            | 0      | `ENOMEM`                               |
| Required length exceeds limit | 0      | `EOVERFLOW`                            |
| Windows `_vscwprintf` fails   | 0      | unchanged (Windows does not set errno) |

`xwcscmp()` returns `0` on no match or if array pointer is `NULL`.

---

## 4. Portability Notes

* Windows path uses `_vscwprintf()` — fast and reliable.
* POSIX path uses `vswprintf(NULL, 0, ...)` or fallback loop.
* Safe for older libc versions or non-POSIX systems.
* Safety limit prevents integer overflow when probing length.

---

## 5. Example Usage

### Wide formatted length

```c
size_t len = xwcslen(L"%ls = %d", L"count", 123);
wchar_t *buf = malloc((len + 1) * sizeof(wchar_t));
swprintf(buf, len + 1, L"%ls = %d", L"count", 123);
```

### Using `vxwcslen()`

```c
size_t format_and_measure(const wchar_t *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    size_t len = vxwcslen(fmt, ap);
    va_end(ap);
    return len;
}
```

### Wide string array comparison

```c
const wchar_t *arr[] = {L"foo", L"bar", NULL};
int result = xwcscmp(L"bar", arr, 3); // returns 1
```

### Wide string concatenation using `xwcscmb()`

```c
wchar_t *s = xwcscmb(L"wide", L"string");
if (s) {
    wprintf(L"%ls\n", s);
    free(s);
}
```

---

## 6. License

This library is distributed under the **GNU GPL v3 or later**.
