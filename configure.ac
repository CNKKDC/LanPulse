# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([LanPulse], [1.0.0], [1557655177@qq.com])
AC_CONFIG_HEADERS([include/autoconfig.h])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE([foreign subdir-objects silent-rules])

# Enable stricter compiler warnings
AC_PROG_CC
AC_PROG_CC_STDC
if test "x$ac_cv_prog_cc_stdc" = xno; then
  AC_MSG_ERROR([C compiler does not support ANSI C])
fi

# Add Windows detection
AC_CANONICAL_HOST

# Check for Windows platform
case $host in
    *mingw*|*cygwin*|*msys*)
        is_windows=yes
        AC_DEFINE([WIN32], [1], [Define to 1 for Windows platforms])
        ;;
    *)
        is_windows=no
        ;;
esac

AM_CONDITIONAL([WINDOWS], [test "x$is_windows" = xyes])

# Windows needs ws2_32 library for sockets
if test "x$is_windows" = xyes; then
    AC_CHECK_LIB([ws2_32], [WSAStartup], [
        WINDOWS_LIBS="-lws2_32"
        AC_SUBST([WINDOWS_LIBS])
    ], [
        AC_MSG_ERROR([Windows Sockets library (ws2_32) not found])
    ])
fi

# Check for required programs
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Standard C checks
AC_HEADER_STDC
AC_CHECK_HEADERS([
    arpa/inet.h
    errno.h
    fcntl.h
    netdb.h
    netinet/in.h
    stddef.h
    stdint.h
    stdlib.h
    string.h
    strings.h
    sys/ioctl.h
    sys/socket.h
    sys/time.h
    time.h
    unistd.h
])

AC_CHECK_FUNCS([
    memset
    memcpy
    strdup
    strerror
    socket
    connect
    bind
    listen
    accept
    select
    poll
    gethostbyname
    gethostbyaddr
    inet_ntoa
    inet_aton
    inet_pton
    inet_ntop
])

# Check BSD socket support
AC_MSG_CHECKING([for BSD socket support])
AC_LINK_IFELSE([
    AC_LANG_PROGRAM([[
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
    ]], [[
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr;
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        close(sock);
    ]])
], [
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_BSD_SOCKETS], [1], [Define to 1 if you have BSD socket support])
], [
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([BSD socket support is required])
])

# Check for additional network libraries
AC_SEARCH_LIBS([socket], [socket])
AC_SEARCH_LIBS([gethostbyname], [nsl])
AC_SEARCH_LIBS([connect], [socket])
AC_SEARCH_LIBS([inet_ntoa], [nsl])

# OpenSSL check
AC_ARG_WITH([openssl],
    [AS_HELP_STRING([--without-openssl], [Build without OpenSSL support])],
    [], [with_openssl=yes])

if test "x$with_openssl" != xno; then
    # Check for OpenSSL headers
    AC_CHECK_HEADERS([openssl/ssl.h openssl/err.h openssl/crypto.h], [], [
        if test "x$with_openssl" = xyes; then
            AC_MSG_ERROR([OpenSSL headers not found])
        else
            AC_MSG_WARN([OpenSSL headers not found, disabling OpenSSL support])
            with_openssl=no
        fi
    ])

    if test "x$with_openssl" != xno; then
        # Check for OpenSSL libraries
        saved_LIBS="$LIBS"
        LIBS=""
        AC_CHECK_LIB([crypto], [CRYPTO_new_ex_data], [
            AC_CHECK_LIB([ssl], [SSL_new], [
                HAVE_OPENSSL=yes
                OPENSSL_LIBS="-lssl -lcrypto"
                AC_SUBST([OPENSSL_LIBS])
                AC_DEFINE([HAVE_OPENSSL], [1], [Define to 1 if you have OpenSSL])
            ], [
                if test "x$with_openssl" = xyes; then
                    AC_MSG_ERROR([OpenSSL library not found])
                else
                    AC_MSG_WARN([OpenSSL library not found, disabling OpenSSL support])
                    with_openssl=no
                fi
            ], [-lcrypto])
        ], [
            if test "x$with_openssl" = xyes; then
                AC_MSG_ERROR([OpenSSL crypto library not found])
            else
                AC_MSG_WARN([OpenSSL crypto library not found, disabling OpenSSL support])
                with_openssl=no
            fi
        ])
        LIBS="$saved_LIBS"
    fi
fi

AM_CONDITIONAL([HAVE_OPENSSL], [test "x$with_openssl" != xno])

# Lua check
AC_ARG_WITH([lua],
    [AS_HELP_STRING([--with-lua@<:@=PREFIX@:>@], [Specify Lua prefix])],
    [], [with_lua=yes])

lua_found=no
if test "x$with_lua" != xno; then
    # Try different Lua versions
    for lua_version in 5.4 5.3 5.2 5.1; do
        if test "x$lua_found" = xno; then
            # Check for Lua headers
            AC_CHECK_HEADER([lua$lua_version/lua.h], [
                lua_include="lua$lua_version/lua.h"
                lua_found=yes
                LUA_VERSION=$lua_version
                break
            ])
            
            if test "x$lua_found" = xno; then
                AC_CHECK_HEADER([lua/lua$lua_version.h], [
                    lua_include="lua/lua$lua_version.h"
                    lua_found=yes
                    LUA_VERSION=$lua_version
                    break
                ])
            fi
            
            if test "x$lua_found" = xno; then
                AC_CHECK_HEADER([lua.h], [
                    lua_include="lua.h"
                    lua_found=yes
                    LUA_VERSION=$lua_version
                    break
                ], [], [
                    #ifdef HAVE_LUA_H
                    #include <lua.h>
                    #endif
                ])
            fi
        fi
    done

    if test "x$lua_found" = xno; then
        if test "x$with_lua" = xyes; then
            AC_MSG_ERROR([Lua headers not found])
        else
            AC_MSG_WARN([Lua headers not found, disabling Lua support])
            with_lua=no
        fi
    fi

    if test "x$lua_found" = xyes; then
        # Check for Lua library
        saved_LIBS="$LIBS"
        lua_lib_found=no
        for lua_lib in lua$LUA_VERSION lua-$LUA_VERSION lua; do
            AC_CHECK_LIB([$lua_lib], [lua_open], [
                lua_lib_found=yes
                HAVE_LUA=yes
                LUA_LIBS="-l$lua_lib"
                AC_SUBST([LUA_LIBS])
                AC_DEFINE([HAVE_LUA], [1], [Define to 1 if you have Lua])
                AC_DEFINE_UNQUOTED([LUA_VERSION], ["$LUA_VERSION"], [Lua version])
                break
            ])
        done
        
        if test "x$lua_lib_found" = xno; then
            if test "x$with_lua" = xyes; then
                AC_MSG_ERROR([Lua library not found])
            else
                AC_MSG_WARN([Lua library not found, disabling Lua support])
                with_lua=no
            fi
        fi
        LIBS="$saved_LIBS"
    fi
fi

AM_CONDITIONAL([HAVE_LUA], [test "x$with_lua" != xno])

# Include Windows libraries in final link flags
LANPULSE_LIBS="@OPENSSL_LIBS@ @LUA_LIBS@ @WINDOWS_LIBS@"
AC_SUBST([LANPULSE_LIBS])

# Output files
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

# Print configuration summary
AC_MSG_NOTICE([Configuration summary:])
AC_MSG_NOTICE([  OpenSSL support: ${with_openssl:-no}])
AC_MSG_NOTICE([  Lua support: ${with_lua:-no}])
if test "x$with_lua" != xno; then
    AC_MSG_NOTICE([  Lua version: $LUA_VERSION])
fi