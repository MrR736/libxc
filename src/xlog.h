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

#include "xstdio.h"

#define XLOG_DEFAULT_TAG "XLOG"
#define XLOG_STREAM      stderr

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

#ifdef DEBUG
#  define XLOGD(tag,fmt,...) xlog_print(XLOG_DEBUG,tag,fmt,##__VA_ARGS__)
# else
#  define XLOGD(tag,fmt,...) 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI int XCALLAPI xlog_vcprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap) {
	va_list apc;
	va_copy(apc, ap);

	const char* logTag = tag ? tag : XLOG_DEFAULT_TAG;
#ifdef __ANDROID__
	(void)file;
	(void)line;
	int androidPriority;
	switch (prio) {
		case XLOG_DEFAULT:
			androidPriority = ANDROID_LOG_DEFAULT;
			break;

		case XLOG_VERBOSE:
			androidPriority = ANDROID_LOG_VERBOSE;
			break;

		case XLOG_DEBUG:
			androidPriority = ANDROID_LOG_DEBUG;
			break;

		case XLOG_INFO:
			androidPriority = ANDROID_LOG_INFO;
			break;

		case XLOG_WARN:
			androidPriority = ANDROID_LOG_WARN;
			break;

		case XLOG_ERROR:
			androidPriority = ANDROID_LOG_ERROR;
			break;

		case XLOG_FATAL:
			androidPriority = ANDROID_LOG_FATAL;
			break;

		case XLOG_SILENT:
			androidPriority = ANDROID_LOG_SILENT;
			break;

		default:
			androidPriority = ANDROID_LOG_UNKNOWN;
			break;
	}
	int ret = __android_log_vprint(androidPriority,logTag,fmt,apc);
	va_end(apc);
	return ret;
#else
	if (prio == XLOG_SILENT) {
		va_end(apc);
		return 0;
	}

	time_t now = time(NULL);
	struct tm tm;

#ifdef _WIN32
	localtime_s(&tm, &now);
#else
	localtime_r(&now, &tm);
#endif
	char timestamp[64];
	strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",&tm);

	const char* priorityString;
	switch (prio) {
		case XLOG_DEFAULT:
			priorityString = "Default";
			break;

		case XLOG_VERBOSE:
			priorityString = "Verbose";
			break;

		case XLOG_DEBUG:
			priorityString = "DEBUG";
			break;

		case XLOG_INFO:
			priorityString = "INFO";
			break;

		case XLOG_WARN:
			priorityString = "Warn";
			break;

		case XLOG_ERROR:
			priorityString = "ERROR";
			break;

		case XLOG_FATAL:
			priorityString = "FATAL";
			break;

		case XLOG_SILENT:
			priorityString = NULL;
			break;

		default:
			priorityString = "Unknown";
			break;

	}
	fprintf(XLOG_STREAM, "%s ", timestamp);
	if (priorityString) fprintf(XLOG_STREAM,"[%s]\t",priorityString);
	fprintf(XLOG_STREAM,"%s ",logTag);
	if (file && line > 0) fprintf(XLOG_STREAM,"(%s:%d) ",xbasename(file),line);
	int ret = vfprintf(XLOG_STREAM,fmt,apc);
	va_end(apc);
	fputc('\n', XLOG_STREAM);
	fflush(XLOG_STREAM);
	return ret;
#endif
}

XSTDAPI int XCALLAPI xlog_cprint(int prio,int line,const char* file,const char* tag,const char* fmt,...) {
	va_list ap;
	va_start(ap,fmt);
	int ret = xlog_vcprint(prio,line,file,tag,fmt,ap);
	va_end(ap);
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif