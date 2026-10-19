#include <stdio.h>
#include "xstddef.h"

XSTDAPI int XCALLAPI xprintf(const char *xrestrict fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	int ret = vprintf(fmt,ap);
	va_end(ap);
	return ret;
}