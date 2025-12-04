# **XSTRING — Formatted Narrow-String Length Utilities**

*A small portability layer for computing formatted string lengths without output*
**Author:** MrR736
**License:** GPL-3.0+

---

## Overview

`xstring.h` provides two lightweight helper functions:

* **`vxstrlen()`** — length of formatted output using a `va_list`
* **`xstrlen()`** — convenience variadic wrapper

These functions behave like `snprintf(NULL, 0, ...)` but return only the length of the formatted string, with no allocation or output performed.

They provide a portable interface that:

* Uses `_vscprintf()` on Windows (reliable length probe)
* Uses `vsnprintf(NULL,0,...)` on POSIX

---

# 1. Function Summary

```c
size_t vxstrlen(const char *restrict fmt, va_list ap);
size_t xstrlen(const char *restrict fmt, ...);
```

Both compute the length the formatted output *would* require, excluding the terminating `'\0'`.

Return:

* **length** on success
* **0** on failure

---

# 2. Detailed Function Behavior

---

## `vxstrlen()`

### Windows

Uses:

```c
_vscprintf(fmt, ap)
```

which returns the number of characters needed for formatted output (excluding `\0`).

### POSIX

Uses:

```c
vsnprintf(NULL, 0, fmt, ap)
```

which returns:

* required byte count (excluding `\0`)
* a negative value on error

### Common behavior

* Returns `0` if the format evaluation fails
* Never allocates a buffer
* Does not write output

---

## `xstrlen()`

Simple convenience wrapper:

```c
va_list ap;
va_start(ap, fmt);
len = vxstrlen(fmt, ap);
va_end(ap);
```

Equivalent to:

```c
strlen(sprintf(fmt, ...));
```

But safe and side-effect-free.

---

# 3. Error Handling

| Condition                | Result          |
| ------------------------ | --------------- |
| Format OK                | Length returned |
| Formatting error         | Returns 0       |
| Windows internal failure | Returns 0       |
| `fmt == NULL`            | Returns 0       |

No errno is set — this mirrors system behavior for `_vscprintf` and `vsnprintf`.

---

# 4. Portability Notes

* `_vscprintf()` (Windows) accurately reports output length
* `vsnprintf(NULL,0,...)` is supported by all modern POSIX systems (C99+)
* No fallback probing or dynamic allocation is needed
* Works identically on:

  * Linux, BSD, macOS
  * Windows (MSVCRT)
  * MinGW / MSYS2

---

# 5. Example Usage

### Allocate exactly enough buffer space

```c
size_t len = xstrlen("%s: %d", "count", 42);
char *buf = malloc(len + 1);
sprintf(buf, "%s: %d", "count", 42);
```

### Variadic wrapper using vxstrlen()

```c
size_t format_and_measure(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    size_t len = vxstrlen(fmt, ap);
    va_end(ap);
    return len;
}
```

---

# 6. License

This library is distributed under the **GNU GPL v3 or later**.
