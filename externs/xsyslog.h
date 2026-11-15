/**
 * xsyslog.h: Cross-Platform System Logging Interface
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

#ifndef __XSYSLOG_H__
#define __XSYSLOG_H__

#include "xstddef.h"
#include "xstring.h"
#include "xstdio.h"

#ifdef _WIN32
#define	XLOG_EMERG	0
#define	XLOG_ALERT	1
#define	XLOG_CRIT	2
#define	XLOG_ERR	3
#define	XLOG_WARNING	4
#define	XLOG_NOTICE	5
#define	XLOG_INFO	6
#define	XLOG_DEBUG	7

#define	XLOG_PRIMASK	0x07

#define XLOG_PRI(p) ((p) & LOG_PRIMASK)
#define XLOG_MAKEPRI(fac, pri)	((fac) | (pri))

#define	XLOG_KERN	(0 << 3)
#define	XLOG_USER	(1 << 3)
#define	XLOG_MAIL	(2 << 3)
#define	XLOG_DAEMON	(3 << 3)
#define	XLOG_AUTH	(4 << 3)
#define	XLOG_SYSLOG	(5 << 3)
#define	XLOG_LPR	(6 << 3)
#define	XLOG_NEWS	(7 << 3)
#define	XLOG_UUCP	(8 << 3)
#define	XLOG_CRON	(9 << 3)
#define	XLOG_AUTHPRIV	(10 << 3)
#define	XLOG_FTP	(11 << 3)

#define	XLOG_PID	0x01
#define	XLOG_CONS	0x02
#define	XLOG_ODELAY	0x04
#define	XLOG_NDELAY	0x08
#define	XLOG_NOWAIT	0x10
#define	XLOG_PERROR	0x20

#define	XLOG_NFACILITIES	24

#define	XLOG_FACMASK	0x03f8
#define	XLOG_FAC(p)	(((p) & XLOG_FACMASK) >> 3)
#else
#include <sys/syslog.h>

/* system is unusable */
#define	XLOG_EMERG	LOG_EMERG

/* action must be taken immediately */
#define	XLOG_ALERT	LOG_ALERT

/* critical conditions */
#define	XLOG_CRIT	LOG_CRIT

/* error conditions */
#define	XLOG_ERR	LOG_ERR

/* warning conditions */
#define	XLOG_WARNING	LOG_WARNING

/* normal but significant condition */
#define	XLOG_NOTICE	LOG_NOTICE

/* informational */
#define	XLOG_INFO	LOG_INFO

/* debug-level messages */
#define	XLOG_DEBUG	LOG_DEBUG

/* mask to extract priority part (internal) */
#define	XLOG_PRIMASK	LOG_PRIMASK

/* extract priority */
#define	XLOG_PRI(p)	LOG_PRI(p)

#define	XLOG_MAKEPRI(fac, pri)	LOG_MAKEPRI(fac, pri)

/* kernel messages */
#define	XLOG_KERN	LOG_KERN

/* random user-level messages */
#define	XLOG_USER	LOG_USER

/* mail system */
#define	XLOG_MAIL	LOG_MAIL

/* system daemons */
#define	XLOG_DAEMON	LOG_DAEMON

/* security/authorization messages */
#define	XLOG_AUTH	LOG_AUTH

/* messages generated internally by syslogd */
#define	XLOG_SYSLOG	LOG_SYSLOG

/* line printer subsystem */
#define	XLOG_LPR	LOG_LPR

/* network news subsystem */
#define	XLOG_NEWS	LOG_NEWS

/* UUCP subsystem */
#define	XLOG_UUCP	LOG_UUCP

/* clock daemon */
#define	XLOG_CRON	LOG_CRON

/* security/authorization messages (private) */
#define	XLOG_AUTHPRIV	LOG_AUTHPRIV

/* ftp daemon */
#define	XLOG_FTP	LOG_AUTHPRIV

/* log the pid with each message */
#define	XLOG_PID	LOG_PID

/* log on the console if errors in sending */
#define	XLOG_CONS	LOG_CONS

/* delay open until first syslog() (default) */
#define	XLOG_ODELAY	LOG_ODELAY

/* don't delay open */
#define	XLOG_NDELAY	LOG_NDELAY

/* don't wait for console forks: DEPRECATED */
#define	XLOG_NOWAIT	LOG_NOWAIT

/* log to stderr as well */
#define	XLOG_PERROR	LOG_PERROR

/* current number of facilities */
#define	XLOG_NFACILITIES	LOG_NFACILITIES

/* mask to extract facility part */
#define	XLOG_FACMASK	LOG_FACMASK

/* facility of pri */
#define	XLOG_FAC(p)	LOG_FAC(p)

#endif

#define XLOG_MASK(pri) (1 << (pri))

#define XLOG_UPTO(pri) ((1 << ((pri) + 1)) - 1)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
static struct {
	HANDLE event;
	char *ident;
	int option;
	int facility;
	int mask;
} __isyslog = { NULL, NULL, 0, 0, 0xFF };
#endif

/* Close descriptor used to write to system logger.
 *
 * This function is a possible cancellation point
 */
XSTDMETHOD(void) xcloselog (void) {
#ifdef _WIN32
	if (__isyslog.event) {
		DeregisterEventSource(__isyslog.event);
		__isyslog.event = NULL;
	}

	free(__isyslog.ident);
	__isyslog.ident = NULL;

	__isyslog.option = 0;
	__isyslog.facility = 0;
#else
	closelog();
#endif
}

/* Open connection to system logger.
 *
 * T his function is a possible cancellation point.
 */
XSTDMETHOD(void) xopenlog (const char *ident, int option, int facility) {
#ifdef _WIN32
	xcloselog();

	if (ident) __isyslog.ident = _strdup(ident);
	else __isyslog.ident = _strdup("Application");
	if (!__isyslog.ident) return;

	__isyslog.option = option;
	__isyslog.facility = facility;

	__isyslog.event = RegisterEventSourceA(NULL,__isyslog.ident);
#else
	openlog(ident,option,facility);
#endif
}

/* Set the log mask level.  */
XSTDMETHOD(int) xsetlogmask(int mask) {
#ifdef _WIN32
	int oldmask = __isyslog.mask;
	if (mask != 0) __isyslog.mask = mask;
	return oldmask;
#else
	return setlogmask(mask);
#endif
}

XSTDMETHOD(void) vxsyslog(int pri, const char *fmt, va_list ap) {
#ifdef _WIN32
	if (!fmt || !__isyslog.event) return;

	/* Extract priority. */
	int priority = XLOG_PRI(pri);

	/* Apply priority mask. */
	if (!(__isyslog.mask & XLOG_MASK(priority))) return;

	/* Allocate message buffer. */
	char *message = vcprintf(fmt, ap);
	if (!message) return;

	/* Map syslog priority to Windows event type. */
	WORD type;

	switch (priority) {
		case XLOG_EMERG:
		case XLOG_ALERT:
		case XLOG_CRIT:
		case XLOG_ERR:
			type = EVENTLOG_ERROR_TYPE;
			break;

		case XLOG_WARNING:
			type = EVENTLOG_WARNING_TYPE;
			break;

		case XLOG_NOTICE:
		case XLOG_INFO:
		case XLOG_DEBUG:
		default:
			type = EVENTLOG_INFORMATION_TYPE;
			break;
	}

	/* Submit event to Windows Event Log. */
	LPCSTR strings[] = { message };

	ReportEventA(
		__isyslog.event,
		type,
		(WORD)XLOG_FAC(pri),
		0,
		NULL,
		1,
		0,
		strings,
		NULL
	);
	xfree(message);
#else
    vsyslog(pri, fmt, ap);
#endif
}

#ifdef __cplusplus
}
#endif

#endif