/**
 * xlog.h: Cross-Platform Log Utilities
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

#ifndef __XLOG_H__
#define __XLOG_H__

#include <stdio.h>

#include "xstddef.h"

#define XLOG_DEFAULT_TAG "XLOG"
#define XLOG_STREAM stderr

typedef enum XLogPriority {
	XLOG_UNKNOWN = 0,	/**< Internal use only */
	XLOG_DEFAULT,		/**< Default priority, internal use only */
	XLOG_VERBOSE,		/**< Verbose logging (usually disabled in release) */
	XLOG_DEBUG,			/**< Debug logging (usually disabled in release) */
	XLOG_INFO,			/**< Informational logging */
	XLOG_WARN,			/**< Warning logging, recoverable failures */
	XLOG_ERROR,			/**< Error logging, unrecoverable failures */
	XLOG_FATAL,			/**< Fatal logging, for abort scenarios */
	XLOG_SILENT,		/**< Internal use only */
} XLogPriority;

/** Convenience macros for printing logs with automatic file and line info */
#define xlog_vprint(prio,tag,fmt,ap) xlog_vcprint(prio,__LINE__,__FILE__,tag,fmt,ap)
#define xlog_print(prio,tag,fmt,...) xlog_cprint(prio,__LINE__,__FILE__,tag,fmt,##__VA_ARGS__)

/** Standard log level macros */
#define XLOGE(tag,fmt,...) xlog_print(XLOG_ERROR,tag,fmt,##__VA_ARGS__)
#define XLOGI(tag,fmt,...) xlog_print(XLOG_INFO,tag,fmt,##__VA_ARGS__)
#define XLOGV(tag,fmt,...) xlog_print(XLOG_VERBOSE,tag,fmt,##__VA_ARGS__)
#define XLOGW(tag,fmt,...) xlog_print(XLOG_WARN, tag,fmt,##__VA_ARGS__)
#define XLOGF(tag,fmt,...) xlog_print(XLOG_FATAL,tag,fmt,##__VA_ARGS__)
#define XLOGS(tag,fmt,...) xlog_print(XLOG_SILENT,tag,fmt,##__VA_ARGS__)

#ifdef DEBUG
# define XLOGD(tag,fmt,...) xlog_print(XLOG_DEBUG,tag,fmt,##__VA_ARGS__)
#else
# define XLOGD(tag,fmt,...) 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI xlog_vcprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap);

XSTDAPI int XCALLAPI xlog_cprint(int prio,int line,const char* file,const char* tag,const char* fmt,...);

#ifdef __cplusplus
}
#endif

#endif