AC_INIT([opendnp3], [1.1.1], [jadamcrain@automatak.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_COPYRIGHT([(C) 2010, 2011 Green EneryCorp. (C) 2013 Automatak, LLC. All right reserved.])
AM_INIT_AUTOMAKE([subdir-objects])

AC_PREFIX_DEFAULT([/usr]) #works best for Ubuntu
AC_CONFIG_FILES([Makefile])

AC_LANG([C++]) # tell autoconf that we're using C++
AX_CXX_COMPILE_STDCXX_11([ext]) #require support for C++11

AX_BOOST_BASE([1.45.0]  ,, [AC_MSG_ERROR(You must install boost >= 1.45.0)])
AX_BOOST_SYSTEM
AX_BOOST_ASIO
AX_BOOST_UNIT_TEST_FRAMEWORK

AC_CHECK_HEADERS([pthread.h],, AC_MSG_ERROR([missing header]))
AC_CHECK_LIB([pthread], [pthread_join],, AC_MSG_ERROR([missing library]))

LT_INIT #initialize libtools

AC_ARG_ENABLE([java],
    AS_HELP_STRING([--enable-java], [Build with support for the Java bindings]),
    [opendnp3_use_java=$enableval],
    [opendnp3_use_java=no])
if test "$opendnp3_use_java" = yes; then
    opendnp3_jni_found=no
    AC_ARG_WITH([jni-include-path],
        AS_HELP_STRING([--with-jni-include-path=/path/to/java/include], [Use the specified path to find jni.h]),
        [
            case "$withval" in
                yes)
                    AC_MSG_ERROR([--with-jni-include-path specified without defining a path])
                    ;;
                no)
                    AC_MSG_ERROR([--with-jni-include-path=no is an invalid choice])
                    ;;
                *)
                    AC_MSG_CHECKING([for jni.h]);
                    if test -f "$withval/jni.h"; then
                        AC_MSG_RESULT([yes])
                        CPPFLAGS="${CPPFLAGS} -I$withval"
                        opendnp3_jni_found=yes
                    else
                        AC_MSG_WARN([no])
                    fi
                    ;;
            esac
        ])
    AC_ARG_WITH([javac],
        AS_HELP_STRING([--with-javac=/path/to/javac], [Use the specific javac executable as the basis for finding jni.h]),
        [
            case "$withval" in
                yes)
                    AC_MSG_ERROR([--with-javac specified but no javac executable declared])
                    ;;
                no)
                    AC_MSG_ERROR([--with-javac=no is an invalid choice])
                    ;;
                *)
                    if test ! -f "$withval"; then
                        AC_MSG_ERROR([invalid javac executable $withval specified])
                    fi
                    JAVAC="$withval"
                    ;;
            esac
            AX_JNI_INCLUDE_DIR
            for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
            do
                CPPFLAGS="$CPPFLAGS -I$JNI_INCLUDE_DIR"
            done
            opendnp3_jni_found=yes
        ])
    if test "$opendnp3_jni_found" = no; then
        AC_MSG_CHECKING([whether \$JAVAC can be used to find jni.h])
        if test "x$JAVAC" != "x"; then
            AC_MSG_RESULT([possibly])
            AX_JNI_INCLUDE_DIR
            for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
            do
                CPPFLAGS="$CPPFLAGS -I$JNI_INCLUDE_DIR"
            done
            opendnp3_jni_found=yes
        else
            AC_MSG_RESULT([no])
        fi
    fi
    if test "$opendnp3_jni_found" = no; then
        AC_MSG_CHECKING([whether \$JAVA_HOME can be used to find jni.h])
        if test -n "$JAVA_HOME"; then
            AC_MSG_RESULT([possibly])
            JAVAC="$JAVA_HOME/bin/javac"
            if test -f "$JAVAC"; then
                AX_JNI_INCLUDE_DIR
                for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
                do
                    CPPFLAGS="$CPPFLAGS -I$JNI_INCLUDE_DIR"
                done
                opendnp3_jni_found=yes
            else
                AC_MSG_CHECKING([for jni.h]);
                if test -f "$JAVA_HOME/include/jni.h"; then
                    AC_MSG_RESULT([yes])
                    CPPFLAGS="${CPPFLAGS} -I$JAVA_HOME/include"
                    opendnp3_jni_found=yes
                else
                    AC_MSG_ERROR([no])
                fi
            fi
        else
            AC_MSG_RESULT([no])
        fi
    fi
    if test "$opendnp3_jni_found" = no; then
        AC_MSG_ERROR([could not find include path for jni.h])
    fi
fi
AM_CONDITIONAL([WANT_JAVA], [test "$opendnp3_use_java" = yes])

AC_ARG_ENABLE([lcov],
    AS_HELP_STRING([--enable-lcov], [Build with lcov support]),
    [opendnp3_use_lcov=$enableval],
    [opendnp3_use_lcov=no])
if test "$opendnp3_use_lcov" = yes; then
    CXXFLAGS="--coverage -g -O0 -std=gnu++11"
fi

AC_MSG_CHECKING([for mock interface support])
AC_ARG_ENABLE([mocks],
    AS_HELP_STRING([--disable-mocks], [Build without mock interface implementations]),
    [opendnp3_use_mocks=$enableval],
    [opendnp3_use_mocks=yes])
if test "$opendnp3_use_mocks" = no; then
    AC_MSG_RESULT([disabled])
    CPPFLAGS="${CPPFLAGS} -DOPENDNP3_NO_MOCKS"
else
    AC_MSG_RESULT([enabled])
fi
AM_CONDITIONAL([OPENDNP3_USE_MOCKS], [test "$opendnp3_use_mocks" = yes])

AC_MSG_CHECKING([for DNP3 master support])
AC_ARG_ENABLE([master],
    AS_HELP_STRING([--disable-master], [Build without DNP3 master support]),
    [opendnp3_use_master=$enableval],
    [opendnp3_use_master=yes])
if test "$opendnp3_use_master" = no; then
    AC_MSG_RESULT([disabled])
    CPPFLAGS="${CPPFLAGS} -DOPENDNP3_NO_MASTER"
else
    AC_MSG_RESULT([enabled])
fi
AM_CONDITIONAL([OPENDNP3_USE_MASTER], [test "$opendnp3_use_master" = yes])

AC_MSG_CHECKING([for DNP3 serial support])
AC_ARG_ENABLE([serial],
    AS_HELP_STRING([--disable-serial], [Build without serial support]),
    [opendnp3_use_serial=$enableval],
    [opendnp3_use_serial=yes])
if test "$opendnp3_use_serial" = no; then
    AC_MSG_RESULT([disabled])
    CPPFLAGS="${CPPFLAGS} -DOPENDNP3_NO_SERIAL"
else
    AC_MSG_RESULT([enabled])
fi
AM_CONDITIONAL([OPENDNP3_USE_SERIAL], [test "$opendnp3_use_serial" = yes])

AC_MSG_CHECKING([for boost::asio::steady_timer support])
AC_LANG_PUSH(C++)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <boost/version.hpp>
]], [[
#if BOOST_VERSION >= 105000
// Everything is okay
#else
#  error Boost version is too old
#endif
]])],[
    AC_MSG_RESULT(yes)
],[
    AC_MSG_RESULT([patch required])
    CPPFLAGS="${CPPFLAGS} -DOPENDNP3_BOOST_TIMER_PATCH"
])
AC_LANG_POP([C++])

AC_ARG_ENABLE([strict-warnings],
    AS_HELP_STRING([--enable-strict-warnings], [Build with -Werror support]),
    [opendnp3_use_strict_warnings=$enableval],
    [opendnp3_use_strict_warnings=no])
if test "$opendnp3_use_strict_warnings" = yes; then
    CXXFLAGS="${CXXFLAGS} -Werror"
fi

AC_OUTPUT #actually output the configuration
