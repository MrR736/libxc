# **XWCHAR — Extended Wide-Character Length Utilities**

*A portability layer for wide-character formatted length calculation*
**Author:** MrR736
**License:** GPL-3.0+

---

## Overview

`xwchar.h` provides two functions:

* **`vxwcslen()`** — Computes the length of a wide-formatted string using a `va_list`.
* **`xwcslen()`** — Wrapper using variadic arguments.

The API is comparable to `vsnprintf`/`snprintf` but for wide characters, returning only the *required length*, not writing the formatted string.

The implementation is portable between:

* POSIX systems that use `vswprintf(NULL,0,...)` support
* Older or non-POSIX systems where buffer probing must be performed
* Windows systems using `_vscwprintf()`

---

# 1. Function Summary

```c
size_t vxwcslen(const wchar_t *restrict fmt, va_list ap);
size_t xwcslen(const wchar_t *restrict fmt, ...);
```

Both functions return:

* the number of **wide characters** needed to format the string
* **0** on failure (also sets `errno` where appropriate)

Neither function writes output to a caller buffer.

---

# 2. Detailed Function Behavior

---

## `vxwcslen()`

### Windows behavior

On Windows, the implementation uses:

```c
_vscwprintf(fmt, ap)
```

which directly returns the number of wide characters needed (excluding NUL).

Failure → returns 0.

---

### POSIX behavior

POSIX allows probing:

```c
vswprintf(NULL, 0, fmt, ap)
```

If supported, the code takes this fast path.

If unsupported or returning a negative value, the implementation enters the **fallback expansion loop**:

1. Start with a buffer of 256 wchar_t.
2. Double the buffer until:

   * formatting succeeds, or
   * it reaches the safety limit (¼ of `SIZE_MAX`), or
   * memory allocation fails.

On failure:

* `errno = ENOMEM` if allocation failed
* `errno = EOVERFLOW` if bounds exceeded

The returned length does **not** include the terminating NUL.

---

## `xwcslen()`

A simple variadic wrapper:

```c
va_start(ap, fmt);
len = vxwcslen(fmt, ap);
va_end(ap);
```

Useful for quick one-shot formatting length determination:

```c
size_t needed = xwcslen(L"%ls %d", L"hello", 42);
```

---

# 3. Error Handling

| Condition                     | Return | errno                                  |
| ----------------------------- | ------ | -------------------------------------- |
| Success                       | length | unchanged                              |
| Allocation failure            | 0      | `ENOMEM`                               |
| Required length exceeds limit | 0      | `EOVERFLOW`                            |
| Windows `_vscwprintf` fails   | 0      | unchanged (Windows does not set errno) |

---

# 4. Portability Notes

* The Windows path is fast and reliable, using the platform’s native probing routine.
* The POSIX fallback ensures compatibility with libc versions where
  `vswprintf(NULL,0,...)` is unsupported (Solaris, older glibc).
* The safety limit ensures no undefined overflow when probing required size.

---

# 5. Example Usage

### Basic formatted length query

```c
size_t len = xwcslen(L"%ls = %d", L"count", 123);
wchar_t *buf = malloc((len + 1) * sizeof(wchar_t));
swprintf(buf, len + 1, L"%ls = %d", L"count", 123);
```

### Using the `va_list` version

```c
size_t format_and_measure(const wchar_t *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    size_t len = vxwcslen(fmt, ap);
    va_end(ap);
    return len;
}
```

---

# 6. License

This library is distributed under the **GNU GPL v3 or later**.
