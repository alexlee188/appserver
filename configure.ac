#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])

AC_INIT([appserver], [Alex Lee], [alexlee188 att gmail dott com])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([README])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([build-aux])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
# compiling `client.c' with per-target flags requires `AM_PROG_CC_C_O' in `configure.ac'
AM_PROG_CC_C_O

# Python checks for pylauncher
AM_PATH_PYTHON([2.5],, [:])

#
# attempt to build the GUI
#
have_GUI=yes


#
# Checking for build cpu
# and, if 64 bit, add some linking option
#
case $build_cpu in

    i686)
      AC_MSG_NOTICE([Platform: ix86 32 bit])
      # checks for platform dependent flags and compiler optimizations
      # AX_CHECK_COMPILE_FLAG(-ffast-math, SIMD_FLAGS="$SIMD_FLAGS -ffast-math", [])
      # AX_CHECK_COMPILE_FLAG(-fopenmp, SIMD_FLAGS="$SIMD_FLAGS -fopenmp", [])
      SIMD_FLAGS="$SIMD_FLAGS  -march=native "
      AC_SUBST(SIMD_FLAGS)    
    ;;

    x86_64)
      AC_MSG_NOTICE([Platform: ix86 64 bit])
      LDFLAGS="$LDFLAGS -L/lib64 -L/usr/lib64"
      export LDFLAGS
      AC_MSG_NOTICE([${LDFLAGS}])      
      # checks for platform dependent flags and compiler optimizations
      #AX_CHECK_COMPILE_FLAG(-ffast-math, SIMD_FLAGS="$SIMD_FLAGS -ffast-math", [])
      #AX_CHECK_COMPILE_FLAG(-fopenmp, SIMD_FLAGS="$SIMD_FLAGS -fopenmp", [])
      SIMD_FLAGS="$SIMD_FLAGS  -march=native "
      AC_SUBST(SIMD_FLAGS)    
    ;;

    armv6l)
      AC_MSG_NOTICE([Platform: ARM 6])
      CFLAGS="-O3 -pipe -march=armv6 -mfpu=vfp -mfloat-abi=hard"
      CXXFLAGS="-O3 -pipe -march=armv6 -mfpu=vfp -mfloat-abi=hard"
      export CFLAGS
      export CXXFLAGS
      AC_MSG_NOTICE([${CFLAGS}])
      AC_MSG_NOTICE([${CXXFLAGS}])
      # no GUI available on Raspberry PI
      have_GUI=no
    ;;

    armv7l)
      AC_MSG_NOTICE([Platform: ARM 7])
      CFLAGS="-O3 -pipe -march=armv7 -mfpu=neon -fno-tree-vectorize -fopenmp"
      CXXFLAGS="-O3 -pipe -march=armv7 -mfpu=neon -fno-tree-vectorize -fopenmp"
      export CFLAGS
      export CXXFLAGS
      AC_MSG_NOTICE([${CFLAGS}])
      AC_MSG_NOTICE([${CXXFLAGS}])
     ;;

esac

dnl # library (in DttSP) used but `RANLIB' is undefined:  the usual way to define `RANLIB' is to add `AC_PROG_RANLIB'
dnl #AC_PROG_RANLIB


#
# Check for compilation options which affect debugging
#
AC_ARG_ENABLE([debug-threads],
              AC_HELP_STRING([--enable-debug-threads],
                             [Enable thread debugging facilities in dspserver]),
              [threaddebug=$enableval], [threaddebug=no])
THREAD_DEBUG_CFLAGS=""
if test "x$threaddebug" = "xyes"; then
    THREAD_DEBUG_CFLAGS="-DTHREAD_DEBUG"
fi
AC_SUBST([THREAD_DEBUG_CFLAGS])

#
# Checks for libraries.
# Check both libraries binary and header files
#

have_m=no
AC_SEARCH_LIBS([round], [m],[have_m=yes])

have_pthreads=no
AC_SEARCH_LIBS([pthread_create], [pthread],[have_pthreads=yes])


have_config=no
AC_SEARCH_LIBS([config_init], [config],[have_config=yes])

have_event20=no
AC_SEARCH_LIBS([event_init], [event],[have_event20=yes])

have_event20_pthreads=no
AC_SEARCH_LIBS([evthread_use_pthreads], [event_pthreads],
               [have_event20_pthreads=yes])

have_event20_openssl=no
AC_SEARCH_LIBS([bufferevent_openssl_socket_new], [event_openssl],[have_event20_openssl=yes])

have_xml2=no
AC_SEARCH_LIBS([xmlCleanupParser], [xml2],[have_xml2=yes])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h malloc.h netdb.h netinet/in.h stddef.h stdlib.h string.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/timeb.h unistd.h])

if test "x${have_pthreads}" = xyes; then
    AC_CHECK_HEADERS([pthread.h], [], [have_pthreads=no])
fi

if test "x${have_pthreads}" = xno; then
    AC_MSG_ERROR([
-----------------------------------
  The pthread library and header file required to build ghpsdr3.
-----------------------------------])

fi

if test "x${have_config}" = xyes; then
    AC_CHECK_HEADERS([libconfig.h], [], [have_config=no])
fi

if test "x${have_config}" = xno; then
    AC_MSG_ERROR([
-----------------------------------
  The libconfig8 library and header file required to build ghpsdr3.
-----------------------------------])

fi


if test "x${have_event20}" = xyes; then
    AC_CHECK_HEADERS([event2/event.h], [], [have_event20=no])
fi

if test "x${have_event20}" = xno; then
    AC_MSG_ERROR([
-----------------------------------
  The libevent-2.0 library and header file required to build ghpsdr3. See README for #installation instructions.
-----------------------------------])
fi

if test "x${have_event20_pthreads}" = xyes; then
    AC_CHECK_HEADERS([event2/thread.h], [], [have_event20_pthreads=no])
fi

if test "x${have_event20_pthreads}" = xno; then
    AC_MSG_ERROR([
-----------------------------------
  The libevent-2.0 library must be built with pthreads support to build
  ghpsdr3.  See the README file for installation instructions.
-----------------------------------
])
fi

if test "x${have_event20_openssl}" = xyes; then
    AC_CHECK_HEADERS([event2/bufferevent_ssl.h], [], [have_event20_openssl=no])
fi

if test "x${have_event20_openssl}" = xyes; then
    AC_CHECK_HEADERS([openssl/ssl.h], [], [have_event20_openssl=no])
fi

if test "x${have_event20_openssl}" = xno; then
    AC_MSG_ERROR([
-----------------------------------
  The libevent-2.0 library and header file with Openssl support required.
  Required lib is libevent-openssl.  You also need to install libssl-dev.
-----------------------------------])
fi

if test "x${have_xml2}" = xyes; then
    AC_CHECK_HEADERS([libxml2/libxml/xmlmodule.h], [], [have_xml2=no])
fi

if test "x${have_xml2}" = xno; then
    AC_MSG_ERROR([
-------------------------------------
    The libxml2 library and header file with xml support required.
-------------------------------------])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor ftime getcwd gethostbyname gettimeofday inet_ntoa memset pow select socket sqrt strrchr strstr strtol])

#
# Optionally built sub packages
#

#
# PERSEUS
#



AC_CONFIG_FILES([Makefile])


AC_OUTPUT

#
# check the compiler option really working
#
corw=`gcc -Q --help=target -march=native`

echo \
"-----------------------------------------------------

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Prefix: '${prefix}'.
Compiler: '${CC} ${CFLAGS} ${CPPFLAGS}'
Now type 'make @<:@<target>@:>@'

 where the otpional <target> is:

    all           - build all binaries
    install       - install everything


build:         ${build}
build vendor:  ${build_vendor}
build os:      ${build_os} 
build cpu:     ${build_cpu}

host:          ${host}
host cpu:      ${host_cpu}
host vendor:   ${host_vendor}
host os:       ${host_os}


target:        ${target}
target cpu:    ${target_cpu}
target vendor: ${target_vendor}
target os:     ${target_os} 

Compiler options in use:

${corw}

-----------------------------------------------------"
