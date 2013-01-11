AC_INIT([opendnp3], [1.1.0], [open-dnp3@googlegroups.com])
AC_COPYRIGHT([(C) 2010, 2011 Green Energy Corp.  All rights reserved.])

srcdir="`cd $srcdir; pwd`"

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([config/autotools/m4])

AM_INIT_AUTOMAKE([subdir-objects -Wno-portability])

AC_PREFIX_DEFAULT([/usr])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])

AC_LANG([C++])

AX_BOOST_BASE([1.43],, AC_MSG_ERROR(missing boost))
AX_BOOST_DATE_TIME
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_SYSTEM
AX_BOOST_THREAD
AX_BOOST_UNIT_TEST_FRAMEWORK

AC_ARG_ENABLE(lcov,
  AS_HELP_STRING([--enable-lcov], [Enable gcov/lcov support]),
    [ac_lcov_enabled=$enableval], [ac_lcov_enabled=no])
if test "$ac_lcov_enabled" = yes; then
  AC_CHECK_PROGS(gcov, gcov)
  if ! test "$gcov"; then
    AC_MSG_ERROR([Could not find gcov utility, check your gcc instalation])
  fi
  AC_CHECK_LIB(gcov, __gcov_init, [], [])
  AC_CHECK_PROGS(lcov, lcov)
  AC_CHECK_PROGS(genhtml, genhtml)
  AC_CHECK_PROGS(gendesc, gendesc)
  AC_CHECK_PROGS(geninfo, geninfo)
  if ! test "$lcov" || ! test "$genhtml" || ! test "$gendesc" || 
     ! test "$geninfo"; then
    AC_MSG_ERROR([Could not find lcov utilities, do you have lcov installed?])
  fi
  CFLAGS="--coverage -g -O0"
  CXXFLAGS="--coverage -g -O0"
  LDFLAGS="--coverage"
  enable_static=yes
  enable_shared=no
fi
AM_CONDITIONAL(LCOV, [test "$ac_lcov_enabled" = yes])

AC_CHECK_HEADERS([arpa/inet.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([ctype.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([errno.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([fcntl.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([float.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([getopt.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([inttypes.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([math.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([pthread.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([setjmp.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([signal.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([stdarg.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([stdio.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([stdlib.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([string.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([sys/select.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([sys/stat.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([sys/types.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([sys/wait.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([syslog.h]	,,AC_MSG_ERROR(missing header))
AC_CHECK_HEADERS([unistd.h]	,,AC_MSG_ERROR(missing header))

AC_CHECK_LIB([c],	[atexit]		,,AC_MSG_ERROR(missing library))
AC_CHECK_LIB([pthread],	[pthread_join]		,,AC_MSG_ERROR(missing library))

AC_PROG_AWK
AC_PROG_CXX
AC_PROG_GREP
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_SED

want_swig="no"

AC_ARG_WITH([java],
  [AS_HELP_STRING([--with-java@<:@=ARG@:>@],
    [use the Java JDK from a standard location (ARG=yes),
     from the specified location (ARG=<path>),
     or disable it (ARG=no)
     @<:@ARG=yes@:>@ ])],
    [
    if test "$withval" = "no"; then
        want_java="no"
    elif test "$withval" = "yes"; then
        want_java="yes"
        ac_java_path=""
    else
        want_java="yes"
        ac_java_path="$withval"
    fi
    ],
    [want_java="no"])
if test "$want_java" = yes; then
  want_swig="yes"
  if test "x$ac_java_path" != "x"; then
    CPPFLAGS="$CPPFLAGS -I$ac_java_path"
  fi
  AC_CHECK_HEADERS([jni.h]	,,AC_MSG_ERROR(missing header))
  AM_PROG_GCJ
fi
AM_CONDITIONAL(WANT_JAVA, [test "$want_java" = yes])

AC_ARG_WITH([python],
  [AS_HELP_STRING([--with-python@<:@=ARG@:>@],
    [use the Python headers from a standard location (ARG=yes),
     from the specified location (ARG=<path>),
     or disable it (ARG=no)
     @<:@ARG=yes@:>@ ])],
    [
    if test "$withval" = "no"; then
        want_python="no"
    elif test "$withval" = "yes"; then
        want_python="yes"
        ac_python_path=""
    else
        want_python="yes"
        ac_python_path="$withval"
    fi
    ],
    [want_python="no"])
if test "$want_python" = yes; then
  want_swig="yes"
  AM_PATH_PYTHON([2.6],,AC_MSG_ERROR(Python 2.6 or later not found))
  AC_CHECK_LIB([python$PYTHON_VERSION], [PyObject_Malloc]  ,,AC_MSG_ERROR(missing library))
  AC_CHECK_LIB([python$PYTHON_VERSION], [PyExc_ValueError] ,,AC_MSG_ERROR(missing library))
  AC_CHECK_LIB([python$PYTHON_VERSION], [_Py_NoneStruct]   ,,AC_MSG_ERROR(missing library))
  if test "x$ac_python_path" != "x"; then
    CPPFLAGS="$CPPFLAGS -I$ac_python_path"
  fi
fi
AM_CONDITIONAL(WANT_PYTHON, [test "$want_python" = yes])

if test "$want_swig" = yes; then
  AX_PKG_SWIG(1.3.21, [], [ AC_MSG_ERROR([SWIG is required to build.]) ])
  AX_SWIG_ENABLE_CXX
  AX_SWIG_MULTI_MODULE_SUPPORT
  if test "$want_python" = yes; then
    SWIG_PYTHON
  fi
fi
AM_CONDITIONAL(SWIG_ENABLED, [test "$want_swig" = yes])

LT_INIT
dnl AX_LIBTOOLIZE_CFLAGS(CFLAGS)
dnl AX_LIBTOOLIZE_CFLAGS(CXXFLAGS)

AC_FUNC_FORK

AC_STRUCT_TIMEZONE

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AM_PROG_CC_C_O

DX_DOXYGEN_FEATURE(ON)
DX_DOT_FEATURE(OFF)
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN($PACKAGE_NAME, [config/doxygen.config])

AC_OUTPUT

echo "
$PACKAGE_NAME $PACKAGE_VERSION configuration:
-----------------------------------------------------
Source code location:       ${srcdir}
Host system type:           ${host}
Install path (prefix):      ${prefix}
Boost libraries:            ${BOOSTLIBDIR}

CXX:                        ${CXX}
CPPFLAGS:                   ${CPPFLAGS}
CXXFLAGS:                   ${CXXFLAGS}
LDFLAGS:                    ${LDFLAGS}
LIBS:                       ${LIBS}

java support:               ${want_java}
lcov support:               ${ac_lcov_enabled}
python support:             ${want_python}

shared libraries:           ${enable_shared}
static libraries:           ${enable_static}
-----------------------------------------------------

When ready, type 'make' to build.
"

