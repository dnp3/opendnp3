AC_INIT([opendnp3], [1.1.0], [jadamcrain@automatak.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_COPYRIGHT([(C) 2010, 2011 Green EneryCorp. (C) 2013 Automatak, LLC. All right reserved.])
AM_INIT_AUTOMAKE([subdir-objects])

AC_PREFIX_DEFAULT([/usr]) #works best for Ubuntu
AC_CONFIG_FILES([Makefile])

AC_LANG([C++]) # tell autoconf that we're using C++
AX_CXX_COMPILE_STDCXX_11([noext]) #require support for C++11

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

AC_ARG_ENABLE([opendnp3nomocks],
     [  --enable-opendnp3nomocks    Build library w/o mock interface implementations],
     [case "${enableval}" in
       yes) opendnp3nomocks=true ;;
       no)  opendnp3nomocks=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-opendnp3nomocks]) ;;
     esac],[debug=false])
AM_CONDITIONAL([OPENDNP3_NO_MOCKS], [test x$opendnp3nomocks = xtrue])

AC_ARG_ENABLE([opendnp3nomaster],
     [  --enable-opendnp3dnpmaster    Build library w/o master support],
     [case "${enableval}" in
       yes) opendnp3nomaster=true ;;
       no)  opendnp3nomaster=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-opendnp3nomaster]) ;;
     esac],[debug=false])
AM_CONDITIONAL([OPENDNP3_NO_MASTER], [test x$opendnp3nomaster = xtrue])

AC_ARG_ENABLE([opendnp3noserial],
     [  --enable-opendnp3noserial    Build library w/o serial support],
     [case "${enableval}" in
       yes) opendnp3noserial=true ;;
       no)  opendnp3noserial=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-opendnp3noserial]) ;;
     esac],[debug=false])
AM_CONDITIONAL([OPENDNP3_NO_SERIAL], [test x$opendnp3noserial = xtrue])

AC_OUTPUT #actually output the configuration

