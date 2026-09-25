# **XWCTYPE — Extended Wide-Character All Wide Characters Utilities**

This is a C header file that provides utility functions for wide-character string manipulation, including functions to convert characters to lowercase, uppercase, and ASCII characters.

The functions `xtowlower`, `xtowupper`, and `xtowascii` take a wide-character string (`wchar_t*`) and perform the following operations:

1. **`xtowlower`**: Converts all wide characters in the string to lowercase.
2. **`xtowupper`**: Converts all wide characters in the string to uppercase.
3. **`xtowascii`**: Converts all wide characters in the string to their ASCII equivalents. Non-ASCII characters are replaced with a question mark (`?`).

Here are some key points regarding the code:

### Memory Management:

* The functions allocate new memory for the result string using `malloc`, ensuring the original string is not modified. The caller is responsible for freeing the allocated memory using `free()` once it is no longer needed to prevent memory leaks.

### Error Handling:

* If the input string is `NULL`, the functions set `errno` to `EINVAL` (Invalid Argument) and return `NULL`.
* If memory allocation fails (i.e., `malloc` returns `NULL`), the functions set `errno` to `ENOMEM` (Out of Memory) and return `NULL`.
* If the return value is `NULL`, users should always check for errors before using the returned string.

### Compatibility:

* The `#ifdef __cplusplus` ensures compatibility with C++ compilers by declaring the functions in `extern "C"` linkage to avoid name mangling.

### Potential Improvements/Considerations:

* **Memory Deallocation**: The functions allocate memory using `malloc`. The caller must ensure the allocated memory is freed after use, or memory leaks may occur.

* **Input Validation**: The functions check if `str` is `NULL`, but do not check if the string is empty. An empty string (`L""`) will still work properly since `wcslen` will return `0`, and no characters will be processed.

* **Unicode/Locale Handling**: The code uses `towlower` and `towupper`, which are typically locale-sensitive functions. This means the behavior may vary depending on the system's locale settings. Some characters may not convert as expected in certain locales (e.g., uppercase letters in Turkish).

### Example Usage:

```c
#include <stdlib.h>
#include "xwctype.h"

wchar_t* str = L"Hello World!";
wchar_t* lower_str = xtowlower(str);
wchar_t* upper_str = xtowupper(str);
wchar_t* ascii_str = xtowascii(str);

// Always check if the result is NULL before using the string
if (lower_str != NULL) {
    // Use the result
    free(lower_str);
}

if (upper_str != NULL) {
    free(upper_str);
}

if (ascii_str != NULL) {
    free(ascii_str);
}
```

---

## `xtowlower(wchar_t* str)`

Converts a wide-character string to lowercase.

**Parameters**:

* `str`: A wide-character string to be converted.

**Returns**:

* A new wide-character string with all characters converted to lowercase, or `NULL` if an error occurs.

**Errors**:

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

---

## `xtowupper(wchar_t* str)`

Converts a wide-character string to uppercase.

**Parameters**:

* `str`: A wide-character string to be converted.

**Returns**:

* A new wide-character string with all characters converted to uppercase, or `NULL` if an error occurs.

**Errors**:

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

---

## `xtowascii(wchar_t* str)`

Converts a wide-character string to ASCII characters, replacing non-ASCII characters with `?`.

**Parameters**:

* `str`: A wide-character string to be converted.

**Returns**:

* A new wide-character string containing only ASCII characters, or `NULL` if an error occurs.

**Errors**:

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

---

## **License**

This library is distributed under the **GNU GPL v3 or later**.
