AC_INIT([opendnp3], [1.1.0], [jadamcrain@automatak.com])
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

LT_INIT #initialize libtools

# optional support for building the java bindings
AC_ARG_WITH([java],
  [AS_HELP_STRING([--with-java@<:@=ARG@:>@],
    [use the Java JDK based on the given javac (ARG=javac)     
     @<:@ARG=yes@:>@ ])],
    [
       JAVAC=$withval	
       AX_JNI_INCLUDE_DIR
       want_java="yes"
       for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
       do
               CPPFLAGS="$CPPFLAGS -I$JNI_INCLUDE_DIR"
       done
    ],
    [want_java="no"])
AM_CONDITIONAL(WANT_JAVA, [test "$want_java" = yes])

# optional support for lcov/gcov
AC_ARG_WITH([lcov],
    [AS_HELP_STRING([--with-lcov@<:@=ARG@:>@],
    [lcov support])],
    [CXXFLAGS="--coverage -g -O0 -std=gnu++11"
    AC_MSG_NOTICE([--with-lcov=yes])],
    [AC_MSG_NOTICE([--with-lcov=no])])

AC_ARG_ENABLE([mocks],
    AS_HELP_STRING([--disable-mocks], [Build without mock interface implementations]),
    [opendnp3_use_mocks=$enableval],
    [opendnp3_use_mocks=yes])
if test "$opendnp3_use_mocks" = no; then
    CXXFLAGS="${CXXFLAGS} -DOPENDNP3_NO_MOCKS"
fi
AM_CONDITIONAL([OPENDNP3_USE_MOCKS], [test "$opendnp3_use_mocks" = yes])

AC_ARG_ENABLE([master],
    AS_HELP_STRING([--disable-master], [Build without DNP3 master support]),
    [opendnp3_use_master=$enableval],
    [opendnp3_use_master=yes])
if test "$opendnp3_use_master" = no; then
    CXXFLAGS="${CXXFLAGS} -DOPENDNP3_NO_MASTER"
fi
AM_CONDITIONAL([OPENDNP3_USE_MASTER], [test "$opendnp3_use_master" = yes])

AC_ARG_ENABLE([serial],
    AS_HELP_STRING([--disable-serial], [Build without serial support]),
    [opendnp3_use_serial=$enableval],
    [opendnp3_use_serial=yes])
if test "$opendnp3_use_serial" = no; then
    CXXFLAGS="${CXXFLAGS} -DOPENDNP3_NO_SERIAL"
fi
AM_CONDITIONAL([OPENDNP3_USE_SERIAL], [test "$opendnp3_use_serial" = yes])

AC_OUTPUT #actually output the configuration
