# **XWCHAR — Extended Wide-Character & Path Utilities**

*A portability layer for wide-character formatting, string handling, and path utilities*
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

* Extracting the filename component from a wide-character path:

  * **`xwbasename()`** — returns a pointer to the final path component

The formatted-length utilities behave similarly to `vswprintf()` / `swprintf()` when calculating the required output size, but do not retain formatted output.

Portable across:

* Windows (`_vscwprintf()`)
* POSIX systems supporting `vswprintf()`
* Older or non-POSIX systems through the library's fallback probing implementation

`xwbasename()` recognizes both Windows (`\`) and POSIX (`/`) path separators.

---

## 1. Function Summary

```c
size_t vxwcslen(const wchar_t *restrict fmt, va_list ap);
size_t xwcslen(const wchar_t *restrict fmt, ...);
int xwcscmp(const wchar_t *__s1, const wchar_t **__s2, size_t n);
wchar_t *xwcscmb(const wchar_t *s1, const wchar_t *s2);
const wchar_t *xwbasename(const wchar_t *path);
```

| Function     | Purpose                                                                            |
| ------------ | ---------------------------------------------------------------------------------- |
| `vxwcslen`   | Returns the number of wide characters required by formatted output using `va_list` |
| `xwcslen`    | Variadic wrapper around `vxwcslen`                                                 |
| `xwcscmp`    | Checks whether a wide string matches an entry in a wide-string array               |
| `xwcscmb`    | Concatenates two wide strings into a newly allocated buffer                        |
| `xwbasename` | Returns the final component of a wide-character filesystem path                    |

---

## 2. Detailed Function Behavior

### `vxwcslen()`

`vxwcslen()` calculates the number of wide characters required to represent formatted output without retaining the formatted result.

```c
size_t vxwcslen(const wchar_t *restrict fmt, va_list ap);
```

#### Windows

Uses:

```c
_vscwprintf(fmt, ap)
```

to determine the required number of wide characters, excluding the terminating `L'\0'`.

#### POSIX

The implementation uses wide-character formatting facilities to determine the required length.

Where direct length probing is unavailable or unsupported, the implementation can use a dynamically resized temporary buffer:

1. Start with an initial `wchar_t` buffer.
2. Attempt formatting.
3. Increase the buffer when necessary.
4. Continue until formatting succeeds or an error/size limit is reached.
5. Release the temporary buffer.

The returned length excludes the terminating NUL character.

---

### `xwcslen()`

`xwcslen()` provides the variadic interface to `vxwcslen()`.

```c
size_t xwcslen(const wchar_t *restrict fmt, ...);
```

Conceptually:

```c
va_list ap;

va_start(ap, fmt);
len = vxwcslen(fmt, ap);
va_end(ap);
```

Example:

```c
size_t len = xwcslen(L"%ls = %d", L"count", 123);
```

The result is the number of wide characters required by the formatted string.

---

### `xwcscmp()`

`xwcscmp()` performs a null-safe search for a wide string within an array of wide strings.

```c
int xwcscmp(
    const wchar_t *__s1,
    const wchar_t **__s2,
    size_t n
);
```

It returns:

* `1` — a matching string was found
* `0` — no matching string was found

#### Parameters

| Parameter | Type               | Description                     |
| --------- | ------------------ | ------------------------------- |
| `__s1`    | `const wchar_t *`  | Wide string to search for       |
| `__s2`    | `const wchar_t **` | Array of wide strings           |
| `n`       | `size_t`           | Number of elements in the array |

`NULL` values are handled explicitly.

If both the searched value and an array element are `NULL`, they are considered a match.

Example:

```c
const wchar_t *arr[] = {
    L"foo",
    L"bar",
    NULL
};

xwcscmp(L"bar", arr, 3);  // 1
xwcscmp(L"test", arr, 3); // 0
```

A `NULL` element can also be matched:

```c
const wchar_t *arr[] = {
    NULL,
    L"foo"
};

xwcscmp(NULL, arr, 2); // 1
```

---

### `xwcscmb()`

`xwcscmb()` concatenates two wide-character strings into a newly allocated buffer.

```c
wchar_t *xwcscmb(
    const wchar_t *s1,
    const wchar_t *s2
);
```

The returned buffer contains:

```text
s1 + s2 + L'\0'
```

The caller owns the returned allocation and must release it with:

```c
free()
```

#### Parameters

| Parameter | Type              | Description                       |
| --------- | ----------------- | --------------------------------- |
| `s1`      | `const wchar_t *` | First wide string; may be `NULL`  |
| `s2`      | `const wchar_t *` | Second wide string; may be `NULL` |

#### Behavior

* Both `s1` and `s2` are `NULL` → returns `NULL`
* Only `s1` is `NULL` → duplicates `s2`
* Only `s2` is `NULL` → duplicates `s1`
* Both are non-`NULL` → concatenates them
* Allocation failure → returns `NULL`

The input strings are never modified.

---

### `xwbasename()`

`xwbasename()` extracts the final component of a wide-character filesystem path.

```c
const wchar_t *xwbasename(const wchar_t *path);
```

Implementation:

```c
XSTDAPI const wchar_t *XCALLAPI xwbasename(const wchar_t *path)
{
    const wchar_t *p1 = wcsrchr(path, L'/');
    const wchar_t *p2 = wcsrchr(path, L'\\');
    const wchar_t *p = p1 > p2 ? p1 : p2;

    return p ? p + 1 : path;
}
```

The function recognizes both:

```text
/
\
```

as path separators.

This allows the same API to process Windows and POSIX-style paths.

#### POSIX example

```c
xwbasename(L"/home/mrr/project/main.c");
```

returns:

```text
main.c
```

#### Windows example

```c
xwbasename(L"C:\\Users\\MrR\\project\\main.c");
```

returns:

```text
main.c
```

#### Mixed separators

```c
xwbasename(L"C:\\Users/MrR\\project/src/main.c");
```

returns:

```text
main.c
```

#### Path without a separator

```c
xwbasename(L"main.c");
```

returns:

```text
main.c
```

---

## 3. `xwbasename()` Memory Semantics

`xwbasename()` does **not allocate or copy** the returned filename.

The returned pointer refers directly to the original `path` string.

For example:

```c
const wchar_t *path = L"C:\\project\\main.c";
const wchar_t *name = xwbasename(path);
```

`name` points into `path`.

Therefore:

* Do not call `free()` on the returned pointer.
* The returned pointer is only valid while `path` remains valid.
* The returned string should be treated as read-only.
* Modifying the original path can invalidate or alter the returned basename.

This makes `xwbasename()` a zero-allocation helper.

---

## 4. Error Handling

| Condition                                    |                  Return | `errno`                  |
| -------------------------------------------- | ----------------------: | ------------------------ |
| Formatting succeeds                          |         Required length | Unchanged                |
| Allocation failure during fallback           |                     `0` | `ENOMEM`                 |
| Required length exceeds implementation limit |                     `0` | `EOVERFLOW`              |
| Windows `_vscwprintf()` failure              |                     `0` | Implementation-dependent |
| `xwcscmp()` finds a match                    |                     `1` | Unchanged                |
| `xwcscmp()` finds no match                   |                     `0` | Unchanged                |
| `xwcscmb()` allocation failure               |                  `NULL` | Implementation-dependent |
| `xwbasename()` has a separator               |     Pointer to basename | Unchanged                |
| `xwbasename()` has no separator              | Original `path` pointer | Unchanged                |

Unless explicitly documented otherwise, `path` should point to a valid NUL-terminated wide-character string when passed to `xwbasename()`.

---

## 5. Portability Notes

### Wide formatting

Windows uses:

```c
_vscwprintf()
```

for direct wide-format length calculation.

POSIX-compatible implementations use the available `vswprintf()` behavior or the library's fallback buffer-probing mechanism.

### Wide path handling

`xwbasename()` does not depend on the host operating system's native path separator.

Both:

```text
/
```

and:

```text
\
```

are recognized.

For example, these are both valid:

```c
xwbasename(L"/usr/local/bin/program");
xwbasename(L"C:\\Program Files\\program.exe");
```

This is useful when processing paths originating from different platforms.

---

## 6. Example Usage

### Wide formatted length

```c
size_t len = xwcslen(
    L"%ls = %d",
    L"count",
    123
);

wchar_t *buf =
    malloc((len + 1) * sizeof(wchar_t));

if (buf) {
    swprintf(
        buf,
        len + 1,
        L"%ls = %d",
        L"count",
        123
    );

    wprintf(L"%ls\n", buf);
    free(buf);
}
```

---

### Using `vxwcslen()`

```c
size_t format_and_measure(
    const wchar_t *fmt,
    ...
) {
    va_list ap;

    va_start(ap, fmt);

    size_t len = vxwcslen(fmt, ap);

    va_end(ap);

    return len;
}
```

---

### Wide string array comparison

```c
const wchar_t *arr[] = {
    L"foo",
    L"bar",
    NULL
};

int result = xwcscmp(
    L"bar",
    arr,
    3
);
```

`result` is `1`.

---

### Wide string concatenation

```c
wchar_t *s = xwcscmb(
    L"wide ",
    L"string"
);

if (s) {
    wprintf(L"%ls\n", s);
    free(s);
}
```

Output:

```text
wide string
```

---

### Extracting a wide filename

```c
const wchar_t *path =
    L"C:\\Users\\MrR\\Projects\\libxc\\src\\xwchar.c";

wprintf(
    L"Filename: %ls\n",
    xwbasename(path)
);
```

Output:

```text
Filename: xwchar.c
```

---

### Wide source-file logging

`xwbasename()` is also useful when diagnostic code needs to display only the filename instead of the complete source path:

```c
wprintf(
    L"%ls:%d\n",
    xwbasename(L"C:\\project\\src\\main.c"),
    42
);
```

Output:

```text
main.c:42
```

---

## 7. Memory Ownership

| Function       | Allocates? | Caller must `free()`? |
| -------------- | ---------: | --------------------: |
| `vxwcslen()`   |         No |                    No |
| `xwcslen()`    |         No |                    No |
| `xwcscmp()`    |         No |                    No |
| `xwcscmb()`    |        Yes |                   Yes |
| `xwbasename()` |         No |                    No |

`xwbasename()` is specifically a non-owning view into the supplied path.

---

## 8. Relationship to Standard C Functions

| XWCHAR         | Related standard/platform functionality |
| -------------- | --------------------------------------- |
| `vxwcslen()`   | `_vscwprintf()` / `vswprintf()`         |
| `xwcslen()`    | `swprintf()`-style formatting           |
| `xwcscmp()`    | `wcscmp()`                              |
| `xwcscmb()`    | `malloc()` + wide-string operations     |
| `xwbasename()` | `wcsrchr()`                             |

`xwbasename()` differs from platform-specific basename implementations by explicitly recognizing both Windows and POSIX path separators.

---

## 9. License

This library is distributed under the **GNU General Public License v3 or later**.
