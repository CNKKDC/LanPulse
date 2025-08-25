/*
 *  Copyright (C) 2025 kkdc <1557655177@qq.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
 * @file lanpulse_common.h
 * @author kkdc <1557655177@qq.com>
 */

#ifndef __LANPULSE_COMMON_H__
#define __LANPULSE_COMMON_H__

#define TRUE   1
#define FALSE  0

#define _GNU_SOURCE
#define __USE_GNU

#if HAVE_CONFIG_H
#include <autoconf.h>
#endif

#ifndef CLS
#warning "L1 cache line size not detected during build. Assuming 64 bytes."
#define CLS 64
#endif

#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#if HAVE_STDIO_H
#include <stdio.h>
#endif

#if HAVE_STDDEF_H
#include <stddef.h>
#endif

#if HAVE_STDINT_h
#include <stdint.h>
#endif

#if HAVE_STDBOOL_H
#include <stdbool.h>
#endif

#if HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#if HAVE_SYS_SYSCALL_H
#include <sys/syscall.h>
#endif

#if HAVE_SYSCALL_H
#include <syscall.h>
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h> /* for gettid(2) */
#endif

#if HAVE_SCHED_H
#include <sched.h>     /* for sched_setaffinity(2) */
#endif

#ifdef HAVE_TYPE_U_LONG_NOT_DEFINED
typedef unsigned long int u_long;
#endif
#ifdef HAVE_TYPE_U_INT_NOT_DEFINED
typedef unsigned int u_int;
#endif
#ifdef HAVE_TYPE_U_SHORT_NOT_DEFINED
typedef unsigned short u_short;
#endif
#ifdef HAVE_TYPE_U_CHAR_NOT_DEFINED
typedef unsigned char u_char;
#endif

#include <pcre2.h>

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#else
#ifdef OS_WIN32
#include "win32-syslog.h"
#endif /* OS_WIN32 */
#endif /* HAVE_SYSLOG_H */

#ifdef OS_WIN32
#include "win32-misc.h"
#include "win32-service.h"
#endif /* OS_WIN32 */

#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#if HAVE_POLL_H
#include <poll.h>
#endif

#if HAVE_SYS_SIGNAL_H
#include <sys/signal.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#if HAVE_SYS_RANDOM_H
#include <sys/random.h>
#endif

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#if HAVE_NETDB_H
#include <netdb.h>
#endif

#if __CYGWIN__
#if !defined _X86_ && !defined __x86_64
#define _X86_
#endif
#endif

#if !__CYGWIN__
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
#endif /* !__CYGWIN__ */

#ifdef HAVE_WINDOWS_H
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <windows.h>
#endif

#ifdef HAVE_W32API_WINBASE_H
#include <w32api/winbase.h>
#endif

#ifdef HAVE_W32API_WTYPES_H
#include <w32api/wtypes.h>
#endif

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

#ifdef HAVE_GRP_H
#include <grp.h>
#endif

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#include <jansson.h>
#ifndef JSON_ESCAPE_SLASH
#define JSON_ESCAPE_SLASH 0
#endif

#ifdef HAVE_MAGIC
#include <magic.h>
#endif

#ifdef HAVE_MATH_H
#include <math.h>
#endif

/* http://gcc.gnu.org/onlinedocs/gcc-3.4.1/cpp/Stringification.html#Stringification */
#define xstr(s) str(s)
#define str(s) #s

#if CPPCHECK==1
    #define BUG(x) if (((x))) exit(1)
#else
    #if defined HAVE_ASSERT_H && !defined NDEBUG
    #include <assert.h>
        #define BUG(x) assert(!(x))
    #else
        #define BUG(x) do {      \
            if (((x))) {            \
                fprintf(stderr, "BUG at %s:%d(%s)\n", __FILE__, __LINE__, __func__);    \
                fprintf(stderr, "Code: '%s'\n", xstr((x)));                             \
                exit(EXIT_FAILURE); \
            }                       \
        } while(0)
    #endif
#endif

/** FreeBSD does not define __WORDSIZE, but it uses __LONG_BIT */
#ifndef __WORDSIZE
    #ifdef __LONG_BIT
        #define __WORDSIZE __LONG_BIT
    #else
        #ifdef LONG_BIT
            #define __WORDSIZE LONG_BIT
        #endif
    #endif
#endif

/** Windows does not define __WORDSIZE, but it uses __X86__ */
#ifndef __WORDSIZE
    #if defined(__X86__) || defined(_X86_) || defined(_M_IX86)
        #define __WORDSIZE 32
    #else
        #if defined(__X86_64__) || defined(_X86_64_) || \
            defined(__x86_64)   || defined(__x86_64__) || \
            defined(__amd64)    || defined(__amd64__)
            #define __WORDSIZE 64
        #endif
    #endif
#endif

/** if not succesful yet try the data models */
#ifndef __WORDSIZE
    #if defined(_ILP32) || defined(__ILP32__)
        #define __WORDSIZE 32
    #endif
    #if defined(_LP64) || defined(__LP64__)
        #define __WORDSIZE 64
    #endif
#endif

#ifndef __WORDSIZE
    #warning Defaulting to __WORDSIZE 32
    #define __WORDSIZE 32
#endif

#ifndef __BYTE_ORDER
    #if defined(BYTE_ORDER)
        #define __BYTE_ORDER BYTE_ORDER
    #elif defined(__BYTE_ORDER__)
        #define __BYTE_ORDER __BYTE_ORDER__
    #else
        #error "byte order not detected"
    #endif
#endif

#ifndef __LITTLE_ENDIAN
    #if defined(LITTLE_ENDIAN)
        #define __LITTLE_ENDIAN LITTLE_ENDIAN
    #elif defined(__ORDER_LITTLE_ENDIAN__)
        #define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
    #endif
#endif

#ifndef __BIG_ENDIAN
    #if defined(BIG_ENDIAN)
        #define __BIG_ENDIAN BIG_ENDIAN
    #elif defined(__ORDER_BIG_ENDIAN__)
        #define __BIG_ENDIAN __ORDER_BIG_ENDIAN__
    #endif
#endif

#if !defined(__LITTLE_ENDIAN) && !defined(__BIG_ENDIAN)
    #error "byte order: can't figure out big or little"
#endif

#ifndef MIN
#define MIN(x, y) (((x)<(y))?(x):(y))
#endif

#ifndef MAX
#define MAX(x, y) (((x)<(y))?(y):(x))
#endif

#define SWAP_VAR(T, X, Y) do{ \
        T t = (X);  (X) = (Y); (Y) = t; \
    } while(0)

#define BIT_U8(n)  ((uint8_t)(1 << (n)))
#define BIT_U16(n) ((uint16_t)(1 << (n)))
#define BIT_U32(n) (1UL  << (n))
#define BIT_U64(n) (1ULL << (n))

#define SWAP_FLAGS(F, A, B) do{ \
        if (((F) & ((A)|(B))) == (A)){ \
            (F) &= ~(A); (F) |= (B); \
        } else if (((F) & ((A)|(B))) == (B)){ \
            (F) &= ~(B); (F) |= (A); \
        } \
    } while(0)

#if defined(__MINGW32__)
#define ATTR_FMT_PRINTF(x, y) __attribute__((format(__MINGW_PRINTF_FORMAT, (x), (y))))
#elif defined(__GNUC__)
#define ATTR_FMT_PRINTF(x, y) __attribute__((format(printf, (x), (y))))
#else
#define ATTR_FMT_PRINTF(x, y)
#endif

#include <ctype.h>
#define u8_tolower(c) ((uint8_t)tolower((uint8_t)(c)))
#define u8_toupper(c) ((uint8_t)toupper((uint8_t)(c)))

#ifndef HAVE_LUA
typedef void lua_State;
#else
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

#ifndef HAVE_STRLCAT
size_t strlcat(char *, const char *src, size_t siz);
#endif
#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t siz);
#endif
#ifndef HAVE_STRPTIME
char *strptime(const char * __restrict, const char * __restrict, struct tm * __restrict);
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* __LANPULSE_COMMON_H__ */
