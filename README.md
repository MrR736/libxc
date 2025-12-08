# libxc — Extern C Utility Library

[![License: GPL-3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)

`libxc` is a lightweight C/C++ library providing extended standard utilities, including:

- String, memory, and wide-character manipulation
- File I/O helpers and safer printf variants
- Extended standard types and macros
- Locale and map utilities

It is designed to complement the C standard library with convenient, safe, and portable extensions.

---

## Modules

| Module  | Description                             | Markdown                     | Man page                     |
| ------- | --------------------------------------- | ---------------------------- | ---------------------------- |
| xstddef | Extended standard types and macros      | [xstddef.md](doc/xstddef.md) | [xstddef.3](doc/xstddef.3)   |
| xstdlib | Memory and utility helpers              | [xstdlib.md](doc/xstdlib.md) | [xstdlib.3](doc/xstdlib.3)   |
| xstring | String manipulation helpers             | [xstring.md](doc/xstring.md) | [xstring.3](doc/xstring.3)   |
| xwchar  | Wide-Character utilities                | [xwchar.md](doc/xwchar.md)   | [xwchar.3](doc/xwchar.3)     |
| xstdio  | File I/O helpers and safer printf       | [xstdio.md](doc/xstdio.md)   | [xstdio.3](doc/xstdio.3)     |
| xlocale | Locale-related utilities                | [xlocale.md](doc/xlocale.md) | [xlocale.3](doc/xlocale.3)   |
| xmap    | Map / associative array utilities       | [xmap.md](doc/xmap.md)       | [xmap.3](doc/xmap.3)         |
| xctype  | Extended Character Type Utilities       | [xctype.md](doc/xctype.md)   | [xctype.3](doc/xctype.3)     |
| xwctype | Extended Wide-Character Type Utilities  | [xwctype.md](doc/xwctype.md) | [xwctype.3](doc/xwctype.3)   |

---

## Getting Started

### Include headers

```c
#include "xstdio.h"
#include "xstring.h"
#include "xstdlib.h"
```

### Quick Examples

#### example.txt
```txt
Hello World!
```

#### Using `xstdio` to read a file

```c
#include "xstdio.h"
#include <stdio.h>

int main() {
    FILE *fp = fopen("example.txt", "r");
    if (!fp) return 1;

    size_t size;
    char *data = furead(fp, &size);
    fclose(fp);

    if (data) {
        printf("File content (%zu bytes):\n%s\n", size, data);
        free(data);
    }
    return 0;
}
```

#### Using `xstring` to format a string safely

```c
#include "xstring.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Example format string
    const char *name = "World";
    int number = 42;

    // 1. Calculate the length of the formatted string
    size_t len = xstrlen("Hello %s! Number: %d", name, number);
    if (len == 0) {
        fprintf(stderr, "Failed to calculate string length.\n");
        return 1;
    }

    // 2. Allocate buffer
    char *buf = (char *)malloc(len + 1);
    if (!buf) {
        perror("malloc");
        return 1;
    }

    // 3. Format the string safely
    snprintf(buf, len + 1, "Hello %s! Number: %d", name, number);

    // 4. Print the result
    printf("%s\n", buf);

    // 5. Free memory
    free(buf);

    return 0;
}
```

---

## License

This library is licensed under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.en.html).
You are free to use, modify, and redistribute it under the terms of the GPLv3.

---

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Make your changes
3. Submit a pull request with a clear description of your changes

Please follow the existing code style for consistency.

---

## Contact

Author: MrR736 — [MrR736@users.github.com](mailto:MrR736@users.github.com)
