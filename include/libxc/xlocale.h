/**
 * xlocale.h: Extern Library
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

#ifndef __XLOCALE_H__
#define __XLOCALE_H__

#include "xtypes.h"
#include "xstddef.h"
#include <locale.h>

#define	XLC_ALL					LC_ALL
#define XLC_COLLATE				LC_COLLATE
#define XLC_CTYPE				LC_CTYPE
#define XLC_MONETARY			LC_MONETARY
#define XLC_NUMERIC				LC_NUMERIC
#define XLC_TIME				LC_TIME

#define XLC_MIN					XLC_ALL
#define XLC_MAX					XLC_TIME

#ifdef _WIN32
#define XLC_ALL_MASK			XLC_ALL
#define XLC_COLLATE_MASK		XLC_COLLATE
#define XLC_CTYPE_MASK			XLC_CTYPE
#define XLC_MONETARY_MASK		XLC_MONETARY
#define XLC_NUMERIC_MASK		XLC_NUMERIC
#define XLC_TIME_MASK			XLC_TIME
#else
#define XLC_MESSAGES			LC_MESSAGES
#define XLC_PAPER				LC_PAPER
#define XLC_NAME				LC_NAME
#define XLC_ADDRESS				LC_ADDRESS
#define XLC_TELEPHONE			LC_TELEPHONE
#define XLC_MEASUREMENT			LC_MEASUREMENT
#define XLC_IDENTIFICATION		LC_IDENTIFICATION

#define XLC_CTYPE_MASK			LC_CTYPE_MASK
#define XLC_NUMERIC_MASK		LC_NUMERIC_MASK
#define XLC_TIME_MASK			LC_TIME_MASK
#define XLC_COLLATE_MASK		LC_COLLATE_MASK
#define XLC_MONETARY_MASK		LC_MONETARY_MASK
#define XLC_MESSAGES_MASK		LC_MESSAGES_MASK
#define XLC_PAPER_MASK			LC_PAPER_MASK
#define XLC_NAME_MASK			LC_NAME_MASK
#define XLC_ADDRESS_MASK		LC_ADDRESS_MASK
#define XLC_TELEPHONE_MASK		LC_TELEPHONE_MASK
#define XLC_MEASUREMENT_MASK	LC_MEASUREMENT_MASK
#define XLC_IDENTIFICATION_MASK	LC_IDENTIFICATION_MASK
#define XLC_ALL_MASK			LC_ALL_MASK
#endif

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI wchar_t* XCALLAPI wsetlocale(int,const wchar_t*);

XSTDAPI char* XCALLAPI xsetlocale(int,const char*);

XSTDAPI wchar_t* XCALLAPI xwsetlocale(int,const wchar_t*);

XSTDAPI void XCALLAPI xfreelocale(xlocale_t);

XSTDAPI xlocale_t XCALLAPI xnewlocale(int,const char*,xlocale_t);

XSTDAPI void XCALLAPI xinit_console_utf8(void);

XSTDAPI void XCALLAPI xinit_console_wutf(void);

#ifdef __cplusplus
}
#endif

#endif // __XLOCALE_H__
