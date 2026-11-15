/**
 * xfcntl.h: Cross-Platform Fcntl Utilities
 *
 * Copyright (C) 2026 MrR736 <MrR736@users.github.com>
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

#ifndef __XFCNTL_H__
#define __XFCNTL_H__

#include "xstddef.h"

#ifdef _WIN32
#define XF_DUPFD		0
#define XF_GETFD		1
#define XF_SETFD		2
#define XF_GETFL		3
#define XF_SETFL		4
#else
#include <fcntl.h>

#define XF_DUPFD		F_DUPFD	/* Duplicate file descriptor.  */
#define XF_GETFD		F_GETFD	/* Get file descriptor flags.  */
#define XF_SETFD		F_SETFD	/* Set file descriptor flags.  */
#define XF_GETFL		F_GETFL	/* Get file status flags.  */
#define XF_SETFL		F_SETFL	/* Set file status flags.  */
#endif

#ifndef __cplusplus
extern "C" {
#endif


#include "xfcntl.h"

#include <stdarg.h>
#include <errno.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

XSTDMETHOD(int) xfcntl (int fd, int cmd, ...) {
	va_list ap;
	int result = -1;

	va_start (ap, cmd);

#ifdef _WIN32

	switch (cmd) {
		case XF_DUPFD:
		{
			int minfd = va_arg (ap, int);
			result = _dup (fd);

			if (result >= 0 && result < minfd) {
				_close (result);
				result = -1;
				errno = EMFILE;
			}

			break;
		}

		case XF_GETFD:
			/* Windows CRT does not expose POSIX descriptor flags. */
			errno = 0;
			result = 0;
			break;

		case XF_SETFD:
			/* No equivalent to FD_CLOEXEC in the Windows CRT. */
			(void) va_arg (ap, int);
			errno = ENOSYS;
			break;

		case XF_GETFL:
		{
			int mode = _setmode (fd, _O_BINARY);

			if (mode != -1) {
				_setmode (fd, mode);
				result = mode;
			}

			break;
		}

		case XF_SETFL:
			/* File status flags cannot be changed portably here. */
			(void) va_arg (ap, int);
			errno = ENOSYS;
			break;

		default:
			errno = EINVAL;
			break;
	}

#else

	switch (cmd) {
		case XF_DUPFD:
		case XF_SETFD:
		case XF_SETFL:
			result = fcntl (fd, cmd, va_arg (ap, int));
			break;

		case XF_GETFD:
		case XF_GETFL:
			result = fcntl (fd, cmd);
			break;

		default:
			errno = EINVAL;
			break;
	}

#endif

	va_end (ap);
	return result;
}


#ifndef __cplusplus
}
#endif

#endif