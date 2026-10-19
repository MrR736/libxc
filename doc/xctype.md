# **XCTYPE — Extended Character Type Utilities**

This is a C header file that provides utility functions for string manipulation. It includes functions to convert characters to lowercase, uppercase, and ASCII characters. The functions operate on null-terminated C strings (`char*`).

---

## **Functions Provided**

### `xtolower`

Converts a string to lowercase.

#### **Parameters:**

* `str`: A pointer to a null-terminated string to be converted.

#### **Returns:**

* A new string with all characters converted to lowercase, or `NULL` if an error occurs.

#### **Errors:**

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

---

### `xtoupper`

Converts a string to uppercase.

#### **Parameters:**

* `str`: A pointer to a null-terminated string to be converted.

#### **Returns:**

* A new string with all characters converted to uppercase, or `NULL` if an error occurs.

#### **Errors:**

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

---

### `xtoascii`

Converts a string to ASCII characters, replacing any non-ASCII characters with `?`.

#### **Parameters:**

* `str`: A pointer to a null-terminated string to be converted.

#### **Returns:**

* A new string containing only ASCII characters, or `NULL` if an error occurs.

#### **Errors:**

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

---

## **Memory Management**

Each of the functions in the XCTYPE library allocates memory for the resulting string. The caller is responsible for freeing this memory using `free()` once the returned string is no longer needed to avoid memory leaks.

---

## **Error Handling**

The functions return `NULL` if an error occurs, and set the `errno` variable to indicate the type of error. The possible error values are:

* `EINVAL`: Invalid argument (if `str` is `NULL`).
* `ENOMEM`: Out of memory (if memory allocation fails).

Make sure to check the return value before using the result.

---

## **Example Usage**

Here is an example showing how to use the provided functions:

```c
#include <stdlib.h>
#include "xctype.h"

int main() {
    char* str = "Hello World!";
    char* lower_str = xtolower(str);
    char* upper_str = xtoupper(str);
    char* ascii_str = xtoascii(str);

    // Always check if the result is NULL before using the string
    if (lower_str != NULL) {
        printf("Lowercase: %s\n", lower_str);
        free(lower_str);
    }

    if (upper_str != NULL) {
        printf("Uppercase: %s\n", upper_str);
        free(upper_str);
    }

    if (ascii_str != NULL) {
        printf("ASCII: %s\n", ascii_str);
        free(ascii_str);
    }

    return 0;
}
```

---

## **License**

This library is distributed under the **GNU GPL v3 or later**.
